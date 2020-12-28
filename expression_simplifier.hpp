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

#include "matrix.hpp"
#include "operations.hpp"
#include "functions.hpp"
#include "utils.hpp"

class ExpressionSimplifier
{
    std::variant<Scalar, Matrix> m_node_variant;
    std::map<std::string, std::variant<Scalar, Matrix>> m_node_map;

public:
    ExpressionSimplifier(std::variant<Scalar, Matrix> const &node_variant, std::map<std::string, std::variant<Scalar, Matrix>> const &node_map = { });

    std::variant<Scalar, Matrix> Simplify();
    std::variant<Scalar, Matrix> Identify();
    std::variant<Scalar, Matrix> Distribute();
    std::variant<Scalar, Matrix> CombineFactors();
    std::variant<Scalar, Matrix> CombineAddends();
    std::variant<Scalar, Matrix> Factorize();

private:
    std::variant<Scalar, Matrix> Identify(std::variant<Scalar, Matrix> const &node_variant);
    std::variant<Scalar, Matrix> Distribute(std::variant<Scalar, Matrix> const &node_variant);
    std::variant<Scalar, Matrix> CombineFactors(std::variant<Scalar, Matrix> const &node_variant);
    std::variant<Scalar, Matrix> CombineAddends(std::variant<Scalar, Matrix> const &node_variant);
    std::variant<Scalar, Matrix> Factorize(std::variant<Scalar, Matrix> const &node_variant);

    static std::vector<Scalar> Factors(Scalar const &node_scalar);    
    static std::vector<Scalar> Addends(Scalar const &node_scalar);

    static void Factors(std::vector<Scalar> &factors, Scalar const &node_scalar);
    static void Addends(std::vector<Scalar> &addends, Scalar const &node_scalar);
};