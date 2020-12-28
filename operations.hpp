/*
 * Copyright 2020 Casey Sanchez
 */

#pragma once

#include <memory>
#include <cmath>
#include <iostream>

#include "node.hpp"
#include "matrix.hpp"

class AdditionNode : public Node
{
public:
    AdditionNode(std::initializer_list<Scalar> const &arguments);

    std::string Type() const override;

    std::complex<double> Value() const override;
};

class SubtractionNode : public Node
{
public:
    SubtractionNode(std::initializer_list<Scalar> const &arguments);

    std::string Type() const override;

    std::complex<double> Value() const override;
};

class MultiplicationNode : public Node
{
public:
    MultiplicationNode(std::initializer_list<Scalar> const &arguments);

    std::string Type() const override;

    std::complex<double> Value() const override;
};

class DivisionNode : public Node
{
public:
    DivisionNode(std::initializer_list<Scalar> const &arguments);

    std::string Type() const override;

    std::complex<double> Value() const override;
};

class ExponentiationNode : public Node
{
public:
    ExponentiationNode(std::initializer_list<Scalar> const &arguments);

    std::string Type() const override;

    std::complex<double> Value() const override;
};
