/*
 * Copyright 2020 Casey Sanchez
 */

#pragma once

#include <memory>
#include <string>
#include <map>
#include <algorithm>

#include "node.hpp"
#include "operations.hpp"
#include "functions.hpp"
#include "utils.hpp"

class ExpressionComposer
{
    std::variant<Scalar, Matrix> m_node_variant;

    std::map<std::string, std::variant<Scalar, Matrix>> m_node_map;

public:
    ExpressionComposer(std::variant<Scalar, Matrix> const &node_variant, std::map<std::string, std::variant<Scalar, Matrix>> const &node_map = { });

    std::string Compose() const;

    void Compose(std::ostream &ostream) const;

private:
    void Compose(std::ostream &ostream, std::variant<Scalar, Matrix> const &node_variant, uint32_t const &precedence = ~0) const;

public:
    friend std::ostream &operator<<(std::ostream &ostream, ExpressionComposer const &expression_composer);
};