#include <iostream>

#include <expression_parser.hpp>
#include <expression_simplifier.hpp>
#include <expression_composer.hpp>
#include <expression_visualizer.hpp>

int main(int argc, char *argv[]) {
    try {
        std::shared_ptr<Variable> x(new Variable(3.0));
        std::shared_ptr<Variable> y(new Variable(2.0));

        std::map<std::string, std::shared_ptr<Node>> node_map = { { "x", x }, { "y", y } };

        ExpressionParser expression_parser("(x+y)*(x+y)*(x+y)", node_map);

        std::shared_ptr<Node> node_ptr = ExpressionSimplifier(expression_parser.Parse(), node_map).Identify();

        std::cout << "Visualized expression tree: " << std::endl;
        std::cout << ExpressionVisualizer(node_ptr, node_map) << std::endl << std::endl;

        std::cout << "Re-composed expression from expression tree: " << std::endl;
        std::cout << ExpressionComposer(node_ptr, node_map) << std::endl << std::endl;
        
        std::shared_ptr<Node> simplified_ptr = ExpressionSimplifier(node_ptr, node_map).Simplify();

        std::cout << "Visualized expression tree: " << std::endl;
        std::cout << ExpressionVisualizer(simplified_ptr, node_map) << std::endl << std::endl;

        std::cout << "Re-composed expression from expression tree: " << std::endl;
        std::cout << ExpressionComposer(simplified_ptr, node_map) << std::endl << std::endl;
    }
    catch(std::exception const &exception) {
        std::cout << exception.what() << std::endl;
        
        return 1;
    }

    return 0;
}