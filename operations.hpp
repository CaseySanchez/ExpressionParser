/*
 * Copyright 2020 Casey Sanchez
 */

#pragma once

#include <memory>
#include <cmath>
#include <iostream>

#include "node.hpp"
#include "matrix.hpp"

class Affirmation : public Node
{
    std::shared_ptr<Node> m_node_ptr;

    struct Visitor
    {
        std::variant<Matrix, std::complex<double>> operator()(std::complex<double> const &value)
        {
            return value * +1.0;
        }

        std::variant<Matrix, std::complex<double>> operator()(Matrix const &value)
        {
            return value * +1.0;
        }
    };

public:
    Affirmation(std::shared_ptr<Node> const &node_ptr);

    std::shared_ptr<Node> NodePtr() const;

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class Negation : public Node
{
    std::shared_ptr<Node> m_node_ptr;

    struct Visitor
    {
        std::variant<Matrix, std::complex<double>> operator()(std::complex<double> const &value)
        {
            return value * -1.0;
        }

        std::variant<Matrix, std::complex<double>> operator()(Matrix const &value)
        {
            return value * -1.0;
        }
    };

public:
    Negation(std::shared_ptr<Node> const &node_ptr);

    std::shared_ptr<Node> NodePtr() const;

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class Exponentiation : public Node
{
    std::shared_ptr<Node> m_lhs_ptr;
    std::shared_ptr<Node> m_rhs_ptr;

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
            throw std::invalid_argument("Undefined operation: matrix ^ complex");
        }
    };

public:
    Exponentiation(std::shared_ptr<Node> const &lhs_ptr, std::shared_ptr<Node> const &rhs_ptr);

    std::shared_ptr<Node> LhsPtr() const;
    std::shared_ptr<Node> RhsPtr() const;

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class Multiplication : public Node
{
    std::shared_ptr<Node> m_lhs_ptr;
    std::shared_ptr<Node> m_rhs_ptr;

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
    Multiplication(std::shared_ptr<Node> const &lhs_ptr, std::shared_ptr<Node> const &rhs_ptr);

    std::shared_ptr<Node> LhsPtr() const;
    std::shared_ptr<Node> RhsPtr() const;

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class Division : public Node
{
    std::shared_ptr<Node> m_lhs_ptr;
    std::shared_ptr<Node> m_rhs_ptr;

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
    Division(std::shared_ptr<Node> const &lhs_ptr, std::shared_ptr<Node> const &rhs_ptr);

    std::shared_ptr<Node> LhsPtr() const;
    std::shared_ptr<Node> RhsPtr() const;

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class Addition : public Node
{
    std::shared_ptr<Node> m_lhs_ptr;
    std::shared_ptr<Node> m_rhs_ptr;

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
    Addition(std::shared_ptr<Node> const &lhs_ptr, std::shared_ptr<Node> const &rhs_ptr);

    std::shared_ptr<Node> LhsPtr() const;
    std::shared_ptr<Node> RhsPtr() const;

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class Subtraction : public Node
{
    std::shared_ptr<Node> m_lhs_ptr;
    std::shared_ptr<Node> m_rhs_ptr;

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
    Subtraction(std::shared_ptr<Node> const &lhs_ptr, std::shared_ptr<Node> const &rhs_ptr);

    std::shared_ptr<Node> LhsPtr() const;
    std::shared_ptr<Node> RhsPtr() const;

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};
