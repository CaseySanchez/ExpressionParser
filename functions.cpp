/*
 * Copyright 2020 Casey Sanchez
 */

#include "functions.hpp"

Cos::Cos(std::shared_ptr<Node> const &node_ptr) : m_node_ptr(node_ptr)
{
}

std::shared_ptr<Node> Cos::NodePtr() const
{
    return m_node_ptr;
}

std::string Cos::Type() const
{
    return "Cos";
}

std::variant<Matrix, std::complex<double>> Cos::Value() const
{
    return std::cos(std::get<std::complex<double>>(m_node_ptr->Value()));
}

Sin::Sin(std::shared_ptr<Node> const &node_ptr) : m_node_ptr(node_ptr)
{
}

std::shared_ptr<Node> Sin::NodePtr() const
{
    return m_node_ptr;
}

std::string Sin::Type() const
{
    return "Sin";
}

std::variant<Matrix, std::complex<double>> Sin::Value() const
{
    return std::sin(std::get<std::complex<double>>(m_node_ptr->Value()));
}

Tan::Tan(std::shared_ptr<Node> const &node_ptr) : m_node_ptr(node_ptr)
{
}

std::shared_ptr<Node> Tan::NodePtr() const
{
    return m_node_ptr;
}

std::string Tan::Type() const
{
    return "Tan";
}

std::variant<Matrix, std::complex<double>> Tan::Value() const
{
    return std::tan(std::get<std::complex<double>>(m_node_ptr->Value()));
}

Acos::Acos(std::shared_ptr<Node> const &node_ptr) : m_node_ptr(node_ptr)
{
}

std::shared_ptr<Node> Acos::NodePtr() const
{
    return m_node_ptr;
}

std::string Acos::Type() const
{
    return "Acos";
}

std::variant<Matrix, std::complex<double>> Acos::Value() const
{
    return std::acos(std::get<std::complex<double>>(m_node_ptr->Value()));
}

Asin::Asin(std::shared_ptr<Node> const &node_ptr) : m_node_ptr(node_ptr)
{
}

std::shared_ptr<Node> Asin::NodePtr() const
{
    return m_node_ptr;
}

std::string Asin::Type() const
{
    return "Asin";
}

std::variant<Matrix, std::complex<double>> Asin::Value() const
{
    return std::asin(std::get<std::complex<double>>(m_node_ptr->Value()));
}

Atan::Atan(std::shared_ptr<Node> const &node_ptr) : m_node_ptr(node_ptr)
{
}

std::shared_ptr<Node> Atan::NodePtr() const
{
    return m_node_ptr;
}

std::string Atan::Type() const
{
    return "Atan";
}

std::variant<Matrix, std::complex<double>> Atan::Value() const
{
    return std::atan(std::get<std::complex<double>>(m_node_ptr->Value()));
}

Sqrt::Sqrt(std::shared_ptr<Node> const &node_ptr) : m_node_ptr(node_ptr)
{
}

std::shared_ptr<Node> Sqrt::NodePtr() const
{
    return m_node_ptr;
}

std::string Sqrt::Type() const
{
    return "Sqrt";
}

std::variant<Matrix, std::complex<double>> Sqrt::Value() const
{
    return std::sqrt(std::get<std::complex<double>>(m_node_ptr->Value()));
}

Abs::Abs(std::shared_ptr<Node> const &node_ptr) : m_node_ptr(node_ptr)
{
}

std::shared_ptr<Node> Abs::NodePtr() const
{
    return m_node_ptr;
}

std::string Abs::Type() const
{
    return "Abs";
}

std::variant<Matrix, std::complex<double>> Abs::Value() const
{
    return std::abs(std::get<std::complex<double>>(m_node_ptr->Value()));
}

Exp::Exp(std::shared_ptr<Node> const &node_ptr) : m_node_ptr(node_ptr)
{
}

std::shared_ptr<Node> Exp::NodePtr() const
{
    return m_node_ptr;
}

std::string Exp::Type() const
{
    return "Exp";
}

std::variant<Matrix, std::complex<double>> Exp::Value() const
{
    return std::exp(std::get<std::complex<double>>(m_node_ptr->Value()));
}

Log::Log(std::shared_ptr<Node> const &node_ptr) : m_node_ptr(node_ptr)
{
}

std::shared_ptr<Node> Log::NodePtr() const
{
    return m_node_ptr;
}

std::string Log::Type() const
{
    return "Log";
}

std::variant<Matrix, std::complex<double>> Log::Value() const
{
    return std::log(std::get<std::complex<double>>(m_node_ptr->Value()));
}
