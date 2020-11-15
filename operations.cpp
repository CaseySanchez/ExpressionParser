/*
 * Copyright 2020 Casey Sanchez
 */

#include "operations.hpp"

Affirmation::Affirmation(std::shared_ptr<Node> const &node_ptr) : m_node_ptr(node_ptr)
{
}

std::shared_ptr<Node> Affirmation::NodePtr() const
{
    return m_node_ptr;
}

std::string Affirmation::Type() const
{
    return "Affirmation";
}

std::variant<Matrix, std::complex<double>> Affirmation::Value() const
{
    return std::visit(Visitor{}, m_node_ptr->Value());
}

Negation::Negation(std::shared_ptr<Node> const &node_ptr) : m_node_ptr(node_ptr)
{
}

std::shared_ptr<Node> Negation::NodePtr() const
{
    return m_node_ptr;
}

std::string Negation::Type() const
{
    return "Negation";
}

std::variant<Matrix, std::complex<double>> Negation::Value() const
{
    return std::visit(Visitor{}, m_node_ptr->Value());
}

Exponentiation::Exponentiation(std::shared_ptr<Node> const &lhs_ptr, std::shared_ptr<Node> const &rhs_ptr) : m_lhs_ptr(lhs_ptr), m_rhs_ptr(rhs_ptr)
{
}

std::shared_ptr<Node> Exponentiation::LhsPtr() const
{
    return m_lhs_ptr;
}

std::shared_ptr<Node> Exponentiation::RhsPtr() const
{
    return m_rhs_ptr;
}

std::string Exponentiation::Type() const
{
    return "Exponentiation";
}

std::variant<Matrix, std::complex<double>> Exponentiation::Value() const
{
    return std::visit(Visitor{}, m_lhs_ptr->Value(), m_rhs_ptr->Value());
}

Multiplication::Multiplication(std::shared_ptr<Node> const &lhs_ptr, std::shared_ptr<Node> const &rhs_ptr) : m_lhs_ptr(lhs_ptr), m_rhs_ptr(rhs_ptr)
{
}

std::shared_ptr<Node> Multiplication::LhsPtr() const
{
    return m_lhs_ptr;
}

std::shared_ptr<Node> Multiplication::RhsPtr() const
{
    return m_rhs_ptr;
}

std::string Multiplication::Type() const
{
    return "Multiplication";
}

std::variant<Matrix, std::complex<double>> Multiplication::Value() const
{
    return std::visit(Visitor{}, m_lhs_ptr->Value(), m_rhs_ptr->Value());
}

Division::Division(std::shared_ptr<Node> const &lhs_ptr, std::shared_ptr<Node> const &rhs_ptr) : m_lhs_ptr(lhs_ptr), m_rhs_ptr(rhs_ptr)
{
}

std::shared_ptr<Node> Division::LhsPtr() const
{
    return m_lhs_ptr;
}

std::shared_ptr<Node> Division::RhsPtr() const
{
    return m_rhs_ptr;
}

std::string Division::Type() const
{
    return "Division";
}

std::variant<Matrix, std::complex<double>> Division::Value() const
{
    return std::visit(Visitor{}, m_lhs_ptr->Value(), m_rhs_ptr->Value());
}

Addition::Addition(std::shared_ptr<Node> const &lhs_ptr, std::shared_ptr<Node> const &rhs_ptr) : m_lhs_ptr(lhs_ptr), m_rhs_ptr(rhs_ptr)
{
}

std::shared_ptr<Node> Addition::LhsPtr() const
{
    return m_lhs_ptr;
}

std::shared_ptr<Node> Addition::RhsPtr() const
{
    return m_rhs_ptr;
}

std::string Addition::Type() const
{
    return "Addition";
}

std::variant<Matrix, std::complex<double>> Addition::Value() const
{
    return std::visit(Visitor{}, m_lhs_ptr->Value(), m_rhs_ptr->Value());
}

Subtraction::Subtraction(std::shared_ptr<Node> const &lhs_ptr, std::shared_ptr<Node> const &rhs_ptr) : m_lhs_ptr(lhs_ptr), m_rhs_ptr(rhs_ptr)
{
}

std::shared_ptr<Node> Subtraction::LhsPtr() const
{
    return m_lhs_ptr;
}

std::shared_ptr<Node> Subtraction::RhsPtr() const
{
    return m_rhs_ptr;
}

std::string Subtraction::Type() const
{
    return "Subtraction";
}

std::variant<Matrix, std::complex<double>> Subtraction::Value() const
{
    return std::visit(Visitor{}, m_lhs_ptr->Value(), m_rhs_ptr->Value());
}
