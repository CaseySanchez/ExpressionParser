/*
 * Copyright 2020 Casey Sanchez
 */

#include "operation.hpp"

Affirmation::Affirmation(std::shared_ptr<Node> const &node_ptr) : node_ptr(node_ptr)
{
}

std::shared_ptr<Node> Affirmation::NodePtr() const
{
    return node_ptr;
}

std::string Affirmation::Type() const
{
    return "Affirmation";
}

double Affirmation::Value() const
{
    return +1.0 * node_ptr->Value();
}

Negation::Negation(std::shared_ptr<Node> const &node_ptr) : node_ptr(node_ptr)
{
}

std::shared_ptr<Node> Negation::NodePtr() const
{
    return node_ptr;
}

std::string Negation::Type() const
{
    return "Negation";
}

double Negation::Value() const
{
    return -1.0 * node_ptr->Value();
}

Exponentiation::Exponentiation(std::shared_ptr<Node> const &lhs_ptr, std::shared_ptr<Node> const &rhs_ptr) : lhs_ptr(lhs_ptr), rhs_ptr(rhs_ptr)
{
}

std::shared_ptr<Node> Exponentiation::LhsPtr() const
{
    return lhs_ptr;
}

std::shared_ptr<Node> Exponentiation::RhsPtr() const
{
    return rhs_ptr;
}

std::string Exponentiation::Type() const
{
    return "Exponentiation";
}

double Exponentiation::Value() const
{
    return std::pow(lhs_ptr->Value(), rhs_ptr->Value());
}

Multiplication::Multiplication(std::shared_ptr<Node> const &lhs_ptr, std::shared_ptr<Node> const &rhs_ptr) : lhs_ptr(lhs_ptr), rhs_ptr(rhs_ptr)
{
}

std::shared_ptr<Node> Multiplication::LhsPtr() const
{
    return lhs_ptr;
}

std::shared_ptr<Node> Multiplication::RhsPtr() const
{
    return rhs_ptr;
}

std::string Multiplication::Type() const
{
    return "Multiplication";
}

double Multiplication::Value() const
{
    return lhs_ptr->Value() * rhs_ptr->Value();
}

Division::Division(std::shared_ptr<Node> const &lhs_ptr, std::shared_ptr<Node> const &rhs_ptr) : lhs_ptr(lhs_ptr), rhs_ptr(rhs_ptr)
{
}

std::shared_ptr<Node> Division::LhsPtr() const
{
    return lhs_ptr;
}

std::shared_ptr<Node> Division::RhsPtr() const
{
    return rhs_ptr;
}

std::string Division::Type() const
{
    return "Division";
}

double Division::Value() const
{
    return lhs_ptr->Value() / rhs_ptr->Value();
}

Addition::Addition(std::shared_ptr<Node> const &lhs_ptr, std::shared_ptr<Node> const &rhs_ptr) : lhs_ptr(lhs_ptr), rhs_ptr(rhs_ptr)
{
}

std::shared_ptr<Node> Addition::LhsPtr() const
{
    return lhs_ptr;
}

std::shared_ptr<Node> Addition::RhsPtr() const
{
    return rhs_ptr;
}

std::string Addition::Type() const
{
    return "Addition";
}

double Addition::Value() const
{
    return lhs_ptr->Value() + rhs_ptr->Value();
}

Subtraction::Subtraction(std::shared_ptr<Node> const &lhs_ptr, std::shared_ptr<Node> const &rhs_ptr) : lhs_ptr(lhs_ptr), rhs_ptr(rhs_ptr)
{
}

std::shared_ptr<Node> Subtraction::LhsPtr() const
{
    return lhs_ptr;
}

std::shared_ptr<Node> Subtraction::RhsPtr() const
{
    return rhs_ptr;
}

std::string Subtraction::Type() const
{
    return "Subtraction";
}

double Subtraction::Value() const
{
    return lhs_ptr->Value() - rhs_ptr->Value();
}
