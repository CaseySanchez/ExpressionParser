/*
 * Copyright 2020 Casey Sanchez
 */

#include "operations.hpp"

Affirmation::Affirmation(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("Affirmation accepts only 1 argument");
    }
}

std::string Affirmation::Type() const
{
    return "Affirmation";
}

std::variant<Matrix, std::complex<double>> Affirmation::Value() const
{
    return std::visit(Visitor{}, m_arguments[0]->Value());
}

Negation::Negation(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 1) {
        throw std::invalid_argument("Negation accepts only 1 argument");
    }
}

std::string Negation::Type() const
{
    return "Negation";
}

std::variant<Matrix, std::complex<double>> Negation::Value() const
{
    return std::visit(Visitor{}, m_arguments[0]->Value());
}

Exponentiation::Exponentiation(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 2) {
        throw std::invalid_argument("Exponentiation accepts only 2 arguments");
    }
}

std::string Exponentiation::Type() const
{
    return "Exponentiation";
}

std::variant<Matrix, std::complex<double>> Exponentiation::Value() const
{
    return std::visit(Visitor{}, m_arguments[0]->Value(), m_arguments[1]->Value());
}

Multiplication::Multiplication(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 2) {
        throw std::invalid_argument("Multiplication accepts only 2 arguments");
    }
}

std::string Multiplication::Type() const
{
    return "Multiplication";
}

std::variant<Matrix, std::complex<double>> Multiplication::Value() const
{
    return std::visit(Visitor{}, m_arguments[0]->Value(), m_arguments[1]->Value());
}

Division::Division(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 2) {
        throw std::invalid_argument("Division accepts only 2 arguments");
    }
}

std::string Division::Type() const
{
    return "Division";
}

std::variant<Matrix, std::complex<double>> Division::Value() const
{
    return std::visit(Visitor{}, m_arguments[0]->Value(), m_arguments[1]->Value());
}

Addition::Addition(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 2) {
        throw std::invalid_argument("Addition accepts only 2 arguments");
    }
}

std::string Addition::Type() const
{
    return "Addition";
}

std::variant<Matrix, std::complex<double>> Addition::Value() const
{
    return std::visit(Visitor{}, m_arguments[0]->Value(), m_arguments[1]->Value());
}

Subtraction::Subtraction(std::initializer_list<std::shared_ptr<Node>> const &arguments) : Node(arguments)
{
    if (arguments.size() != 2) {
        throw std::invalid_argument("Subtraction accepts only 2 arguments");
    }
}

std::string Subtraction::Type() const
{
    return "Subtraction";
}

std::variant<Matrix, std::complex<double>> Subtraction::Value() const
{
    return std::visit(Visitor{}, m_arguments[0]->Value(), m_arguments[1]->Value());
}
