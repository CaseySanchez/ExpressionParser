#include <iostream>

#include <expression_parser.hpp>
#include <expression_simplifier.hpp>
#include <expression_composer.hpp>
#include <expression_visualizer.hpp>

int main(int argc, char *argv[]) {
    try {
        std::shared_ptr<VariableNode> x(new VariableNode(3.0));
        std::shared_ptr<VariableNode> y(new VariableNode(2.0));

        std::map<std::string, std::shared_ptr<Node>> node_map = { { "x", x }, { "y", y } };

        ExpressionParser expression_parser("(x+y)*(x+y)+y^(x+1)", node_map);

        std::shared_ptr<Node> node_ptr = expression_parser.Parse();

        std::cout << "Visualized expression tree: " << std::endl;
        std::cout << ExpressionVisualizer(node_ptr, node_map) << std::endl << std::endl;

        std::cout << "Re-composed expression from expression tree: " << std::endl;
        std::cout << ExpressionComposer(node_ptr, node_map) << std::endl << std::endl;
        
        std::shared_ptr<Node> distributed_ptr = ExpressionSimplifier(ExpressionSimplifier(node_ptr, node_map).Distribute(), node_map).Identify();

        std::cout << "Visualized expression tree: " << std::endl;
        std::cout << ExpressionVisualizer(distributed_ptr, node_map) << std::endl << std::endl;

        std::cout << "Re-composed expression from expression tree: " << std::endl;
        std::cout << ExpressionComposer(distributed_ptr, node_map) << std::endl << std::endl;

        std::shared_ptr<Node> combined_factors_ptr = ExpressionSimplifier(ExpressionSimplifier(distributed_ptr, node_map).CombineFactors(), node_map).Identify();

        std::cout << "Visualized expression tree: " << std::endl;
        std::cout << ExpressionVisualizer(combined_factors_ptr, node_map) << std::endl << std::endl;

        std::cout << "Re-composed expression from expression tree: " << std::endl;
        std::cout << ExpressionComposer(combined_factors_ptr, node_map) << std::endl << std::endl;

        std::shared_ptr<Node> combined_addends_ptr = ExpressionSimplifier(ExpressionSimplifier(combined_factors_ptr, node_map).CombineAddends(), node_map).Identify();

        std::cout << "Visualized expression tree: " << std::endl;
        std::cout << ExpressionVisualizer(combined_addends_ptr, node_map) << std::endl << std::endl;

        std::cout << "Re-composed expression from expression tree: " << std::endl;
        std::cout << ExpressionComposer(combined_addends_ptr, node_map) << std::endl << std::endl;
    }
    catch(std::exception const &exception) {
        std::cout << exception.what() << std::endl;
        
        return 1;
    }

    return 0;
}