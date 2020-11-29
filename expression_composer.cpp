/*
 * Copyright 2020 Casey Sanchez
 */

#include "expression_composer.hpp"

ExpressionComposer::ExpressionComposer(std::shared_ptr<Node> const &node_ptr, std::map<std::string, std::shared_ptr<Node>> const &node_map) : m_node_ptr(node_ptr), m_node_map(node_map)
{
}

void ExpressionComposer::Compose(std::ostream &ostream) const
{
    Compose(ostream, m_node_ptr);
}

void ExpressionComposer::Compose(std::ostream &ostream, std::shared_ptr<Node> const &node_ptr, uint32_t const &precedence) const
{
    if (node_ptr->Type() == "Node") {
        if (std::holds_alternative<Matrix>(node_ptr->Value())) {
            Matrix matrix = std::get<Matrix>(node_ptr->Value());

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
    else if (node_ptr->Type() == "Variable") {
        auto node_it = std::find_if(std::cbegin(m_node_map), std::cend(m_node_map), 
            [&node_ptr](std::pair<std::string, std::shared_ptr<Node>> const &node_pair) {  
                return node_pair.second == node_ptr;
            });

        if (node_it != std::cend(m_node_map)) {
            if (precedence < 1) {
                ostream << "\\left(" << node_it->first << "\\right)";
            }
            else {
                ostream << node_it->first;
            }
        }
        else {
            ostream << "\\left(" << node_ptr << "i\\right)";
        }
    }
    else if (node_ptr->Type() == "Constant") {
        auto node_it = std::find_if(std::cbegin(m_node_map), std::cend(m_node_map), 
            [&node_ptr](std::pair<std::string, std::shared_ptr<Node>> const &node_pair) {  
                return node_pair.second == node_ptr;
            });

        if (node_it != std::cend(m_node_map)) {
            if (precedence < 1) {
                ostream << "\\left(" << node_it->first << "\\right)";
            }
            else {
                ostream << node_it->first;
            }
        }
        else {
            ostream << "\\left(" << node_ptr << "\\right)";
        }
    }
    else if (node_ptr->Type() == "Exponentiation") {
        Compose(ostream, node_ptr->Argument(0), 0);
        
        ostream << "^"; 
        
        Compose(ostream, node_ptr->Argument(1), 0);
    }
    else if (node_ptr->Type() == "Multiplication") {
        if (precedence < 2) {
            ostream << "\\left("; 
            
            Compose(ostream, node_ptr->Argument(0), 2);
            
            ostream << "*"; 
            
            Compose(ostream, node_ptr->Argument(1), 2);
            
            ostream << "\\right)";
        }
        else {
            Compose(ostream, node_ptr->Argument(0), 2);
            
            ostream << "*";
            
            Compose(ostream, node_ptr->Argument(1), 2);
        }
    }
    else if (node_ptr->Type() == "Division") {
        if (precedence < 2) {
            ostream << "\\left(";
            
            Compose(ostream, node_ptr->Argument(0), 2);
            
            ostream << "/";
            
            Compose(ostream, node_ptr->Argument(1), 2);
            
            ostream << "\\right)";
        }
        else {
            Compose(ostream, node_ptr->Argument(0), 2);
            
            ostream << "/";
            
            Compose(ostream, node_ptr->Argument(1), 2);
        }
    }
    else if (node_ptr->Type() == "Addition") {
        if (precedence < 3) {
            ostream << "\\left(";
            
            Compose(ostream, node_ptr->Argument(0), 3);
            
            ostream << "+";
            
            Compose(ostream, node_ptr->Argument(1), 3);
            
            ostream << "\\right)";
        }
        else {
            Compose(ostream, node_ptr->Argument(0), 3);
            
            ostream << "+";
            
            Compose(ostream, node_ptr->Argument(1), 3);
        }
    }
    else if (node_ptr->Type() == "Subtraction") {
        if (precedence < 3) {
            ostream << "\\left(";
            
            Compose(ostream, node_ptr->Argument(0), 3);
            
            ostream << "-";
            
            Compose(ostream, node_ptr->Argument(1), 3);
            
            ostream << "\\right)";
        }
        else {
            Compose(ostream, node_ptr->Argument(0), 3);
            
            ostream << "-";
            
            Compose(ostream, node_ptr->Argument(1), 3);
        }
    }
    else if (node_ptr->Type() == "Sin") {
        ostream << "sin";
        
        Compose(ostream, node_ptr->Argument(0), 0);
    }
    else if (node_ptr->Type() == "Cos") {
        ostream << "cos";
        
        Compose(ostream, node_ptr->Argument(0), 0);
    }
    else if (node_ptr->Type() == "Tan") {
        ostream << "tan";
        
        Compose(ostream, node_ptr->Argument(0), 0);
    }
    else if (node_ptr->Type() == "Asin") {
        ostream << "asin";
        
        Compose(ostream, node_ptr->Argument(0), 0);
    }
    else if (node_ptr->Type() == "Acos") {
        ostream << "acos";
        
        Compose(ostream, node_ptr->Argument(0), 0);
    }
    else if (node_ptr->Type() == "Atan") {
        ostream << "atan";
        
        Compose(ostream, node_ptr->Argument(0), 0);
    }
    else if (node_ptr->Type() == "Sqrt") {
        ostream << "sqrt";
        
        Compose(ostream, node_ptr->Argument(0), 0);
    }
    else if (node_ptr->Type() == "Abs") {
        ostream << "abs";
        
        Compose(ostream, node_ptr->Argument(0), 0);
    }
    else if (node_ptr->Type() == "Exp") {
        ostream << "exp";
        
        Compose(ostream, node_ptr->Argument(0), 0);
    }
    else if (node_ptr->Type() == "Log") {
        ostream << "log";
        
        Compose(ostream, node_ptr->Argument(0), 0);
    }
    else if (node_ptr->Type() == "Determinant") {
        ostream << "det";
        
        Compose(ostream, node_ptr->Argument(0), 0);
    }
    else if (node_ptr->Type() == "Inverse") {
        ostream << "inv";
        
        Compose(ostream, node_ptr->Argument(0), 0);
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