/*
 * Copyright 2020 Casey Sanchez
 */

#pragma once

#include <memory>
#include <cmath>

#include "node.hpp"

class Affirmation : public Node
{
    std::shared_ptr<Node> node_ptr;

public:
    Affirmation(std::shared_ptr<Node> const &node_ptr);

    std::shared_ptr<Node> NodePtr() const;

    std::string Type() const override;

    double Value() const override;
};

class Negation : public Node
{
    std::shared_ptr<Node> node_ptr;

public:
    Negation(std::shared_ptr<Node> const &node_ptr);

    std::shared_ptr<Node> NodePtr() const;

    std::string Type() const override;

    double Value() const override;
};

class Exponentiation : public Node
{
    std::shared_ptr<Node> lhs_ptr;
    std::shared_ptr<Node> rhs_ptr;

public:
    Exponentiation(std::shared_ptr<Node> const &lhs_ptr, std::shared_ptr<Node> const &rhs_ptr);

    std::shared_ptr<Node> LhsPtr() const;
    std::shared_ptr<Node> RhsPtr() const;

    std::string Type() const override;

    double Value() const override;
};

class Multiplication : public Node
{
    std::shared_ptr<Node> lhs_ptr;
    std::shared_ptr<Node> rhs_ptr;

public:
    Multiplication(std::shared_ptr<Node> const &lhs_ptr, std::shared_ptr<Node> const &rhs_ptr);

    std::shared_ptr<Node> LhsPtr() const;
    std::shared_ptr<Node> RhsPtr() const;

    std::string Type() const override;

    double Value() const override;
};

class Division : public Node
{
    std::shared_ptr<Node> lhs_ptr;
    std::shared_ptr<Node> rhs_ptr;

public:
    Division(std::shared_ptr<Node> const &lhs_ptr, std::shared_ptr<Node> const &rhs_ptr);

    std::shared_ptr<Node> LhsPtr() const;
    std::shared_ptr<Node> RhsPtr() const;

    std::string Type() const override;

    double Value() const override;
};

class Addition : public Node
{
    std::shared_ptr<Node> lhs_ptr;
    std::shared_ptr<Node> rhs_ptr;

public:
    Addition(std::shared_ptr<Node> const &lhs_ptr, std::shared_ptr<Node> const &rhs_ptr);

    std::shared_ptr<Node> LhsPtr() const;
    std::shared_ptr<Node> RhsPtr() const;

    std::string Type() const override;

    double Value() const override;
};

class Subtraction : public Node
{
    std::shared_ptr<Node> lhs_ptr;
    std::shared_ptr<Node> rhs_ptr;

public:
    Subtraction(std::shared_ptr<Node> const &lhs_ptr, std::shared_ptr<Node> const &rhs_ptr);

    std::shared_ptr<Node> LhsPtr() const;
    std::shared_ptr<Node> RhsPtr() const;

    std::string Type() const override;

    double Value() const override;
};
