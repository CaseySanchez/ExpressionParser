/*
 * Copyright 2020 Casey Sanchez
 */

#include "node.hpp"

Node::Node(double const &value) : value(value)
{
    type = "Node";
}

std::string Node::Type() const
{
    return type;
}

double Node::Value() const
{
    return value;
}

Variable::Variable(double const &value) : Node(value)
{
    type = "Variable";
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
    type = "Constant";
}
