/*
 * Copyright 2020 Casey Sanchez
 */

#include "functions.hpp"

CosNode::CosNode(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("CosNode accepts only 1 argument");
    }
}

std::string CosNode::Type() const
{
    return "CosNode";
}

std::variant<Matrix, std::complex<double>> CosNode::Value() const
{
    return std::visit(Visitor{}, Argument(0)->Value());
}

SinNode::SinNode(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("SinNode accepts only 1 argument");
    }
}

std::string SinNode::Type() const
{
    return "SinNode";
}

std::variant<Matrix, std::complex<double>> SinNode::Value() const
{
    return std::visit(Visitor{}, Argument(0)->Value());
}

TanNode::TanNode(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("TanNode accepts only 1 argument");
    }
}

std::string TanNode::Type() const
{
    return "TanNode";
}

std::variant<Matrix, std::complex<double>> TanNode::Value() const
{
    return std::visit(Visitor{}, Argument(0)->Value());
}

AcosNode::AcosNode(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("AcosNode accepts only 1 argument");
    }
}

std::string AcosNode::Type() const
{
    return "AcosNode";
}

std::variant<Matrix, std::complex<double>> AcosNode::Value() const
{
    return std::visit(Visitor{}, Argument(0)->Value());
}

AsinNode::AsinNode(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("AsinNode accepts only 1 argument");
    }
}

std::string AsinNode::Type() const
{
    return "AsinNode";
}

std::variant<Matrix, std::complex<double>> AsinNode::Value() const
{
    return std::visit(Visitor{}, Argument(0)->Value());
}

AtanNode::AtanNode(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("AtanNode accepts only 1 argument");
    }
}

std::string AtanNode::Type() const
{
    return "AtanNode";
}

std::variant<Matrix, std::complex<double>> AtanNode::Value() const
{
    return std::visit(Visitor{}, Argument(0)->Value());
}

SqrtNode::SqrtNode(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("SqrtNode accepts only 1 argument");
    }
}

std::string SqrtNode::Type() const
{
    return "SqrtNode";
}

std::variant<Matrix, std::complex<double>> SqrtNode::Value() const
{
    return std::visit(Visitor{}, Argument(0)->Value());
}

AbsNode::AbsNode(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("AbsNode accepts only 1 argument");
    }
}

std::string AbsNode::Type() const
{
    return "AbsNode";
}

std::variant<Matrix, std::complex<double>> AbsNode::Value() const
{
    return std::visit(Visitor{}, Argument(0)->Value());
}

ExpNode::ExpNode(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("ExpNode accepts only 1 argument");
    }
}

std::string ExpNode::Type() const
{
    return "ExpNode";
}

std::variant<Matrix, std::complex<double>> ExpNode::Value() const
{
    return std::visit(Visitor{}, Argument(0)->Value());
}

LogNode::LogNode(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("LogNode accepts only 1 argument");
    }
}

std::string LogNode::Type() const
{
    return "LogNode";
}

std::variant<Matrix, std::complex<double>> LogNode::Value() const
{
    return std::visit(Visitor{}, Argument(0)->Value());
}

DeterminantNode::DeterminantNode(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("DeterminantNode accepts only 1 argument");
    }
}

std::string DeterminantNode::Type() const
{
    return "DeterminantNode";
}

std::variant<Matrix, std::complex<double>> DeterminantNode::Value() const
{
    return std::visit(Visitor{}, Argument(0)->Value());
}

InverseNode::InverseNode(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("InverseNode accepts only 1 argument");
    }
}

std::string InverseNode::Type() const
{
    return "InverseNode";
}

std::variant<Matrix, std::complex<double>> InverseNode::Value() const
{
    return std::visit(Visitor{}, Argument(0)->Value());
}