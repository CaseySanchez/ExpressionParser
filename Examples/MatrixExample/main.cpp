// Transforms a 2D vector by a 2D rotation matrix

#include <iostream> 

#include <expression_parser.hpp>
#include <expression_composer.hpp>
#include <expression_simplifier.hpp>

int main(int argc, char *argv[])
{
    try {
        std::shared_ptr<Variable> theta(new Variable(3.14159 / 4.0));
        std::shared_ptr<Constant> x(new Constant(1.0));
        std::shared_ptr<Constant> y(new Constant(3.0));

        std::map<std::string, std::shared_ptr<Node>> node_map = { { "theta", theta }, { "x", x }, { "y", y } };

        std::string expression_str = "\\begin{bmatrix} cos\\left(theta\\right) & -sin\\left(theta\\right) \\\\ sin\\left(theta\\right) & cos\\left(theta\\right) \\end{bmatrix} * \\begin{bmatrix} x \\\\ y \\end{bmatrix}";
        
        ExpressionParser expression_parser(expression_str, node_map);

        std::shared_ptr<Node> matrix_node(expression_parser.Parse());

        for (int i = 1; i <= 8; ++i) {
            *theta = 2 * 3.14159 * (static_cast<double>(i) / 8.0);

            std::cout << "r(" << static_cast<Node>(*theta) << ") * v = " << matrix_node << std::endl;
        }

        std::cout << "Visualized expression: " << std::endl;
        std::cout << ExpressionVisualizer(matrix_node, node_map) << std::endl;

        std::cout << "Re-composed expression from expression tree: " << std::endl;
        std::cout << ExpressionComposer(matrix_node, node_map) << std::endl;
    }
    catch(std::exception const &exception) {
        std::cout << exception.what() << std::endl;
        
        return 1;
    }

    return 0;
}