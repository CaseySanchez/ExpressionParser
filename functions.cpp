/*
 * Copyright 2020 Casey Sanchez
 */

#include "functions.hpp"

CosNode::CosNode(std::initializer_list<Scalar> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("CosNode accepts only 1 argument");
    }
}

std::string CosNode::Type() const
{
    return "CosNode";
}

std::complex<double> CosNode::Value() const
{
    return std::cos(Argument(0)->Value());
}

SinNode::SinNode(std::initializer_list<Scalar> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("SinNode accepts only 1 argument");
    }
}

std::string SinNode::Type() const
{
    return "SinNode";
}

std::complex<double> SinNode::Value() const
{
    return std::sin(Argument(0)->Value());
}

TanNode::TanNode(std::initializer_list<Scalar> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("TanNode accepts only 1 argument");
    }
}

std::string TanNode::Type() const
{
    return "TanNode";
}

std::complex<double> TanNode::Value() const
{
    return std::tan(Argument(0)->Value());
}

AcosNode::AcosNode(std::initializer_list<Scalar> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("AcosNode accepts only 1 argument");
    }
}

std::string AcosNode::Type() const
{
    return "AcosNode";
}

std::complex<double> AcosNode::Value() const
{
    return std::acos(Argument(0)->Value());
}

AsinNode::AsinNode(std::initializer_list<Scalar> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("AsinNode accepts only 1 argument");
    }
}

std::string AsinNode::Type() const
{
    return "AsinNode";
}

std::complex<double> AsinNode::Value() const
{
    return std::asin(Argument(0)->Value());
}

AtanNode::AtanNode(std::initializer_list<Scalar> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("AtanNode accepts only 1 argument");
    }
}

std::string AtanNode::Type() const
{
    return "AtanNode";
}

std::complex<double> AtanNode::Value() const
{
    return std::atan(Argument(0)->Value());
}

SqrtNode::SqrtNode(std::initializer_list<Scalar> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("SqrtNode accepts only 1 argument");
    }
}

std::string SqrtNode::Type() const
{
    return "SqrtNode";
}

std::complex<double> SqrtNode::Value() const
{
    return std::sqrt(Argument(0)->Value());
}

AbsNode::AbsNode(std::initializer_list<Scalar> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("AbsNode accepts only 1 argument");
    }
}

std::string AbsNode::Type() const
{
    return "AbsNode";
}

std::complex<double> AbsNode::Value() const
{
    return std::abs(Argument(0)->Value());
}

ExpNode::ExpNode(std::initializer_list<Scalar> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("ExpNode accepts only 1 argument");
    }
}

std::string ExpNode::Type() const
{
    return "ExpNode";
}

std::complex<double> ExpNode::Value() const
{
    return std::exp(Argument(0)->Value());
}

LnNode::LnNode(std::initializer_list<Scalar> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("LnNode accepts only 1 argument");
    }
}

std::string LnNode::Type() const
{
    return "LnNode";
}

std::complex<double> LnNode::Value() const
{
    return std::log(Argument(0)->Value());
}
