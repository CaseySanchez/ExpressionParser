/*
 * Copyright 2020 Casey Sanchez
 */

#pragma once

#include <memory>
#include <string>
#include <map>

#include "node.hpp"
#include "operations.hpp"
#include "functions.hpp"
#include "utils.hpp"

class ExpressionSimplifier
{
    std::shared_ptr<Node> m_node_ptr;

public:
    ExpressionSimplifier(std::shared_ptr<Node> const &node_ptr);

    std::shared_ptr<Node> Simplify();

    std::shared_ptr<Node> Distribute(std::shared_ptr<Node> const &node_ptr);

    std::vector<std::shared_ptr<Node>> Factors(std::shared_ptr<Node> const &node_ptr);
    std::vector<std::shared_ptr<Node>> Summands(std::shared_ptr<Node> const &node_ptr);

private:
    std::shared_ptr<Node> Simplify(std::shared_ptr<Node> const &node_ptr);

    std::shared_ptr<Node> Distribute(std::vector<std::shared_ptr<Node>> const &lhs_summands, std::vector<std::shared_ptr<Node>> const &rhs_summands);

    void Factors(std::vector<std::shared_ptr<Node>> &factors, std::shared_ptr<Node> const &node_ptr);
    void Summands(std::vector<std::shared_ptr<Node>> &summands, std::shared_ptr<Node> const &node_ptr);
};