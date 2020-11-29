/*
 * Copyright 2020 Casey Sanchez
 */

#pragma once

#include "node.hpp"

class ExpressionVisualizer
{
    std::shared_ptr<Node> m_node_ptr;
    std::map<std::string, std::shared_ptr<Node>> m_node_map;

public:
    ExpressionVisualizer(std::shared_ptr<Node> const &node_ptr, std::map<std::string, std::shared_ptr<Node>> const &node_map);

    void Visualize(std::ostream &ostream) const;

private:
    void Visualize(std::ostream &ostream, std::shared_ptr<Node> const &node_ptr, size_t const &depth) const;

public:
    friend std::ostream &operator<<(std::ostream &ostream, ExpressionVisualizer const &expression_visualizer);
};