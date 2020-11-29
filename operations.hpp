/*
 * Copyright 2020 Casey Sanchez
 */

#pragma once

#include <memory>
#include <cmath>
#include <iostream>

#include "node.hpp"
#include "matrix.hpp"

class Exponentiation : public Node
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
    Exponentiation(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class Multiplication : public Node
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
    Multiplication(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class Division : public Node
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
    Division(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class Addition : public Node
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
    Addition(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class Subtraction : public Node
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
    Subtraction(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};
