/*
 * Copyright 2020 Casey Sanchez
 */

#pragma once

#include <memory>
#include <cmath>

#include "node.hpp"
#include "matrix.hpp"

class CosNode : public Node
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
                    matrix(i, j) = std::shared_ptr<CosNode>(new CosNode({ value(i, j) }));
                }
            }

            return matrix;
        }
    };

public:
    CosNode(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class SinNode : public Node
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
                    matrix(i, j) = std::shared_ptr<SinNode>(new SinNode({ value(i, j) }));
                }
            }

            return matrix;
        }
    };

public:
    SinNode(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class TanNode : public Node
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
                    matrix(i, j) = std::shared_ptr<TanNode>(new TanNode({ value(i, j) }));
                }
            }

            return matrix;
        }
    };

public:
    TanNode(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class AcosNode : public Node
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
                    matrix(i, j) = std::shared_ptr<AcosNode>(new AcosNode({ value(i, j) }));
                }
            }

            return matrix;
        }
    };

public:
    AcosNode(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class AsinNode : public Node
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
                    matrix(i, j) = std::shared_ptr<AsinNode>(new AsinNode({ value(i, j) }));
                }
            }

            return matrix;
        }
    };

public:
    AsinNode(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class AtanNode : public Node
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
                    matrix(i, j) = std::shared_ptr<AtanNode>(new AtanNode({ value(i, j) }));
                }
            }

            return matrix;
        }
    };

public:
    AtanNode(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class SqrtNode : public Node
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
                    matrix(i, j) = std::shared_ptr<SqrtNode>(new SqrtNode({ value(i, j) }));
                }
            }

            return matrix;
        }
    };

public:
    SqrtNode(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class AbsNode : public Node
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
                    matrix(i, j) = std::shared_ptr<AbsNode>(new AbsNode({ value(i, j) }));
                }
            }

            return matrix;
        }
    };

public:
    AbsNode(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class ExpNode : public Node
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
                    matrix(i, j) = std::shared_ptr<ExpNode>(new ExpNode({ value(i, j) }));
                }
            }

            return matrix;
        }
    };

public:
    ExpNode(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class LogNode : public Node
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
                    matrix(i, j) = std::shared_ptr<LogNode>(new LogNode({ value(i, j) }));
                }
            }

            return matrix;
        }
    };

public:
    LogNode(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class DeterminantNode : public Node
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
    DeterminantNode(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class InverseNode : public Node
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
    InverseNode(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};

class Derivative : public Node
{
    struct Visitor
    {
        std::variant<Matrix, std::complex<double>> operator()(std::complex<double> const &value)
        {
            return value;
        }

        std::variant<Matrix, std::complex<double>> operator()(Matrix const &value)
        {
            return value;
        }
    };

public:
    Derivative(std::initializer_list<std::shared_ptr<Node>> const &arguments);

    std::string Type() const override;

    std::variant<Matrix, std::complex<double>> Value() const override;
};