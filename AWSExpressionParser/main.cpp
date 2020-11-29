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

#include "json.hpp"

aws::lambda_runtime::invocation_response handler(aws::lambda_runtime::invocation_request const &request)
{
    try {
        nlohmann::json json = nlohmann::json::parse(request.payload);

        if (!json.contains("action") || !json["action"].is_string()) {
            throw std::invalid_argument("No action provided");
        }

        if (!json.contains("expression") || !json["expression"].is_string()) {
            throw std::invalid_argument("No expression provided");
        }

        std::string action_str = json["action"].get<std::string>();
        std::string expression_str = json["expression"].get<std::string>();;

        std::map<std::string, std::shared_ptr<Node>> node_map;

        if (json.contains("variables") && json["variables"].is_object()) {
            for (auto const &item : json["variables"].items()) { 
                if (item.value().is_string()) {
                    ComplexParser complex_parser(item.value().get<std::string>());

                    std::complex<double> complex = complex_parser.Parse();

                    node_map[item.key()] = std::shared_ptr<Variable>(new Variable(complex));
                }
            }
        }

        if (json.contains("constants") && json["constants"].is_object()) {
            for (auto const &item : json["constants"].items()) { 
                if (item.value().is_string()) {
                    ComplexParser complex_parser(item.value().get<std::string>());

                    std::complex<double> complex = complex_parser.Parse();

                    node_map[item.key()] = std::shared_ptr<Constant>(new Constant(complex));
                }
            }
        }

        std::ostringstream response_stream;
        
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

            }
            else {
                throw std::invalid_argument("No action provided");
            }
        }
        else {
            throw std::invalid_argument("No action provided");
        }

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