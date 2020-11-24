/*
 * Copyright 2020 Casey Sanchez
 */

#include "expression_composer.hpp"

ExpressionComposer::ExpressionComposer(std::shared_ptr<Node> const &node_ptr, std::map<std::string, std::shared_ptr<Node>> const &node_map) : m_node_ptr(node_ptr), m_node_map(node_map)
{
}

std::string ExpressionComposer::Compose() const
{
    return Compose(m_node_ptr);
}

std::string ExpressionComposer::Compose(std::shared_ptr<Node> const &node_ptr, uint32_t const &precedence) const
{
    if (node_ptr->Type() == "Node") {
        if (std::holds_alternative<Matrix>(node_ptr->Value())) {
            Matrix matrix = std::get<Matrix>(node_ptr->Value());

            std::ostringstream result;
            
            result << "\\begin{bmatrix}";

            for (size_t i = 0; i < matrix.Rows(); ++i) {
                for (size_t j = 0; j < matrix.Cols(); ++j) {
                    ExpressionComposer expression_composer(matrix(i, j), m_node_map);

                    result << expression_composer.Compose();
            
                    if (j < matrix.Cols() - 1) {
                        result << "&";
                    }
                }

                if (i < matrix.Rows() - 1) {
                    result << "\\\\";
                }
            }

            result << "\\end{bmatrix}";

            return result.str();
        }
    }
    else if (node_ptr->Type() == "Variable") {
        auto node_it = std::find_if(std::cbegin(m_node_map), std::cend(m_node_map), 
            [&node_ptr](std::pair<std::string, std::shared_ptr<Node>> const &node_pair) {  
                return node_pair.second == node_ptr;
            });

        if (node_it != std::cend(m_node_map)) {
            if (precedence < 1) {
                return "\\left(" + node_it->first + "\\right)";
            }
            
            return node_it->first;
        }
        else {
            std::complex<double> complex = std::get<std::complex<double>>(node_ptr->Value());

            if (!Approximately(complex.real(), 0.0) && !Approximately(complex.imag(), 0.0)) {
                return "\\left(" + std::to_string(complex.real()) + (complex.imag() > 0.0 ? "+" : "") + std::to_string(complex.imag()) + "i\\right)";
            }
            else if(!Approximately(complex.real(), 0.0)) {
                return "\\left(" + std::to_string(complex.real()) + "\\right)";
            }
            else if(!Approximately(complex.imag(), 0.0)) {
                return "\\left(" + std::to_string(complex.imag()) + "i\\right)";
            }
        }
    }
    else if (node_ptr->Type() == "Constant") {
        auto node_it = std::find_if(std::cbegin(m_node_map), std::cend(m_node_map), 
            [&node_ptr](std::pair<std::string, std::shared_ptr<Node>> const &node_pair) {  
                return node_pair.second == node_ptr;
            });

        if (node_it != std::cend(m_node_map)) {
            if (precedence < 1) {
                return "\\left(" + node_it->first + "\\right)";
            }
            
            return node_it->first;
        }
        else {
            return "\\left(" + std::to_string(std::get<std::complex<double>>(node_ptr->Value()).real()) + "+" + std::to_string(std::get<std::complex<double>>(node_ptr->Value()).imag()) + "i)";
        }
    }
    else if (node_ptr->Type() == "Exponentiation") {
        std::shared_ptr<Exponentiation> exponentiation = std::dynamic_pointer_cast<Exponentiation>(node_ptr);

        return Compose(exponentiation->Argument(0), 0) + "^" + Compose(exponentiation->Argument(1), 0);
    }
    else if (node_ptr->Type() == "Affirmation") {
        std::shared_ptr<Affirmation> affirmation = std::dynamic_pointer_cast<Affirmation>(node_ptr);

        if (precedence < 1) {
            return "\\left(+" + Compose(affirmation->Argument(0), 1) + "\\right)";
        }

        return "+" + Compose(affirmation->Argument(0), 1);
    }
    else if (node_ptr->Type() == "Negation") {
        std::shared_ptr<Negation> negation = std::dynamic_pointer_cast<Negation>(node_ptr);

        if (precedence < 1) {
            return "\\left(-" + Compose(negation->Argument(0), 1) + "\\right)";
        }

        return "-" + Compose(negation->Argument(0), 1);
    }
    else if (node_ptr->Type() == "Multiplication") {
        std::shared_ptr<Multiplication> multiplication = std::dynamic_pointer_cast<Multiplication>(node_ptr);

        if (precedence < 2) {
            return "\\left(" + Compose(multiplication->Argument(0), 2) + "*" + Compose(multiplication->Argument(1), 2) + "\\right)";
        }

        return Compose(multiplication->Argument(0), 2) + "*" + Compose(multiplication->Argument(1), 2);
    }
    else if (node_ptr->Type() == "Division") {
        std::shared_ptr<Division> division = std::dynamic_pointer_cast<Division>(node_ptr);

        if (precedence < 2) {
            return "\\left(" + Compose(division->Argument(0), 2) + "/" + Compose(division->Argument(1), 2) + "\\right)";
        }

        return Compose(division->Argument(0), 2) + "/" + Compose(division->Argument(1), 2);
    }
    else if (node_ptr->Type() == "Addition") {
        std::shared_ptr<Addition> addition = std::dynamic_pointer_cast<Addition>(node_ptr);

        if (precedence < 3) {
            return "\\left(" + Compose(addition->Argument(0), 3) + "+" + Compose(addition->Argument(1), 3) + "\\right)";
        }

        return Compose(addition->Argument(0), 3) + "+" + Compose(addition->Argument(1), 3);
    }
    else if (node_ptr->Type() == "Subtraction") {
        std::shared_ptr<Subtraction> subtraction = std::dynamic_pointer_cast<Subtraction>(node_ptr);

        if (precedence < 3) {
            return "\\left(" + Compose(subtraction->Argument(0), 3) + "-" + Compose(subtraction->Argument(1), 3) + "\\right)";
        }

        return Compose(subtraction->Argument(0), 3) + "-" + Compose(subtraction->Argument(1), 3);
    }
    else if (node_ptr->Type() == "Sin") {
        std::shared_ptr<Sin> sin = std::dynamic_pointer_cast<Sin>(node_ptr);

        return "sin" + Compose(sin->Argument(0), 0);
    }
    else if (node_ptr->Type() == "Cos") {
        std::shared_ptr<Cos> cos = std::dynamic_pointer_cast<Cos>(node_ptr);

        return "cos" + Compose(cos->Argument(0), 0);
    }
    else if (node_ptr->Type() == "Tan") {
        std::shared_ptr<Tan> tan = std::dynamic_pointer_cast<Tan>(node_ptr);

        return "tan" + Compose(tan->Argument(0), 0);
    }
    else if (node_ptr->Type() == "Asin") {
        std::shared_ptr<Asin> asin = std::dynamic_pointer_cast<Asin>(node_ptr);

        return "asin" + Compose(asin->Argument(0), 0);
    }
    else if (node_ptr->Type() == "Acos") {
        std::shared_ptr<Acos> acos = std::dynamic_pointer_cast<Acos>(node_ptr);

        return "acos" + Compose(acos->Argument(0), 0);
    }
    else if (node_ptr->Type() == "Atan") {
        std::shared_ptr<Atan> atan = std::dynamic_pointer_cast<Atan>(node_ptr);

        return "atan" + Compose(atan->Argument(0), 0);
    }
    else if (node_ptr->Type() == "Sqrt") {
        std::shared_ptr<Sqrt> sqrt = std::dynamic_pointer_cast<Sqrt>(node_ptr);

        return "sqrt" + Compose(sqrt->Argument(0), 0);
    }
    else if (node_ptr->Type() == "Abs") {
        std::shared_ptr<Abs> abs = std::dynamic_pointer_cast<Abs>(node_ptr);

        return "abs" + Compose(abs->Argument(0), 0);
    }
    else if (node_ptr->Type() == "Exp") {
        std::shared_ptr<Exp> exp = std::dynamic_pointer_cast<Exp>(node_ptr);

        return "exp" + Compose(exp->Argument(0), 0);
    }
    else if (node_ptr->Type() == "Log") {
        std::shared_ptr<Log> log = std::dynamic_pointer_cast<Log>(node_ptr);

        return "log" + Compose(log->Argument(0), 0);
    }
    else if (node_ptr->Type() == "Determinant") {
        std::shared_ptr<Determinant> determinant = std::dynamic_pointer_cast<Determinant>(node_ptr);

        return "det" + Compose(determinant->Argument(0), 0);
    }
    else if (node_ptr->Type() == "Inverse") {
        std::shared_ptr<Inverse> inverse = std::dynamic_pointer_cast<Inverse>(node_ptr);

        return "inv" + Compose(inverse->Argument(0), 0);
    }

    throw std::invalid_argument("Node of unknown type: " + node_ptr->Type());
}