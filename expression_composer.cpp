/*
 * Copyright 2020 Casey Sanchez
 */

#include "expression_composer.hpp"

ExpressionComposer::ExpressionComposer(std::shared_ptr<Node> const &node_ptr, std::map<std::string, std::shared_ptr<Node>> const &node_map) : m_node_ptr(node_ptr), m_node_map(node_map)
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
    Compose(ostream, m_node_ptr);
}

void ExpressionComposer::Compose(std::ostream &ostream, std::shared_ptr<Node> const &node_ptr, uint32_t const &precedence) const
{
    if (node_ptr->Type() == "MatrixNode") {
        Matrix matrix = node_ptr->MatrixValue();

        if (precedence == 1) {
            ostream << "\\left(";

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

            ostream << "\\right)";
        }
        else {
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
    }
    else if (node_ptr->Type() == "VariableNode") {
        auto node_it = std::find_if(std::cbegin(m_node_map), std::cend(m_node_map), 
            [&node_ptr](std::pair<std::string, std::shared_ptr<Node>> const &node_pair) {  
                return node_pair.second == node_ptr;
            });

        if (precedence == 1) {
            ostream << "\\left(";

            if (node_it != std::cend(m_node_map)) {
                ostream << node_it->first;
            }
            else {
                ostream << node_ptr;
            }

            ostream << "\\right)";
        }
        else {
            if (node_it != std::cend(m_node_map)) {
                ostream << node_it->first;
            }
            else {
                ostream << node_ptr;
            }
        }
    }
    else if (node_ptr->Type() == "ConstantNode") {
        auto node_it = std::find_if(std::cbegin(m_node_map), std::cend(m_node_map), 
            [&node_ptr](std::pair<std::string, std::shared_ptr<Node>> const &node_pair) {  
                return node_pair.second == node_ptr;
            });

        if (precedence == 1) {
            ostream << "\\left(";

            if (node_it != std::cend(m_node_map)) {
                ostream << node_it->first;
            }
            else {
                ostream << node_ptr;
            }

            ostream << "\\right)";
        }
        else {
            if (node_it != std::cend(m_node_map)) {
                ostream << node_it->first;
            }
            else {
                ostream << node_ptr;
            }
        }
    }
    else if (node_ptr->Type() == "ExponentiationNode") {
        if (precedence < 2) {
            ostream << "\\left("; 

            Compose(ostream, node_ptr->Argument(0), 2);
            
            ostream << "^"; 
            
            Compose(ostream, node_ptr->Argument(1), 2);

            ostream << "\\right)";
        }
        else {
            Compose(ostream, node_ptr->Argument(0), 2);
            
            ostream << "^"; 
            
            Compose(ostream, node_ptr->Argument(1), 2);
        }
    }
    else if (node_ptr->Type() == "MultiplicationNode") {
        if (precedence == 2) {
            ostream << "{"; 
            
            Compose(ostream, node_ptr->Argument(0), 3);
            
            ostream << "*"; 
            
            Compose(ostream, node_ptr->Argument(1), 3);
            
            ostream << "}";
        }
        else if (precedence < 3) {
            ostream << "\\left("; 
            
            Compose(ostream, node_ptr->Argument(0), 3);
            
            ostream << "*"; 
            
            Compose(ostream, node_ptr->Argument(1), 3);
            
            ostream << "\\right)";
        }
        else {
            Compose(ostream, node_ptr->Argument(0), 3);
            
            ostream << "*";
            
            Compose(ostream, node_ptr->Argument(1), 3);
        }
    }
    else if (node_ptr->Type() == "DivisionNode") {
        if (precedence == 2) {
            ostream << "{";
            
            Compose(ostream, node_ptr->Argument(0), 3);
            
            ostream << "/";
            
            Compose(ostream, node_ptr->Argument(1), 3);
            
            ostream << "}";
        }
        else if (precedence < 3) {
            ostream << "\\left(";
            
            Compose(ostream, node_ptr->Argument(0), 3);
            
            ostream << "/";
            
            Compose(ostream, node_ptr->Argument(1), 3);
            
            ostream << "\\right)";
        }
        else {
            Compose(ostream, node_ptr->Argument(0), 3);
            
            ostream << "/";
            
            Compose(ostream, node_ptr->Argument(1), 3);
        }
    }
    else if (node_ptr->Type() == "AdditionNode") {
        if (precedence == 2) {
            ostream << "{";
            
            Compose(ostream, node_ptr->Argument(0), 4);
            
            ostream << "+";
            
            Compose(ostream, node_ptr->Argument(1), 4);
            
            ostream << "}";
        }
        else if (precedence < 4) {
            ostream << "\\left(";
            
            Compose(ostream, node_ptr->Argument(0), 4);
            
            ostream << "+";
            
            Compose(ostream, node_ptr->Argument(1), 4);
            
            ostream << "\\right)";
        }
        else {
            Compose(ostream, node_ptr->Argument(0), 4);
            
            ostream << "+";
            
            Compose(ostream, node_ptr->Argument(1), 4);
        }
    }
    else if (node_ptr->Type() == "SubtractionNode") {
        if (precedence == 2) {
            ostream << "{";
            
            Compose(ostream, node_ptr->Argument(0), 4);
            
            ostream << "-";
            
            Compose(ostream, node_ptr->Argument(1), 4);
            
            ostream << "}";
        }
        else if (precedence < 4) {
            ostream << "\\left(";
            
            Compose(ostream, node_ptr->Argument(0), 4);
            
            ostream << "-";
            
            Compose(ostream, node_ptr->Argument(1), 4);
            
            ostream << "\\right)";
        }
        else {
            Compose(ostream, node_ptr->Argument(0), 4);
            
            ostream << "-";
            
            Compose(ostream, node_ptr->Argument(1), 4);
        }
    }
    else if (node_ptr->Type() == "SinNode") {
        ostream << "sin";
        
        Compose(ostream, node_ptr->Argument(0), 1);
    }
    else if (node_ptr->Type() == "CosNode") {
        ostream << "cos";
        
        Compose(ostream, node_ptr->Argument(0), 1);
    }
    else if (node_ptr->Type() == "TanNode") {
        ostream << "tan";
        
        Compose(ostream, node_ptr->Argument(0), 1);
    }
    else if (node_ptr->Type() == "AsinNode") {
        ostream << "asin";
        
        Compose(ostream, node_ptr->Argument(0), 1);
    }
    else if (node_ptr->Type() == "AcosNode") {
        ostream << "acos";
        
        Compose(ostream, node_ptr->Argument(0), 1);
    }
    else if (node_ptr->Type() == "AtanNode") {
        ostream << "atan";
        
        Compose(ostream, node_ptr->Argument(0), 1);
    }
    else if (node_ptr->Type() == "SqrtNode") {
        ostream << "sqrt";
        
        Compose(ostream, node_ptr->Argument(0), 1);
    }
    else if (node_ptr->Type() == "AbsNode") {
        ostream << "abs";
        
        Compose(ostream, node_ptr->Argument(0), 1);
    }
    else if (node_ptr->Type() == "ExpNode") {
        ostream << "exp";
        
        Compose(ostream, node_ptr->Argument(0), 1);
    }
    else if (node_ptr->Type() == "LogNode") {
        ostream << "log";
        
        Compose(ostream, node_ptr->Argument(0), 1);
    }
    else if (node_ptr->Type() == "DeterminantNode") {
        ostream << "det";
        
        Compose(ostream, node_ptr->Argument(0), 1);
    }
    else if (node_ptr->Type() == "InverseNode") {
        ostream << "inv";
        
        Compose(ostream, node_ptr->Argument(0), 1);
    }
    else {
        throw std::invalid_argument("Node of unknown type: " + node_ptr->Type());
    }
}

std::ostream &operator<<(std::ostream &ostream, ExpressionComposer const &expression_composer)
{
    expression_composer.Compose(ostream);

    return ostream;
}