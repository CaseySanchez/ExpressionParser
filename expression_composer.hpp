/*
 * Copyright 2020 Casey Sanchez
 */

#pragma once

#include <memory>
#include <string>
#include <map>
#include <algorithm>

#include "node.hpp"
#include "operation.hpp"
#include "function.hpp"

class ExpressionComposer
{
    std::shared_ptr<Node> node_ptr;

    std::map<std::string, std::shared_ptr<Node>> node_map;

public:
    ExpressionComposer(std::shared_ptr<Node> const &node_ptr, std::map<std::string, std::shared_ptr<Node>> const &node_map = { });

    std::string Compose() const;

private:
    std::string Compose(std::shared_ptr<Node> const &node_ptr, uint32_t precedence = ~0) const;
};