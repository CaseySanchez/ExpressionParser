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
        std::shared_ptr<Constant> pi(new Constant(3.14159265358979323846));
        std::shared_ptr<Constant> e(new Constant(2.71828182845904523536));
        std::shared_ptr<Variable> x(new Variable(0.0));
        std::shared_ptr<Variable> y(new Variable(0.0));
        std::shared_ptr<Variable> z(new Variable(0.0));

        std::map<std::string, std::shared_ptr<Node>> node_map = { { "pi", pi }, { "e", e }, { "x", x }, { "y", y }, { "z", z } };

        ExpressionParser expression_parser("pi ^ \\left{ -3 / 2 \\right} * e ^ \\left{ - \\left( x ^ 2 + y ^ 2 + z ^ 2 \\right) \\right}", node_map);
        
        std::shared_ptr<Node> psi = expression_parser.Parse();

        for (double radius = 0.0; radius < 2.0; radius += 0.5) {
            for (double theta = 0.0; theta < 3.14159; theta += 0.5 * 3.14159) {
                for (double phi = 0.0; phi < 2.0 * 3.14159; phi += 0.5 * 3.14159) {
                    *x = radius * std::sin(theta) * std::cos(phi);
                    *y = radius * std::sin(theta) * std::sin(phi);
                    *z = radius * std::cos(theta);

                    std::cout << "psi(" << radius << ", " << theta << ", " << phi << ") = " << std::get<std::complex<double>>(psi->Value()) << std::endl;
                }
            }
        }

        ExpressionComposer expression_composer(psi, node_map);

        std::cout << "Re-composed expression from expression tree: " << std::endl;
        std::cout << expression_composer.Compose() << std::endl;
    }
    catch(std::exception const &exception) {
        std::cout << exception.what() << std::endl;
        
        return 1;
    }

    return 0;
}