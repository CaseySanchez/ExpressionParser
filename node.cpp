/*
 * Copyright 2020 Casey Sanchez
 */

#include "node.hpp"

Node::Node(double const &value) : value(value)
{
}

std::string Node::Type() const
{
    return "Node";
}

double Node::Value() const
{
    return value;
}

Variable::Variable(double const &value) : Node(value)
{
}

std::string Variable::Type() const
{
    return "Variable";
}

std::string Variable::Name() const
{
    return name;
}

Node &Variable::operator=(double const &value)
{
    this->value = value;

    return *this;
}

Constant::Constant(double const &value) : Node(value)
{
}

std::string Constant::Type() const
{
    return "Constant";
}