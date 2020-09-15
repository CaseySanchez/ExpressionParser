/*
 * Copyright 2020 Casey Sanchez
 */

#include "function.hpp"

Cos::Cos(std::shared_ptr<Node> const &node_ptr) : node_ptr(node_ptr)
{
}

std::shared_ptr<Node> Cos::NodePtr() const
{
    return node_ptr;
}

std::string Cos::Type() const
{
    return "Cos";
}

double Cos::Value() const
{
    return std::cos(node_ptr->Value());
}

Sin::Sin(std::shared_ptr<Node> const &node_ptr) : node_ptr(node_ptr)
{
}

std::shared_ptr<Node> Sin::NodePtr() const
{
    return node_ptr;
}

std::string Sin::Type() const
{
    return "Sin";
}

double Sin::Value() const
{
    return std::sin(node_ptr->Value());
}

Tan::Tan(std::shared_ptr<Node> const &node_ptr) : node_ptr(node_ptr)
{
}

std::shared_ptr<Node> Tan::NodePtr() const
{
    return node_ptr;
}

std::string Tan::Type() const
{
    return "Tan";
}

double Tan::Value() const
{
    return std::tan(node_ptr->Value());
}

Acos::Acos(std::shared_ptr<Node> const &node_ptr) : node_ptr(node_ptr)
{
}

std::shared_ptr<Node> Acos::NodePtr() const
{
    return node_ptr;
}

std::string Acos::Type() const
{
    return "Acos";
}

double Acos::Value() const
{
    return std::acos(node_ptr->Value());
}

Asin::Asin(std::shared_ptr<Node> const &node_ptr) : node_ptr(node_ptr)
{
}

std::shared_ptr<Node> Asin::NodePtr() const
{
    return node_ptr;
}

std::string Asin::Type() const
{
    return "Asin";
}

double Asin::Value() const
{
    return std::asin(node_ptr->Value());
}

Atan::Atan(std::shared_ptr<Node> const &node_ptr) : node_ptr(node_ptr)
{
}

std::shared_ptr<Node> Atan::NodePtr() const
{
    return node_ptr;
}

std::string Atan::Type() const
{
    return "Atan";
}

double Atan::Value() const
{
    return std::atan(node_ptr->Value());
}

Sqrt::Sqrt(std::shared_ptr<Node> const &node_ptr) : node_ptr(node_ptr)
{
}

std::shared_ptr<Node> Sqrt::NodePtr() const
{
    return node_ptr;
}

std::string Sqrt::Type() const
{
    return "Sqrt";
}

double Sqrt::Value() const
{
    return std::sqrt(node_ptr->Value());
}

Abs::Abs(std::shared_ptr<Node> const &node_ptr) : node_ptr(node_ptr)
{
}

std::shared_ptr<Node> Abs::NodePtr() const
{
    return node_ptr;
}

std::string Abs::Type() const
{
    return "Abs";
}

double Abs::Value() const
{
    return std::abs(node_ptr->Value());
}

Exp::Exp(std::shared_ptr<Node> const &node_ptr) : node_ptr(node_ptr)
{
}

std::shared_ptr<Node> Exp::NodePtr() const
{
    return node_ptr;
}

std::string Exp::Type() const
{
    return "Exp";
}

double Exp::Value() const
{
    return std::exp(node_ptr->Value());
}

Log::Log(std::shared_ptr<Node> const &node_ptr) : node_ptr(node_ptr)
{
}

std::shared_ptr<Node> Log::NodePtr() const
{
    return node_ptr;
}

std::string Log::Type() const
{
    return "Log";
}

double Log::Value() const
{
    return std::log(node_ptr->Value());
}
