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
            Matrix matrix(value.Rows(), value.Cols());

            for (size_t i = 0; i < matrix.Rows(); ++i) {
                for (size_t j = 0; j < matrix.Cols(); ++j) {
                    matrix(i, j) = std::shared_ptr<Cos>(new Cos({ value(i, j) }));
                }
            }

            return matrix;
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
            Matrix matrix(value.Rows(), value.Cols());

            for (size_t i = 0; i < matrix.Rows(); ++i) {
                for (size_t j = 0; j < matrix.Cols(); ++j) {
                    matrix(i, j) = std::shared_ptr<Sin>(new Sin({ value(i, j) }));
                }
            }

            return matrix;
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
            Matrix matrix(value.Rows(), value.Cols());

            for (size_t i = 0; i < matrix.Rows(); ++i) {
                for (size_t j = 0; j < matrix.Cols(); ++j) {
                    matrix(i, j) = std::shared_ptr<Tan>(new Tan({ value(i, j) }));
                }
            }

            return matrix;
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
            Matrix matrix(value.Rows(), value.Cols());

            for (size_t i = 0; i < matrix.Rows(); ++i) {
                for (size_t j = 0; j < matrix.Cols(); ++j) {
                    matrix(i, j) = std::shared_ptr<Acos>(new Acos({ value(i, j) }));
                }
            }

            return matrix;
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
            Matrix matrix(value.Rows(), value.Cols());

            for (size_t i = 0; i < matrix.Rows(); ++i) {
                for (size_t j = 0; j < matrix.Cols(); ++j) {
                    matrix(i, j) = std::shared_ptr<Asin>(new Asin({ value(i, j) }));
                }
            }

            return matrix;
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
            Matrix matrix(value.Rows(), value.Cols());

            for (size_t i = 0; i < matrix.Rows(); ++i) {
                for (size_t j = 0; j < matrix.Cols(); ++j) {
                    matrix(i, j) = std::shared_ptr<Atan>(new Atan({ value(i, j) }));
                }
            }

            return matrix;
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
            Matrix matrix(value.Rows(), value.Cols());

            for (size_t i = 0; i < matrix.Rows(); ++i) {
                for (size_t j = 0; j < matrix.Cols(); ++j) {
                    matrix(i, j) = std::shared_ptr<Sqrt>(new Sqrt({ value(i, j) }));
                }
            }

            return matrix;
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
            Matrix matrix(value.Rows(), value.Cols());

            for (size_t i = 0; i < matrix.Rows(); ++i) {
                for (size_t j = 0; j < matrix.Cols(); ++j) {
                    matrix(i, j) = std::shared_ptr<Abs>(new Abs({ value(i, j) }));
                }
            }

            return matrix;
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
            Matrix matrix(value.Rows(), value.Cols());

            for (size_t i = 0; i < matrix.Rows(); ++i) {
                for (size_t j = 0; j < matrix.Cols(); ++j) {
                    matrix(i, j) = std::shared_ptr<Exp>(new Exp({ value(i, j) }));
                }
            }

            return matrix;
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
            Matrix matrix(value.Rows(), value.Cols());

            for (size_t i = 0; i < matrix.Rows(); ++i) {
                for (size_t j = 0; j < matrix.Cols(); ++j) {
                    matrix(i, j) = std::shared_ptr<Log>(new Log({ value(i, j) }));
                }
            }

            return matrix;
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
