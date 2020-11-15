/*
 * Copyright 2020 Casey Sanchez
 */

#include "node.hpp"

Node::Node(std::variant<Matrix, std::complex<double>> const &value) : m_value(value)
{
}

std::string Node::Type() const
{
    return "Node";
}

std::variant<Matrix, std::complex<double>> Node::Value() const
{
    return m_value;
}

Variable::Variable(std::complex<double> const &value) : Node(value)
{
}

std::string Variable::Type() const
{
    return "Variable";
}

Node &Variable::operator=(std::complex<double> const &value)
{
    m_value = value;

    return *this;
}

Constant::Constant(std::complex<double> const &value) : Node(value)
{
}

std::string Constant::Type() const
{
    return "Constant";
}