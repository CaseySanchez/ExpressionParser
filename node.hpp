/*
 * Copyright 2020 Casey Sanchez
 */

#pragma once

#include <string>
#include <complex>
#include <memory>
#include <vector>
#include <variant>

#include "matrix.hpp"

class Node
{
protected:
    std::variant<Matrix, std::complex<double>> m_value;

public:
    Node(std::variant<Matrix, std::complex<double>> const &value = 0.0);

    virtual std::string Type() const;

    virtual std::variant<Matrix, std::complex<double>> Value() const;
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
