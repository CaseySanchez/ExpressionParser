/*
 * Copyright 2020 Casey Sanchez
 */

#pragma once

#include "node.hpp"

#include "matrix.hpp"

class ExpressionVisualizer
{
    std::variant<Scalar, Matrix> m_node_variant;
    std::map<std::string, std::variant<Scalar, Matrix>> m_node_map;

public:
    ExpressionVisualizer(std::variant<Scalar, Matrix> const &node_variant, std::map<std::string, std::variant<Scalar, Matrix>> const &node_map = { });

    std::string Visualize() const;
    
    void Visualize(std::ostream &ostream) const;

private:
    void Visualize(std::ostream &ostream, std::variant<Scalar, Matrix> const &node_variant, size_t const &depth) const;

public:
    friend std::ostream &operator<<(std::ostream &ostream, ExpressionVisualizer const &expression_visualizer);
};