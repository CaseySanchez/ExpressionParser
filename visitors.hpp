#pragma once

#include <variant>

#include "matrix.hpp"
#include "operations.hpp"
#include "functions.hpp"

struct AdditionVisitor
{
    std::variant<Scalar, Matrix> operator()(Scalar const &lhs, Scalar const &rhs)
    {
        return Scalar(new AdditionNode({ lhs, rhs }));
    }

    std::variant<Scalar, Matrix> operator()(Scalar const &lhs, Matrix const &rhs)
    {
        Matrix matrix(rhs.Rows(), rhs.Cols());

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = Scalar(new AdditionNode({ lhs, rhs(i, j) }));
            }
        }
        
        return matrix;
    }

    std::variant<Scalar, Matrix> operator()(Matrix const &lhs, Scalar const &rhs)
    {
        Matrix matrix(lhs.Rows(), lhs.Cols());

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = Scalar(new AdditionNode({ lhs(i, j), rhs }));
            }
        }
        
        return matrix;
    }

    std::variant<Scalar, Matrix> operator()(Matrix const &lhs, Matrix const &rhs)
    {
        if (lhs.Rows() != rhs.Rows() || lhs.Cols() != rhs.Cols()) {
            throw std::invalid_argument("AdditionVisitor: Dimensions are not equal");
        }

        Matrix matrix(lhs.Rows(), lhs.Cols());

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = Scalar(new AdditionNode({ lhs(i, j), rhs(i, j) }));
            }
        }
        
        return matrix;
    }
};

struct SubtractionVisitor
{
    std::variant<Scalar, Matrix> operator()(Scalar const &lhs, Scalar const &rhs)
    {
        return Scalar(new SubtractionNode({ lhs, rhs }));
    }

    std::variant<Scalar, Matrix> operator()(Scalar const &lhs, Matrix const &rhs)
    {
        Matrix matrix(rhs.Rows(), rhs.Cols());

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = Scalar(new SubtractionNode({ lhs, rhs(i, j) }));
            }
        }
        
        return matrix;
    }

    std::variant<Scalar, Matrix> operator()(Matrix const &lhs, Scalar const &rhs)
    {
        Matrix matrix(lhs.Rows(), lhs.Cols());

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = Scalar(new SubtractionNode({ lhs(i, j), rhs }));
            }
        }
        
        return matrix;
    }

    std::variant<Scalar, Matrix> operator()(Matrix const &lhs, Matrix const &rhs)
    {
        if (lhs.Rows() != rhs.Rows() || lhs.Cols() != rhs.Cols()) {
            throw std::invalid_argument("SubtractionVisitor: Dimensions are not equal");
        }

        Matrix matrix(lhs.Rows(), lhs.Cols());

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = Scalar(new SubtractionNode({ lhs(i, j), rhs(i, j) }));
            }
        }
        
        return matrix;
    }
};

struct MultiplicationVisitor
{
    std::variant<Scalar, Matrix> operator()(Scalar const &lhs, Scalar const &rhs)
    {
        return Scalar(new MultiplicationNode({ lhs, rhs }));
    }

    std::variant<Scalar, Matrix> operator()(Scalar const &lhs, Matrix const &rhs)
    {
        Matrix matrix(rhs.Rows(), rhs.Cols());

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = Scalar(new MultiplicationNode({ lhs, rhs(i, j) }));
            }
        }
        
        return matrix;
    }

    std::variant<Scalar, Matrix> operator()(Matrix const &lhs, Scalar const &rhs)
    {
        Matrix matrix(lhs.Rows(), lhs.Cols());

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = Scalar(new MultiplicationNode({ lhs(i, j), rhs }));
            }
        }
        
        return matrix;
    }

    std::variant<Scalar, Matrix> operator()(Matrix const &lhs, Matrix const &rhs)
    {
        if (lhs.Cols() != rhs.Rows()) {
            throw std::invalid_argument("MultiplicationVisitor: LHS cols must equal RHS rows");
        }

        Matrix matrix(lhs.Rows(), rhs.Cols());

        for (size_t i = 0; i < lhs.Rows(); ++i) {
            for (size_t j = 0; j < rhs.Cols(); ++j) {
                Scalar sum(new ConstantNode(0.0));

                for (size_t k = 0; k < lhs.Cols(); ++k) {
                    Scalar product(new MultiplicationNode({ lhs(i, k), rhs(k, j) }));
                    
                    sum = Scalar(new AdditionNode({ sum, product }));
                }

                matrix(i, j) = sum;
            }
        }

        return matrix;
    }
};

struct DivisionVisitor
{
    std::variant<Scalar, Matrix> operator()(Scalar const &lhs, Scalar const &rhs)
    {
        return Scalar(new DivisionNode({ lhs, rhs }));
    }

    std::variant<Scalar, Matrix> operator()(Scalar const &lhs, Matrix const &rhs)
    {
        throw std::invalid_argument("DivisionVisitor unrecognized arguments Scalar and Matrix");
    }

    std::variant<Scalar, Matrix> operator()(Matrix const &lhs, Scalar const &rhs)
    {
        throw std::invalid_argument("DivisionVisitor unrecognized arguments Matrix and Scalar");
    }

    std::variant<Scalar, Matrix> operator()(Matrix const &lhs, Matrix const &rhs)
    {
        throw std::invalid_argument("DivisionVisitor unrecognized arguments Matrix and Matrix");
    }
};

struct ExponentiationVisitor
{
    std::variant<Scalar, Matrix> operator()(Scalar const &lhs, Scalar const &rhs)
    {
        return Scalar(new ExponentiationNode({ lhs, rhs }));
    }

    std::variant<Scalar, Matrix> operator()(Scalar const &lhs, Matrix const &rhs)
    {
        throw std::invalid_argument("ExponentiationVisitor unrecognized arguments Scalar and Matrix");
    }

    std::variant<Scalar, Matrix> operator()(Matrix const &lhs, Scalar const &rhs)
    {
        throw std::invalid_argument("ExponentiationVisitor unrecognized arguments Matrix and Scalar");
    }

    std::variant<Scalar, Matrix> operator()(Matrix const &lhs, Matrix const &rhs)
    {
        throw std::invalid_argument("ExponentiationVisitor unrecognized arguments Matrix and Matrix");
    }
};

struct CosVisitor
{
    std::variant<Scalar, Matrix> operator()(Scalar const &arg)
    {
        return Scalar(std::shared_ptr<CosNode>(new CosNode({ arg })));
    }

    std::variant<Scalar, Matrix> operator()(Matrix const &arg)
    {
        Matrix matrix(arg.Rows(), arg.Cols());

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = Scalar(new CosNode({ arg(i, j) }));
            }
        }
        
        return matrix;
    }
};

struct SinVisitor
{
    std::variant<Scalar, Matrix> operator()(Scalar const &arg)
    {
        return Scalar(new SinNode({ arg }));
    }

    std::variant<Scalar, Matrix> operator()(Matrix const &arg)
    {
        Matrix matrix(arg.Rows(), arg.Cols());

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = Scalar(new SinNode({ arg(i, j) }));
            }
        }
        
        return matrix;
    }
};

struct TanVisitor
{
    std::variant<Scalar, Matrix> operator()(Scalar const &arg)
    {
        return Scalar(new TanNode({ arg }));
    }

    std::variant<Scalar, Matrix> operator()(Matrix const &arg)
    {
        Matrix matrix(arg.Rows(), arg.Cols());

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = Scalar(new TanNode({ arg(i, j) }));
            }
        }
        
        return matrix;
    }
};

struct AcosVisitor
{
    std::variant<Scalar, Matrix> operator()(Scalar const &arg)
    {
        return Scalar(new AcosNode({ arg }));
    }

    std::variant<Scalar, Matrix> operator()(Matrix const &arg)
    {
        Matrix matrix(arg.Rows(), arg.Cols());

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = Scalar(new AcosNode({ arg(i, j) }));
            }
        }
        
        return matrix;
    }
};

struct AsinVisitor
{
    std::variant<Scalar, Matrix> operator()(Scalar const &arg)
    {
        return Scalar(std::shared_ptr<AsinNode>(new AsinNode({ arg })));
    }

    std::variant<Scalar, Matrix> operator()(Matrix const &arg)
    {
        Matrix matrix(arg.Rows(), arg.Cols());

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = Scalar(new AsinNode({ arg(i, j) }));
            }
        }
        
        return matrix;
    }
};

struct AtanVisitor
{
    std::variant<Scalar, Matrix> operator()(Scalar const &arg)
    {
        return Scalar(new AtanNode({ arg }));
    }

    std::variant<Scalar, Matrix> operator()(Matrix const &arg)
    {
        Matrix matrix(arg.Rows(), arg.Cols());

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = Scalar(new AtanNode({ arg(i, j) }));
            }
        }
        
        return matrix;
    }
};

struct SqrtVisitor
{
    std::variant<Scalar, Matrix> operator()(Scalar const &arg)
    {
        return Scalar(new SqrtNode({ arg }));
    }

    std::variant<Scalar, Matrix> operator()(Matrix const &arg)
    {
        Matrix matrix(arg.Rows(), arg.Cols());

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = Scalar(new SqrtNode({ arg(i, j) }));
            }
        }
        
        return matrix;
    }
};

struct AbsVisitor
{
    std::variant<Scalar, Matrix> operator()(Scalar const &arg)
    {
        return Scalar(new AbsNode({ arg }));
    }

    std::variant<Scalar, Matrix> operator()(Matrix const &arg)
    {
        Matrix matrix(arg.Rows(), arg.Cols());

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = Scalar(new AbsNode({ arg(i, j) }));
            }
        }
        
        return matrix;
    }
};

struct ExpVisitor
{
    std::variant<Scalar, Matrix> operator()(Scalar const &arg)
    {
        return Scalar(new ExpNode({ arg }));
    }

    std::variant<Scalar, Matrix> operator()(Matrix const &arg)
    {
        Matrix matrix(arg.Rows(), arg.Cols());

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = Scalar(new ExpNode({ arg(i, j) }));
            }
        }
        
        return matrix;
    }
};

struct LnVisitor
{
    std::variant<Scalar, Matrix> operator()(Scalar const &arg)
    {
        return Scalar(new LnNode({ arg }));
    }

    std::variant<Scalar, Matrix> operator()(Matrix const &arg)
    {
        Matrix matrix(arg.Rows(), arg.Cols());

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = Scalar(new LnNode({ arg(i, j) }));
            }
        }
        
        return matrix;
    }
};
