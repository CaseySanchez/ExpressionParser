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

std::shared_ptr<Node> &Node::Argument(size_t const &index)
{
    return m_arguments.at(index);
}

std::shared_ptr<Node> Node::Argument(size_t const &index) const
{
    return m_arguments.at(index);
}

std::vector<std::shared_ptr<Node>> &Node::Arguments()
{
    return m_arguments;
}

std::vector<std::shared_ptr<Node>> Node::Arguments() const
{
    return m_arguments;
}

std::string Node::Type() const
{
    return "Node";
}

std::variant<Matrix, std::complex<double>> Node::Value() const
{
    return m_value;
}

std::complex<double> Node::ComplexValue() const
{
    return std::get<std::complex<double>>(Value());
}

Matrix Node::MatrixValue() const
{
    return std::get<Matrix>(Value());
}

bool Node::Equivalent(std::shared_ptr<Node> const &lhs_ptr, std::shared_ptr<Node> const &rhs_ptr)
{ 
    if (lhs_ptr->Type() == rhs_ptr->Type()) {
        std::vector<std::shared_ptr<Node>> lhs_args = lhs_ptr->Arguments();
        std::vector<std::shared_ptr<Node>> rhs_args = rhs_ptr->Arguments();

        if (lhs_args.size() == 0) {
            if (lhs_ptr->Type() == "VariableNode") {
                return lhs_ptr == rhs_ptr;
            }
            else if (lhs_ptr->Type() == "ConstantNode") {
                return Approximately(lhs_ptr->ComplexValue(), rhs_ptr->ComplexValue());
            }
        }
        else {
            for (auto lhs_arg : lhs_args) {
                if (std::find_if(std::cbegin(rhs_args), std::cend(rhs_args), [&lhs_arg](std::shared_ptr<Node> const &rhs_arg) -> bool { return Node::Equivalent(lhs_arg, rhs_arg); }) == std::cend(rhs_args)) {
                    return false;
                }
            }

            return true;
        }
    }
    
    return false;
}

std::ostream &operator<<(std::ostream &ostream, Node const &node)
{
    auto value = node.Value();

    if (std::holds_alternative<std::complex<double>>(value)) {
        std::complex<double> complex = std::get<std::complex<double>>(value);

        if (!Approximately(complex.real(), 0.0) && !Approximately(complex.imag(), 0.0)) {
            ostream << complex.real() << (complex.imag() > 0.0 ? "+" : "") << complex.imag() << "i";
        }
        else if(!Approximately(complex.real(), 0.0)) {
            ostream << complex.real();
        }
        else if(!Approximately(complex.imag(), 0.0)) {
            ostream << complex.imag() << "i";
        }
        else {
            ostream << 0.0;
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

VariableNode::VariableNode(std::complex<double> const &value) : Node(value)
{
}

std::string VariableNode::Type() const
{
    return "VariableNode";
}

Node &VariableNode::operator=(std::complex<double> const &value)
{
    m_value = value;

    return *this;
}

ConstantNode::ConstantNode(std::complex<double> const &value) : Node(value)
{
}

std::string ConstantNode::Type() const
{
    return "ConstantNode";
}

MatrixNode::MatrixNode(Matrix const &value) : Node(value)
{
}

std::string MatrixNode::Type() const
{
    return "MatrixNode";
}