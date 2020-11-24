/*
 * Copyright 2020 Casey Sanchez
 */

#pragma once

#include <memory>
#include <cmath>

#include "node.hpp"
#include "matrix.hpp"

class Cos : public Node
{
    struct Visitor
    {
        std::variant<Matrix, std::complex<double>> operator()(std::complex<double> const &value)
        {
            return std::cos(value);
        }

        std::variant<Matrix, std::complex<double>> operator()(Matrix const &value)
        {
            throw std::invalid_argument("Undefined function: Cos(matrix)");
        }
    };

public:
    Cos(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class Sin : public Node
{
    struct Visitor
    {
        std::variant<Matrix, std::complex<double>> operator()(std::complex<double> const &value)
        {
            return std::sin(value);
        }

        std::variant<Matrix, std::complex<double>> operator()(Matrix const &value)
        {
            throw std::invalid_argument("Undefined function: Sin(matrix)");
        }
    };

public:
    Sin(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class Tan : public Node
{
    struct Visitor
    {
        std::variant<Matrix, std::complex<double>> operator()(std::complex<double> const &value)
        {
            return std::tan(value);
        }

        std::variant<Matrix, std::complex<double>> operator()(Matrix const &value)
        {
            throw std::invalid_argument("Undefined function: Tan(matrix)");
        }
    };

public:
    Tan(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class Acos : public Node
{
    struct Visitor
    {
        std::variant<Matrix, std::complex<double>> operator()(std::complex<double> const &value)
        {
            return std::acos(value);
        }

        std::variant<Matrix, std::complex<double>> operator()(Matrix const &value)
        {
            throw std::invalid_argument("Undefined function: Acos(matrix)");
        }
    };

public:
    Acos(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class Asin : public Node
{
    struct Visitor
    {
        std::variant<Matrix, std::complex<double>> operator()(std::complex<double> const &value)
        {
            return std::asin(value);
        }

        std::variant<Matrix, std::complex<double>> operator()(Matrix const &value)
        {
            throw std::invalid_argument("Undefined function: Asin(matrix)");
        }
    };

public:
    Asin(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class Atan : public Node
{
    struct Visitor
    {
        std::variant<Matrix, std::complex<double>> operator()(std::complex<double> const &value)
        {
            return std::atan(value);
        }

        std::variant<Matrix, std::complex<double>> operator()(Matrix const &value)
        {
            throw std::invalid_argument("Undefined function: Atan(matrix)");
        }
    };

public:
    Atan(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class Sqrt : public Node
{
    struct Visitor
    {
        std::variant<Matrix, std::complex<double>> operator()(std::complex<double> const &value)
        {
            return std::sqrt(value);
        }

        std::variant<Matrix, std::complex<double>> operator()(Matrix const &value)
        {
            throw std::invalid_argument("Undefined function: Sqrt(matrix)");
        }
    };

public:
    Sqrt(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class Abs : public Node
{
    struct Visitor
    {
        std::variant<Matrix, std::complex<double>> operator()(std::complex<double> const &value)
        {
            return std::abs(value);
        }

        std::variant<Matrix, std::complex<double>> operator()(Matrix const &value)
        {
            throw std::invalid_argument("Undefined function: Abs(matrix)");
        }
    };

public:
    Abs(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class Exp : public Node
{
    struct Visitor
    {
        std::variant<Matrix, std::complex<double>> operator()(std::complex<double> const &value)
        {
            return std::exp(value);
        }

        std::variant<Matrix, std::complex<double>> operator()(Matrix const &value)
        {
            throw std::invalid_argument("Undefined function: Exp(matrix)");
        }
    };

public:
    Exp(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class Log : public Node
{
    struct Visitor
    {
        std::variant<Matrix, std::complex<double>> operator()(std::complex<double> const &value)
        {
            return std::log(value);
        }

        std::variant<Matrix, std::complex<double>> operator()(Matrix const &value)
        {
            throw std::invalid_argument("Undefined function: Log(matrix)");
        }
    };

public:
    Log(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class Determinant : public Node
{
    struct Visitor
    {
        std::variant<Matrix, std::complex<double>> operator()(std::complex<double> const &value)
        {
            throw std::invalid_argument("Undefined function: det complex");
        }

        std::variant<Matrix, std::complex<double>> operator()(Matrix const &value)
        {
            std::shared_ptr<Node> determinant_ptr = value.Determinant();

            return determinant_ptr->Value();
        }
    };

public:
    Determinant(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class Inverse : public Node
{
    struct Visitor
    {
        std::variant<Matrix, std::complex<double>> operator()(std::complex<double> const &value)
        {
            return 1.0 / value;
        }

        std::variant<Matrix, std::complex<double>> operator()(Matrix const &value)
        {
            return value.Inverse();
        }
    };

public:
    Inverse(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};
