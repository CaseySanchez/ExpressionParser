/*
 * Copyright 2020 Casey Sanchez
 */

#pragma once

#include <iostream>
#include <string>
#include <complex>
#include <memory>
#include <vector>
#include <map>
#include <variant>
#include <algorithm>
#include <numeric>

#include "utils.hpp"

class Node;

using Scalar = std::shared_ptr<Node>;

class Node
{    
protected:
    std::complex<double> m_value;
    std::vector<Scalar> m_arguments;

public:
    Node(std::complex<double> const &value = 0.0);
    Node(std::initializer_list<Scalar> const &arguments);

    Scalar &Argument(size_t const &index);
    Scalar Argument(size_t const &index) const;
    std::vector<Scalar> &Arguments();
    std::vector<Scalar> Arguments() const;

    virtual std::string Type() const;

    virtual std::complex<double> Value() const;

public:
    static bool Equivalent(Scalar const &lhs_ptr, Scalar const &rhs_ptr);

    friend std::ostream &operator<<(std::ostream &ostream, Node const &node);
    friend std::ostream &operator<<(std::ostream &ostream, Scalar const &scalar);
};

class VariableNode : public Node
{
public:
    VariableNode(std::complex<double> const &value = 0.0);

    std::string Type() const override;

    Node &operator=(std::complex<double> const &value);
};

class ConstantNode : public Node
{
public:
    ConstantNode(std::complex<double> const &value);

    std::string Type() const override;

    Node &operator=(Node const &node) = delete;
};
