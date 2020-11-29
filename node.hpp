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

#include "matrix.hpp"
#include "utils.hpp"

class Node : protected std::enable_shared_from_this<Node>
{
protected:
    std::variant<Matrix, std::complex<double>> m_value;
    std::vector<std::shared_ptr<Node>> m_arguments;

public:
    Node(std::variant<Matrix, std::complex<double>> const &value = 0.0);
    Node(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::shared_ptr<Node> &Argument(size_t const &index);
    std::vector<std::shared_ptr<Node>> &Arguments();

    virtual std::string Type() const;

    virtual std::variant<Matrix, std::complex<double>> Value() const;

public:
    friend std::ostream &operator<<(std::ostream &ostream, Node const &node);
    friend std::ostream &operator<<(std::ostream &ostream, std::shared_ptr<Node> const &node_ptr);
};

class Variable : public Node
{
public:
    Variable(std::complex<double> const &value = 0.0);

    std::string Type() const override;

    Node &operator=(std::complex<double> const &value);
};

class Constant : public Node
{
public:
    Constant(std::complex<double> const &value);

    std::string Type() const override;

    Node &operator=(Node const &node) = delete;
};
