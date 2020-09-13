/*
 * Copyright 2020 Casey Sanchez
 */

#include "operation.hpp"

Affirmation::Affirmation(std::shared_ptr<Node> const &node_ptr) : node_ptr(node_ptr)
{
    type = "Affirmation";
}

std::shared_ptr<Node> Affirmation::NodePtr() const
{
    return node_ptr;
}

double Affirmation::Value() const
{
    return +1.0 * node_ptr->Value();
}

Negation::Negation(std::shared_ptr<Node> const &node_ptr) : node_ptr(node_ptr)
{
    type = "Negation";
}

std::shared_ptr<Node> Negation::NodePtr() const
{
    return node_ptr;
}

double Negation::Value() const
{
    return -1.0 * node_ptr->Value();
}

Exponentiation::Exponentiation(std::shared_ptr<Node> const &lhs_ptr, std::shared_ptr<Node> const &rhs_ptr) : lhs_ptr(lhs_ptr), rhs_ptr(rhs_ptr)
{
    type = "Exponentiation";
}

std::shared_ptr<Node> Exponentiation::LhsPtr() const
{
    return lhs_ptr;
}

std::shared_ptr<Node> Exponentiation::RhsPtr() const
{
    return rhs_ptr;
}

double Exponentiation::Value() const
{
    return std::pow(lhs_ptr->Value(), rhs_ptr->Value());
}

Multiplication::Multiplication(std::shared_ptr<Node> const &lhs_ptr, std::shared_ptr<Node> const &rhs_ptr) : lhs_ptr(lhs_ptr), rhs_ptr(rhs_ptr)
{
    type = "Multiplication";
}

std::shared_ptr<Node> Multiplication::LhsPtr() const
{
    return lhs_ptr;
}

std::shared_ptr<Node> Multiplication::RhsPtr() const
{
    return rhs_ptr;
}

double Multiplication::Value() const
{
    return lhs_ptr->Value() * rhs_ptr->Value();
}

Division::Division(std::shared_ptr<Node> const &lhs_ptr, std::shared_ptr<Node> const &rhs_ptr) : lhs_ptr(lhs_ptr), rhs_ptr(rhs_ptr)
{
    type = "Division";
}

std::shared_ptr<Node> Division::LhsPtr() const
{
    return lhs_ptr;
}

std::shared_ptr<Node> Division::RhsPtr() const
{
    return rhs_ptr;
}

double Division::Value() const
{
    return lhs_ptr->Value() / rhs_ptr->Value();
}

Addition::Addition(std::shared_ptr<Node> const &lhs_ptr, std::shared_ptr<Node> const &rhs_ptr) : lhs_ptr(lhs_ptr), rhs_ptr(rhs_ptr)
{
    type = "Addition";
}

std::shared_ptr<Node> Addition::LhsPtr() const
{
    return lhs_ptr;
}

std::shared_ptr<Node> Addition::RhsPtr() const
{
    return rhs_ptr;
}

double Addition::Value() const
{
    return lhs_ptr->Value() + rhs_ptr->Value();
}

Subtraction::Subtraction(std::shared_ptr<Node> const &lhs_ptr, std::shared_ptr<Node> const &rhs_ptr) : lhs_ptr(lhs_ptr), rhs_ptr(rhs_ptr)
{
    type = "Subtraction";
}

std::shared_ptr<Node> Subtraction::LhsPtr() const
{
    return lhs_ptr;
}

std::shared_ptr<Node> Subtraction::RhsPtr() const
{
    return rhs_ptr;
}

double Subtraction::Value() const
{
    return lhs_ptr->Value() - rhs_ptr->Value();
}
