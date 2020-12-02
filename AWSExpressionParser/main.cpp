#include <iostream> 
#include <sstream>

#include <aws/core/Aws.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/lambda-runtime/runtime.h>

#include <expression_parser.hpp>
#include <expression_composer.hpp>
#include <expression_simplifier.hpp>
#include <equation_parser.hpp>
#include <complex_parser.hpp>

aws::lambda_runtime::invocation_response handler(aws::lambda_runtime::invocation_request const &request)
{
    try {
        Aws::Utils::Json::JsonValue json_value(request.payload.c_str());
        Aws::Utils::Json::JsonView json_view(json_value);

        if (!json_view.ValueExists("action") || !json_view.GetObject("action").IsString()) {
            throw std::invalid_argument("No action provided");
        }

        if (!json_view.ValueExists("expression") || !json_view.GetObject("expression").IsString()) {
            throw std::invalid_argument("No expression provided");
        }

        std::string action_str(json_view.GetString("action").c_str());
        std::string expression_str(json_view.GetString("expression").c_str());

        std::map<std::string, std::shared_ptr<Node>> node_map;

        if (json_view.ValueExists("variables") && json_view.GetObject("variables").IsObject()) {
            for (auto const &item : json_view.GetObject("variables").GetAllObjects()) { 
                if (item.second.IsString()) {
                    ComplexParser complex_parser(std::string(item.second.AsString().c_str()));

                    std::complex<double> complex = complex_parser.Parse();

                    node_map[std::string(item.first.c_str())] = std::shared_ptr<VariableNode>(new VariableNode(complex));
                }
            }
        }

        std::ostringstream response_stream;

        response_stream << "\\begin{aligned}";
        
        std::regex action_regex("^(Evaluate|Solve for|Simplify)(.*)$");
        std::smatch action_match;

        if (std::regex_search(std::cbegin(action_str), std::cend(action_str), action_match, action_regex)) {
            if (action_match[1].str() == "Evaluate") {
                try {
                    EquationParser equation_parser(expression_str, node_map);

                    response_stream << (equation_parser.Equal() ? "True" : "False");
                }
                catch(std::invalid_argument const &) {
                    ExpressionParser expression_parser(expression_str, node_map);

                    std::shared_ptr<Node> node_ptr(expression_parser.Parse());

                    response_stream << node_ptr;
                }
            }
            else if (action_match[1].str() == "Solve for") {

            }
            else if (action_match[1].str() == "Simplify") {
                ExpressionParser expression_parser(expression_str, node_map);
                
                std::shared_ptr<Node> node_ptr(expression_parser.Parse());

                response_stream << "&=" << ExpressionComposer(node_ptr, node_map) << "\\\\";

                std::shared_ptr<Node> distributed_ptr = ExpressionSimplifier(node_ptr, node_map).Distribute();

                if (!Node::Equivalent(distributed_ptr, node_ptr)) {
                    response_stream << "&=" << ExpressionComposer(distributed_ptr, node_map) << "\\\\";
                }

                std::shared_ptr<Node> distributed_identified_ptr = ExpressionSimplifier(distributed_ptr, node_map).Identify();

                if (!Node::Equivalent(distributed_identified_ptr, distributed_ptr)) {
                    response_stream << "&=" << ExpressionComposer(distributed_identified_ptr, node_map) << "\\\\";
                }

                std::shared_ptr<Node> combined_factors_ptr = ExpressionSimplifier(distributed_identified_ptr, node_map).CombineFactors();
                
                response_stream << "&=" << ExpressionComposer(combined_factors_ptr, node_map) << "\\\\";

                std::shared_ptr<Node> combined_factors_identified_ptr = ExpressionSimplifier(combined_factors_ptr, node_map).Identify();

                if (!Node::Equivalent(combined_factors_identified_ptr, combined_factors_ptr)) {
                    response_stream << "&=" << ExpressionComposer(combined_factors_identified_ptr, node_map) << "\\\\";
                }

                std::shared_ptr<Node> combined_addends_ptr = ExpressionSimplifier(combined_factors_identified_ptr, node_map).CombineAddends();

                response_stream << "&=" << ExpressionComposer(combined_addends_ptr, node_map) << "\\\\";

                std::shared_ptr<Node> combined_addends_identified_ptr = ExpressionSimplifier(combined_addends_ptr, node_map).Identify();

                if (!Node::Equivalent(combined_addends_identified_ptr, combined_addends_ptr)) {
                    response_stream << "&=" << ExpressionComposer(combined_addends_identified_ptr, node_map);
                }
            }
            else {
                throw std::invalid_argument("No action provided");
            }
        }
        else {
            throw std::invalid_argument("No action provided");
        }

        response_stream << "\\end{aligned}";
        
        return aws::lambda_runtime::invocation_response(response_stream.str(), "text/plain", true);
    }
    catch (std::exception const &exception) {
        return aws::lambda_runtime::invocation_response(exception.what(), "text/plain", false);
    }
}

int main()
{
   aws::lambda_runtime::run_handler(handler);
   
   return 0;
}