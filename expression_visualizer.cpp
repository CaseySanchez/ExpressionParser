/*
 * Copyright 2020 Casey Sanchez
 */

#include "expression_visualizer.hpp"

ExpressionVisualizer::ExpressionVisualizer(std::shared_ptr<Node> const &node_ptr, std::map<std::string, std::shared_ptr<Node>> const &node_map) : m_node_ptr(node_ptr), m_node_map(node_map)
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
    Visualize(ostream, m_node_ptr, 0);
}

void ExpressionVisualizer::Visualize(std::ostream &ostream, std::shared_ptr<Node> const &node_ptr, size_t const &depth) const
{        
    ostream << std::string(depth * 4, ' ') << "[" << node_ptr->Type() << "] ";
    
    auto node_it = std::find_if(std::cbegin(m_node_map), std::cend(m_node_map), 
        [&node_ptr](std::pair<std::string, std::shared_ptr<Node>> const &node_pair) {  
            return node_pair.second == node_ptr;
        });

    if (node_it != std::cend(m_node_map)) {
        ostream << node_it->first << std::endl;
    }
    else {
        ostream << node_ptr << std::endl;
    }

    for (auto const &argument_ptr : node_ptr->Arguments()) {
        Visualize(ostream, argument_ptr, depth + 1);
    }
}

std::ostream &operator<<(std::ostream &ostream, ExpressionVisualizer const &expression_visualizer)
{
    expression_visualizer.Visualize(ostream);

    return ostream;
}
