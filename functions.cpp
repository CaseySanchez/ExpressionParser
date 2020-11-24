/*
 * Copyright 2020 Casey Sanchez
 */

#include "functions.hpp"

Cos::Cos(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("Cos accepts only 1 argument");
    }
}

std::string Cos::Type() const
{
    return "Cos";
}

std::variant<Matrix, std::complex<double>> Cos::Value() const
{
    return std::visit(Visitor{}, m_arguments[0]->Value());
}

Sin::Sin(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("Sin accepts only 1 argument");
    }
}

std::string Sin::Type() const
{
    return "Sin";
}

std::variant<Matrix, std::complex<double>> Sin::Value() const
{
    return std::visit(Visitor{}, m_arguments[0]->Value());
}

Tan::Tan(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("Tan accepts only 1 argument");
    }
}

std::string Tan::Type() const
{
    return "Tan";
}

std::variant<Matrix, std::complex<double>> Tan::Value() const
{
    return std::visit(Visitor{}, m_arguments[0]->Value());
}

Acos::Acos(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("Acos accepts only 1 argument");
    }
}

std::string Acos::Type() const
{
    return "Acos";
}

std::variant<Matrix, std::complex<double>> Acos::Value() const
{
    return std::visit(Visitor{}, m_arguments[0]->Value());
}

Asin::Asin(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("Asin accepts only 1 argument");
    }
}

std::string Asin::Type() const
{
    return "Asin";
}

std::variant<Matrix, std::complex<double>> Asin::Value() const
{
    return std::visit(Visitor{}, m_arguments[0]->Value());
}

Atan::Atan(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("Atan accepts only 1 argument");
    }
}

std::string Atan::Type() const
{
    return "Atan";
}

std::variant<Matrix, std::complex<double>> Atan::Value() const
{
    return std::visit(Visitor{}, m_arguments[0]->Value());
}

Sqrt::Sqrt(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("Sqrt accepts only 1 argument");
    }
}

std::string Sqrt::Type() const
{
    return "Sqrt";
}

std::variant<Matrix, std::complex<double>> Sqrt::Value() const
{
    return std::visit(Visitor{}, m_arguments[0]->Value());
}

Abs::Abs(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("Abs accepts only 1 argument");
    }
}

std::string Abs::Type() const
{
    return "Abs";
}

std::variant<Matrix, std::complex<double>> Abs::Value() const
{
    return std::visit(Visitor{}, m_arguments[0]->Value());
}

Exp::Exp(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("Exp accepts only 1 argument");
    }
}

std::string Exp::Type() const
{
    return "Exp";
}

std::variant<Matrix, std::complex<double>> Exp::Value() const
{
    return std::visit(Visitor{}, m_arguments[0]->Value());
}

Log::Log(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("Log accepts only 1 argument");
    }
}

std::string Log::Type() const
{
    return "Log";
}

std::variant<Matrix, std::complex<double>> Log::Value() const
{
    return std::visit(Visitor{}, m_arguments[0]->Value());
}

Determinant::Determinant(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("Determinant accepts only 1 argument");
    }
}

std::string Determinant::Type() const
{
    return "Determinant";
}

std::variant<Matrix, std::complex<double>> Determinant::Value() const
{
    return std::visit(Visitor{}, m_arguments[0]->Value());
}

Inverse::Inverse(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("Inverse accepts only 1 argument");
    }
}

std::string Inverse::Type() const
{
    return "Inverse";
}

std::variant<Matrix, std::complex<double>> Inverse::Value() const
{
    return std::visit(Visitor{}, m_arguments[0]->Value());
}