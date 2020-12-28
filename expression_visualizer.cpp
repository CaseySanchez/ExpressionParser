/*
 * Copyright 2020 Casey Sanchez
 */

#include "expression_visualizer.hpp"

ExpressionVisualizer::ExpressionVisualizer(std::variant<Scalar, Matrix> const &node_variant, std::map<std::string, std::variant<Scalar, Matrix>> const &node_map) : m_node_variant(node_variant), m_node_map(node_map)
{
}

std::string ExpressionVisualizer::Visualize() const
{
    std::ostringstream ostringstream;

    Visualize(ostringstream);

    return ostringstream.str();
}

void ExpressionVisualizer::Visualize(std::ostream &ostream) const
{
    Visualize(ostream, m_node_variant, 0);
}

void ExpressionVisualizer::Visualize(std::ostream &ostream, std::variant<Scalar, Matrix> const &node_variant, size_t const &depth) const
{
    ostream << std::string(depth * 4, ' ');
    
    if (std::holds_alternative<Matrix>(node_variant)) {
        Matrix matrix = std::get<Matrix>(node_variant);
        
        ostream << "[Matrix] ";
        ostream << matrix << std::endl;

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                Visualize(ostream, matrix(i, j), depth + 1);
            }
        }
    }
    else if (std::holds_alternative<Scalar>(node_variant)) {
        Scalar scalar = std::get<Scalar>(node_variant);

        ostream << "[" << scalar->Type() << "] ";
        
        auto node_it = std::find_if(std::cbegin(m_node_map), std::cend(m_node_map), 
            [&scalar](std::pair<std::string, std::variant<Scalar, Matrix>> const &node_pair) {  
                if (std::holds_alternative<Scalar>(node_pair.second)) {
                    return std::get<Scalar>(node_pair.second) == scalar;
                }

                return false;
            });

        if (node_it != std::cend(m_node_map)) {
            ostream << node_it->first << std::endl;
        }
        else {
            ostream << scalar << std::endl;
        }

        for (auto const &argument_ptr : scalar->Arguments()) {
            Visualize(ostream, argument_ptr, depth + 1);
        }
    }
}

std::ostream &operator<<(std::ostream &ostream, ExpressionVisualizer const &expression_visualizer)
{
    expression_visualizer.Visualize(ostream);

    return ostream;
}
