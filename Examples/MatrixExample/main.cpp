// Evaluates 3D wavefunction at various spherical coordinates where 
// psi(x, y, z) = pi^{-3/2} * e^{-(x^2+y^2+z^2)}
// such that int_{-infty}^{infty} int_{-infty}^{infty} int_{-infty}^{infty} psi(x, y, z) dz dy dx = 1

#include <iostream> 

#include <expression_parser.hpp>
#include <expression_composer.hpp>
#include <expression_simplifier.hpp>

int main(int argc, char *argv[])
{
    try {
        std::shared_ptr<Constant> theta(new Constant(3.14159 / 4.0));
        std::shared_ptr<Constant> x(new Constant(1.0));
        std::shared_ptr<Constant> y(new Constant(3.0));

        std::map<std::string, std::shared_ptr<Node>> node_map = { { "theta", theta }, { "x", x }, { "y", y } };

        //std::string expression_str = "\\begin{bmatrix} cos\\left(theta\\right) & -sin\\left(theta\\right) \\\\ sin\\left(theta\\right) & cos\\left(theta\\right) \\end{bmatrix} * \\begin{bmatrix} x \\\\ y \\end{bmatrix}";
        std::string expression_str = "\\begin{bmatrix} \\begin{bmatrix} 1 & 2 \\end{bmatrix} & \\begin{bmatrix} 3 & 4 \\end{bmatrix} \\\\ \\begin{bmatrix} 5 & 6 \\end{bmatrix} & \\begin{bmatrix} 7 & 8 \\end{bmatrix} \\end{bmatrix} * \\begin{bmatrix} \\begin{bmatrix} 1 \\\\ 2 \\end{bmatrix} & \\begin{bmatrix} 3 \\\\ 4 \\end{bmatrix} \\\\ \\begin{bmatrix} 5 \\\\ 6 \\end{bmatrix} & \\begin{bmatrix} 7 \\\\ 8 \\end{bmatrix} \\end{bmatrix}";

        ExpressionParser expression_parser(expression_str, node_map);

        std::shared_ptr<Node> matrix_node(expression_parser.Parse());

        std::cout << expression_str << std::endl << " = " << std::endl << matrix_node << std::endl;
        
        ExpressionComposer expression_composer(matrix_node, node_map);

        std::cout << "Re-composed expression from expression tree: " << std::endl;
        std::cout << expression_composer.Compose() << std::endl;
    }
    catch(std::exception const &exception) {
        std::cout << exception.what() << std::endl;
        
        return 1;
    }

    return 0;
}