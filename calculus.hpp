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

class Calculus
{
    std::shared_ptr<Node> m_node_ptr;
    std::map<std::string, std::shared_ptr<Node>> m_node_map;
    
public: 
    Calculus(std::shared_ptr<Node> const &node_ptr, std::map<std::string, std::shared_ptr<Node>> const &node_map);

    std::shared_ptr<Node> Partial(std::shared_ptr<Node> const &with_respect_to_ptr);
    std::shared_ptr<Node> Partial(std::shared_ptr<Node> const &node_ptr, std::shared_ptr<Node> const &with_respect_to_ptr);

    std::shared_ptr<Node> Gradient(std::shared_ptr<Node> const &node_ptr, std::shared_ptr<Node> const &with_respect_to_x_ptr, std::shared_ptr<Node> const &with_respect_to_y_ptr, std::shared_ptr<Node> const &with_respect_to_z_ptr);
    std::shared_ptr<Node> Divergence(std::shared_ptr<Node> const &node_ptr, std::shared_ptr<Node> const &with_respect_to_x_ptr, std::shared_ptr<Node> const &with_respect_to_y_ptr, std::shared_ptr<Node> const &with_respect_to_z_ptr);
    std::shared_ptr<Node> Curl(std::shared_ptr<Node> const &node_ptr, std::shared_ptr<Node> const &with_respect_to_x_ptr, std::shared_ptr<Node> const &with_respect_to_y_ptr, std::shared_ptr<Node> const &with_respect_to_z_ptr);
};
