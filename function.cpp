/*
 * Copyright 2020 Casey Sanchez
 */

#include "function.hpp"

Cos::Cos(std::shared_ptr<Node> const &node_ptr) : node_ptr(node_ptr)
{
    type = "Cos";
}

std::shared_ptr<Node> Cos::NodePtr() const
{
    return node_ptr;
}

double Cos::Value() const
{
    return std::cos(node_ptr->Value());
}

Sin::Sin(std::shared_ptr<Node> const &node_ptr) : node_ptr(node_ptr)
{
    type = "Sin";
}

std::shared_ptr<Node> Sin::NodePtr() const
{
    return node_ptr;
}

double Sin::Value() const
{
    return std::sin(node_ptr->Value());
}

Tan::Tan(std::shared_ptr<Node> const &node_ptr) : node_ptr(node_ptr)
{
    type = "Tan";
}

std::shared_ptr<Node> Tan::NodePtr() const
{
    return node_ptr;
}

double Tan::Value() const
{
    return std::tan(node_ptr->Value());
}

Acos::Acos(std::shared_ptr<Node> const &node_ptr) : node_ptr(node_ptr)
{
    type = "Acos";
}

std::shared_ptr<Node> Acos::NodePtr() const
{
    return node_ptr;
}

double Acos::Value() const
{
    return std::acos(node_ptr->Value());
}

Asin::Asin(std::shared_ptr<Node> const &node_ptr) : node_ptr(node_ptr)
{
    type = "Asin";
}

std::shared_ptr<Node> Asin::NodePtr() const
{
    return node_ptr;
}

double Asin::Value() const
{
    return std::asin(node_ptr->Value());
}

Atan::Atan(std::shared_ptr<Node> const &node_ptr) : node_ptr(node_ptr)
{
    type = "Atan";
}

std::shared_ptr<Node> Atan::NodePtr() const
{
    return node_ptr;
}

double Atan::Value() const
{
    return std::atan(node_ptr->Value());
}

Sqrt::Sqrt(std::shared_ptr<Node> const &node_ptr) : node_ptr(node_ptr)
{
    type = "Sqrt";
}

std::shared_ptr<Node> Sqrt::NodePtr() const
{
    return node_ptr;
}

double Sqrt::Value() const
{
    return std::sqrt(node_ptr->Value());
}

Abs::Abs(std::shared_ptr<Node> const &node_ptr) : node_ptr(node_ptr)
{
    type = "Abs";
}

std::shared_ptr<Node> Abs::NodePtr() const
{
    return node_ptr;
}

double Abs::Value() const
{
    return std::abs(node_ptr->Value());
}

Exp::Exp(std::shared_ptr<Node> const &node_ptr) : node_ptr(node_ptr)
{
    type = "Exp";
}

std::shared_ptr<Node> Exp::NodePtr() const
{
    return node_ptr;
}

double Exp::Value() const
{
    return std::exp(node_ptr->Value());
}

Log::Log(std::shared_ptr<Node> const &node_ptr) : node_ptr(node_ptr)
{
    type = "Log";
}

std::shared_ptr<Node> Log::NodePtr() const
{
    return node_ptr;
}

double Log::Value() const
{
    return std::log(node_ptr->Value());
}
