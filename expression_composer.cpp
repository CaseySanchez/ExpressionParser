/*
 * Copyright 2020 Casey Sanchez
 */

#include "expression_composer.hpp"

ExpressionComposer::ExpressionComposer(std::variant<Scalar, Matrix> const &node_variant, std::map<std::string, std::variant<Scalar, Matrix>> const &node_map) : m_node_variant(node_variant), m_node_map(node_map)
{
}

std::string ExpressionComposer::Compose() const
{
    std::ostringstream ostringstream;

    Compose(ostringstream);

    return ostringstream.str();
}

void ExpressionComposer::Compose(std::ostream &ostream) const
{
    Compose(ostream, m_node_variant);
}

void ExpressionComposer::Compose(std::ostream &ostream, std::variant<Scalar, Matrix> const &node_variant, uint32_t const &precedence) const
{
    if (std::holds_alternative<Matrix>(node_variant)) {
        Matrix matrix = std::get<Matrix>(node_variant);

        ostream << "\\begin{bmatrix}";

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                ostream << ExpressionComposer(matrix(i, j), m_node_map);
        
                if (j < matrix.Cols() - 1) {
                    ostream << "&";
                }
            }

            if (i < matrix.Rows() - 1) {
                ostream << "\\\\";
            }
        }

        ostream << "\\end{bmatrix}";
    }
    else if (std::holds_alternative<Scalar>(node_variant)) {
        Scalar scalar = std::get<Scalar>(node_variant);

        if (scalar->Type() == "VariableNode") {
            auto node_it = std::find_if(std::cbegin(m_node_map), std::cend(m_node_map), 
                [&scalar](std::pair<std::string, std::variant<Scalar, Matrix>> const &node_pair) {  
                    return std::get<Scalar>(node_pair.second) == scalar;
                });

            if (node_it != std::cend(m_node_map)) {
                ostream << node_it->first;
            }
            else {
                ostream << scalar;
            }
        }
        else if (scalar->Type() == "ConstantNode") {
            auto node_it = std::find_if(std::cbegin(m_node_map), std::cend(m_node_map), 
                [&scalar](std::pair<std::string, std::variant<Scalar, Matrix>> const &node_pair) {  
                    return std::get<Scalar>(node_pair.second) == scalar;
                });

            if (node_it != std::cend(m_node_map)) {
                ostream << node_it->first;
            }
            else {
                ostream << scalar;
            }
        }
        else if (scalar->Type() == "ExponentiationNode") {
            Compose(ostream, Scalar(scalar->Argument(0)), 0);
            
            ostream << "^{"; 
            
            Compose(ostream, Scalar(scalar->Argument(1)), 0);

            ostream << "}";
        }
        else if (scalar->Type() == "MultiplicationNode") {
            if (precedence < 1) {
                ostream << "\\left("; 
                
                Compose(ostream, Scalar(scalar->Argument(0)), 1);
                
                ostream << "*"; 
                
                Compose(ostream, Scalar(scalar->Argument(1)), 1);
                
                ostream << "\\right)";
            }
            else {
                Compose(ostream, Scalar(scalar->Argument(0)), 1);
                
                ostream << "*";
                
                Compose(ostream, Scalar(scalar->Argument(1)), 1);
            }
        }
        else if (scalar->Type() == "DivisionNode") {
            if (precedence < 1) {
                ostream << "\\left(";

                ostream << "\\frac{";
                
                Compose(ostream, Scalar(scalar->Argument(0)), 1);
                
                ostream << "}{";
                
                Compose(ostream, Scalar(scalar->Argument(1)), 1);
                
                ostream << "}";

                ostream << "\\right)";
            }
            else {
                ostream << "\\frac{";

                Compose(ostream, Scalar(scalar->Argument(0)), 1);
                
                ostream << "}{";
                
                Compose(ostream, Scalar(scalar->Argument(1)), 1);

                ostream << "}";
            }
        }
        else if (scalar->Type() == "AdditionNode") {
            if (precedence < 2) {
                ostream << "\\left(";
                
                Compose(ostream, Scalar(scalar->Argument(0)), 2);
                
                ostream << "+";
                
                Compose(ostream, Scalar(scalar->Argument(1)), 2);
                
                ostream << "\\right)";
            }
            else {
                Compose(ostream, Scalar(scalar->Argument(0)), 2);
                
                ostream << "+";
                
                Compose(ostream, Scalar(scalar->Argument(1)), 2);
            }
        }
        else if (scalar->Type() == "SubtractionNode") {
            if (precedence < 2) {
                ostream << "\\left(";
                
                Compose(ostream, Scalar(scalar->Argument(0)), 2);
                
                ostream << "-";
                
                Compose(ostream, Scalar(scalar->Argument(1)), 2);
                
                ostream << "\\right)";
            }
            else {
                Compose(ostream, Scalar(scalar->Argument(0)), 2);
                
                ostream << "-";
                
                Compose(ostream, Scalar(scalar->Argument(1)), 2);
            }
        }
        else if (scalar->Type() == "SinNode") {
            ostream << "sin\\left(";
            
            Compose(ostream, Scalar(scalar->Argument(0)), ~0);
            
            ostream << "\\right)";
        }
        else if (scalar->Type() == "CosNode") {
            ostream << "cos\\left(";
            
            Compose(ostream, Scalar(scalar->Argument(0)), ~0);
            
            ostream << "\\right)";
        }
        else if (scalar->Type() == "TanNode") {
            ostream << "tan\\left(";
            
            Compose(ostream, Scalar(scalar->Argument(0)), ~0);
            
            ostream << "\\right)";
        }
        else if (scalar->Type() == "AsinNode") {
            ostream << "asin\\left(";
            
            Compose(ostream, Scalar(scalar->Argument(0)), ~0);
            
            ostream << "\\right)";
        }
        else if (scalar->Type() == "AcosNode") {
            ostream << "acos\\left(";
            
            Compose(ostream, Scalar(scalar->Argument(0)), ~0);
            
            ostream << "\\right)";
        }
        else if (scalar->Type() == "AtanNode") {
            ostream << "atan\\left(";
            
            Compose(ostream, Scalar(scalar->Argument(0)), ~0);
            
            ostream << "\\right)";
        }
        else if (scalar->Type() == "SqrtNode") {
            ostream << "sqrt\\left(";
            
            Compose(ostream, Scalar(scalar->Argument(0)), ~0);
            
            ostream << "\\right)";
        }
        else if (scalar->Type() == "AbsNode") {
            ostream << "abs\\left(";
            
            Compose(ostream, Scalar(scalar->Argument(0)), ~0);
            
            ostream << "\\right)";
        }
        else if (scalar->Type() == "ExpNode") {
            ostream << "exp\\left(";
            
            Compose(ostream, Scalar(scalar->Argument(0)), ~0);
            
            ostream << "\\right)";
        }
        else if (scalar->Type() == "LnNode") {
            ostream << "ln\\left(";
            
            Compose(ostream, Scalar(scalar->Argument(0)), ~0);
            
            ostream << "\\right)";
        }
        else if (scalar->Type() == "DeterminantNode") {
            ostream << "det\\left(";
            
            Compose(ostream, Scalar(scalar->Argument(0)), ~0);
            
            ostream << "\\right)";
        }
        else if (scalar->Type() == "InverseNode") {
            ostream << "inv\\left(";
            
            Compose(ostream, Scalar(scalar->Argument(0)), ~0);
            
            ostream << "\\right)";
        }
        else {
            throw std::invalid_argument("Node of unknown type: " + scalar->Type());
        }
    }
}

std::ostream &operator<<(std::ostream &ostream, ExpressionComposer const &expression_composer)
{
    expression_composer.Compose(ostream);

    return ostream;
}