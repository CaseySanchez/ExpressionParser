#include "expression_parser.hpp"

int main(int argc, char *argv[])
{
    try {
        std::shared_ptr<Variable> x(new Variable(0.0));
        std::shared_ptr<Variable> y(new Variable(0.0));
        std::shared_ptr<Variable> z(new Variable(0.0));

        ExpressionParser expression_parser("3.14159^( -3 / 2 ) * exp( - ( x^2 + y^2 + z^2 ) )", { std::pair("x", x), std::pair("y", y), std::pair("z", z) });
        
        std::shared_ptr<Term> expression_root = expression_parser.Parse();

        for (double radius = 0.0; radius < 2.0; radius += 0.5) {
            for (double theta = 0.0; theta < 2.0 * 3.14159; theta += 0.25 * 3.14159) {
                for (double phi = 0.0; phi < 2.0 * 3.14159; phi += 0.25 * 3.14159) {
                    *x = radius * std::sin(theta) * std::cos(phi);
                    *y = radius * std::sin(theta) * std::sin(phi);
                    *z = radius * std::cos(theta);

                    std::cout << "psi(" << radius << ", " << theta << ", " << phi << ") = " << expression_root->Value() << std::endl;
                }
            }
        }
    }
    catch(std::exception const &exception) {
        std::cout << exception.what() << std::endl;
        
        throw exception;
    }

    return 0;
}