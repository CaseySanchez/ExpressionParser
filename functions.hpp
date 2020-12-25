/*
 * Copyright 2020 Casey Sanchez
 */

#pragma once

#include <memory>
#include <cmath>

#include "node.hpp"
#include "matrix.hpp"

class CosNode : public Node
{
public:
    CosNode(std::initializer_list<Scalar> const &arguments);

    std::string Type() const override;

    std::complex<double> Value() const override;
};

class SinNode : public Node
{
public:
    SinNode(std::initializer_list<Scalar> const &arguments);

    std::string Type() const override;

    std::complex<double> Value() const override;
};

class TanNode : public Node
{
public:
    TanNode(std::initializer_list<Scalar> const &arguments);

    std::string Type() const override;

    std::complex<double> Value() const override;
};

class AcosNode : public Node
{
public:
    AcosNode(std::initializer_list<Scalar> const &arguments);

    std::string Type() const override;

    std::complex<double> Value() const override;
};

class AsinNode : public Node
{
public:
    AsinNode(std::initializer_list<Scalar> const &arguments);

    std::string Type() const override;

    std::complex<double> Value() const override;
};

class AtanNode : public Node
{
public:
    AtanNode(std::initializer_list<Scalar> const &arguments);

    std::string Type() const override;

    std::complex<double> Value() const override;
};

class SqrtNode : public Node
{
public:
    SqrtNode(std::initializer_list<Scalar> const &arguments);

    std::string Type() const override;

    std::complex<double> Value() const override;
};

class AbsNode : public Node
{
public:
    AbsNode(std::initializer_list<Scalar> const &arguments);

    std::string Type() const override;

    std::complex<double> Value() const override;
};

class ExpNode : public Node
{
public:
    ExpNode(std::initializer_list<Scalar> const &arguments);

    std::string Type() const override;

    std::complex<double> Value() const override;
};

class LnNode : public Node
{
public:
    LnNode(std::initializer_list<Scalar> const &arguments);

    std::string Type() const override;

    std::complex<double> Value() const override;
};
