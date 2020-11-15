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

std::ostream &operator<<(std::ostream &ostream, Node const &node)
{
    auto value = node.Value();

    if (std::holds_alternative<std::complex<double>>(value)) {
        ostream << "\\left(" << std::get<std::complex<double>>(value).real() << "+" << std::get<std::complex<double>>(value).imag() << "i\\right)";
    }
    else if (std::holds_alternative<Matrix>(value)) {
        ostream << std::get<Matrix>(value);
    }

    return ostream;
}

std::ostream &operator<<(std::ostream &ostream, std::shared_ptr<Node> const &node_ptr)
{
    ostream << *node_ptr;

    return ostream;
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