/*
 * Copyright 2020 Casey Sanchez
 */

#include "node.hpp"

Node::Node(std::variant<Matrix, std::complex<double>> const &value) : m_value(value)
{
}

Node::Node(std::initializer_list<std::shared_ptr<Node>> const &arguments) : m_arguments(arguments)
{
}

std::shared_ptr<Node> Node::Argument(size_t const &index) const
{
    return m_arguments.at(index);
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
        std::complex<double> complex = std::get<std::complex<double>>(value);

        if (!Approximately(complex.real(), 0.0) && !Approximately(complex.imag(), 0.0)) {
            ostream << std::to_string(complex.real()) << (complex.imag() > 0.0 ? "+" : "") << std::to_string(complex.imag()) << "i";
        }
        else if(!Approximately(complex.real(), 0.0)) {
            ostream << std::to_string(complex.real());
        }
        else if(!Approximately(complex.imag(), 0.0)) {
            ostream << std::to_string(complex.imag()) << "i";
        }
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