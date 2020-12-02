/*
 * Copyright 2020 Casey Sanchez
 */

#pragma once

#include <memory>
#include <cmath>
#include <iostream>

#include "node.hpp"
#include "matrix.hpp"

class ExponentiationNode : public Node
{
    struct Visitor
    {
        std::variant<Matrix, std::complex<double>> operator()(std::complex<double> const &lhs, std::complex<double> const &rhs)
        {
            return std::pow(lhs, rhs);
        }

        std::variant<Matrix, std::complex<double>> operator()(Matrix const &lhs, Matrix const &rhs)
        {
            throw std::invalid_argument("Undefined operation: matrix ^ matrix");
        }

        std::variant<Matrix, std::complex<double>> operator()(std::complex<double> const &lhs, Matrix const &rhs)
        {
            throw std::invalid_argument("Undefined operation: complex ^ matrix");
        }

        std::variant<Matrix, std::complex<double>> operator()(Matrix const &lhs, std::complex<double> const &rhs)
        {
            return lhs ^ static_cast<size_t>(rhs.real());
        }
    };

public:
    ExponentiationNode(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class MultiplicationNode : public Node
{
    struct Visitor
    {
        std::variant<Matrix, std::complex<double>> operator()(std::complex<double> const &lhs, std::complex<double> const &rhs)
        {
            return lhs * rhs;
        }

        std::variant<Matrix, std::complex<double>> operator()(Matrix const &lhs, Matrix const &rhs)
        {
            return lhs * rhs;
        }

        std::variant<Matrix, std::complex<double>> operator()(std::complex<double> const &lhs, Matrix const &rhs)
        {
            return rhs * lhs;
        }

        std::variant<Matrix, std::complex<double>> operator()(Matrix const &lhs, std::complex<double> const &rhs)
        {
            return lhs * rhs;
        }
    };

public:
    MultiplicationNode(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class DivisionNode : public Node
{
    struct Visitor
    {
        std::variant<Matrix, std::complex<double>> operator()(std::complex<double> const &lhs, std::complex<double> const &rhs)
        {
            return lhs / rhs;
        }

        std::variant<Matrix, std::complex<double>> operator()(Matrix const &lhs, Matrix const &rhs)
        {
            throw std::invalid_argument("Undefined operation: matrix / matrix");
        }

        std::variant<Matrix, std::complex<double>> operator()(std::complex<double> const &lhs, Matrix const &rhs)
        {
            throw std::invalid_argument("Undefined operation: complex / matrix");
        }

        std::variant<Matrix, std::complex<double>> operator()(Matrix const &lhs, std::complex<double> const &rhs)
        {
            return lhs * (1.0 / rhs);
        }
    };

public:
    DivisionNode(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class AdditionNode : public Node
{
    struct Visitor
    {
        std::variant<Matrix, std::complex<double>> operator()(std::complex<double> const &lhs, std::complex<double> const &rhs)
        {
            return lhs + rhs;
        }

        std::variant<Matrix, std::complex<double>> operator()(Matrix const &lhs, Matrix const &rhs)
        {
            return lhs + rhs;
        }

        std::variant<Matrix, std::complex<double>> operator()(std::complex<double> const &lhs, Matrix const &rhs)
        {
            return rhs + lhs;
        }

        std::variant<Matrix, std::complex<double>> operator()(Matrix const &lhs, std::complex<double> const &rhs)
        {
            return lhs + rhs;
        }
    };

public:
    AdditionNode(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class SubtractionNode : public Node
{
    struct Visitor
    {
        std::variant<Matrix, std::complex<double>> operator()(std::complex<double> const &lhs, std::complex<double> const &rhs)
        {
            return lhs - rhs;
        }

        std::variant<Matrix, std::complex<double>> operator()(Matrix const &lhs, Matrix const &rhs)
        {
            return lhs - rhs;
        }

        std::variant<Matrix, std::complex<double>> operator()(std::complex<double> const &lhs, Matrix const &rhs)
        {
            throw std::invalid_argument("Undefined operation: complex - matrix");
        }

        std::variant<Matrix, std::complex<double>> operator()(Matrix const &lhs, std::complex<double> const &rhs)
        {
            return lhs - rhs;
        }
    };

public:
    SubtractionNode(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};
