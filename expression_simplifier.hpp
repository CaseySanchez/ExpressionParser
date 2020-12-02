/*
 * Copyright 2020 Casey Sanchez
 */

#pragma once

#include <memory>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>

#include "node.hpp"
#include "operations.hpp"
#include "functions.hpp"
#include "utils.hpp"
#include "expression_visualizer.hpp"

class ExpressionSimplifier
{
    std::shared_ptr<Node> m_node_ptr;
    std::map<std::string, std::shared_ptr<Node>> m_node_map;

public:
    ExpressionSimplifier(std::shared_ptr<Node> const &node_ptr, std::map<std::string, std::shared_ptr<Node>> const &node_map);

    std::shared_ptr<Node> Identify();
    std::shared_ptr<Node> Distribute();
    std::shared_ptr<Node> CombineFactors();
    std::shared_ptr<Node> CombineAddends();

private:
    std::shared_ptr<Node> Identify(std::shared_ptr<Node> const &node_ptr);
    std::shared_ptr<Node> Distribute(std::shared_ptr<Node> const &node_ptr);
    std::shared_ptr<Node> CombineFactors(std::shared_ptr<Node> const &node_ptr);
    std::shared_ptr<Node> CombineAddends(std::shared_ptr<Node> const &node_ptr);

    std::vector<std::shared_ptr<Node>> Factors(std::shared_ptr<Node> const &node_ptr);    
    void Factors(std::vector<std::shared_ptr<Node>> &factors, std::shared_ptr<Node> const &node_ptr);
    std::vector<std::shared_ptr<Node>> Addends(std::shared_ptr<Node> const &node_ptr);
    void Addends(std::vector<std::shared_ptr<Node>> &addends, std::shared_ptr<Node> const &node_ptr);
};