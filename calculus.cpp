/*
* Copyright 2020 Casey Sanchez
*/

#include "calculus.hpp"

Calculus::Calculus(std::variant<Scalar, Matrix> const &node_variant, std::map<std::string, std::variant<Scalar, Matrix>> const &node_map) : m_node_variant(node_variant), m_node_map(node_map)
{
}

Scalar Calculus::Partial(Scalar const &with_respect_to_ptr)
{
    return Partial(m_node_variant, with_respect_to_ptr);
}

Scalar Calculus::Partial(std::variant<Scalar, Matrix> const &node_variant, Scalar const &with_respect_to_ptr)
{
    if (!std::holds_alternative<Scalar>(node_variant)) {
        throw std::invalid_argument("Partial only defined for Scalar");
    }

    Scalar scalar = std::get<Scalar>(node_variant);

    // d/dx { x } = 1
    if (Node::Equivalent(scalar, with_respect_to_ptr)) {
        return std::shared_ptr<ConstantNode>(new ConstantNode(1.0));
    }
    // d/dx { 1 } = 0
    else if (scalar->Type() == "ConstantNode") {
        return std::shared_ptr<ConstantNode>(new ConstantNode(0.0));
    }
    // d/dx { y } = 0
    else if (scalar->Type() == "VariableNode") {
        return std::shared_ptr<ConstantNode>(new ConstantNode(0.0));
    }
    // d/dx { f(x) + g(x) } = f'(x) + g'(x)
    else if (scalar->Type() == "AdditionNode") {
        return std::shared_ptr<AdditionNode>(new AdditionNode({
            Partial(scalar->Argument(0), with_respect_to_ptr),
            Partial(scalar->Argument(1), with_respect_to_ptr)
        }));
    }
    // d/dx { f(x) - g(x) } = f'(x) - g'(x)
    else if (scalar->Type() == "SubtractionNode") {
        return std::shared_ptr<SubtractionNode>(new SubtractionNode({
            Partial(scalar->Argument(0), with_respect_to_ptr),
            Partial(scalar->Argument(1), with_respect_to_ptr)
        }));
    }
    // d/dx { f(x) * g(x) } = f'(x) * g(x) + g'(x) * f(x)
    else if (scalar->Type() == "MultiplicationNode") {
        return std::shared_ptr<AdditionNode>(new AdditionNode({
            std::shared_ptr<MultiplicationNode>(new MultiplicationNode({ 
                Partial(scalar->Argument(0), with_respect_to_ptr),
                scalar->Argument(1)
            })),
            std::shared_ptr<MultiplicationNode>(new MultiplicationNode({
                Partial(scalar->Argument(1), with_respect_to_ptr),
                scalar->Argument(0)
            }))
        }));
    }
    // d/dx { f(x) / g(x) } = { f'(x) * g(x) - g'(x) * f(x) } / { g(x) }^2
    else if (scalar->Type() == "DivisionNode") {
        return std::shared_ptr<DivisionNode>(new DivisionNode({
            std::shared_ptr<SubtractionNode>(new SubtractionNode({
                std::shared_ptr<MultiplicationNode>(new MultiplicationNode({ 
                    Partial(scalar->Argument(0), with_respect_to_ptr),
                    scalar->Argument(1)
                })),
                std::shared_ptr<MultiplicationNode>(new MultiplicationNode({
                    Partial(scalar->Argument(1), with_respect_to_ptr),
                    scalar->Argument(0)
                }))
            })),
            std::shared_ptr<ExponentiationNode>(new ExponentiationNode({
                scalar->Argument(1),
                std::shared_ptr<ConstantNode>(new ConstantNode(2.0))
            }))
        }));
    }
    // d/dx { x^2 } = d/dx { e^{ ln(x) * 2 } } = e^{ ln(x) * 2 } * d/dx { ln(x) * 2 } = 2 * x^2 * x^{-1} = 2 * x
    else if (scalar->Type() == "ExponentiationNode") {
        return Partial(std::shared_ptr<ExpNode>(new ExpNode({
                std::shared_ptr<MultiplicationNode>(new MultiplicationNode({ 
                    std::shared_ptr<LnNode>(new LnNode({ scalar->Argument(0) })),
                    scalar->Argument(1)
                }))
            })), with_respect_to_ptr);
    }
    // d/dx { e^{ x^2 } } = e^{ x^2 } * d/dx { x^2 } = e^{ x^2 } * 2 * x
    else if (scalar->Type() == "ExpNode") {
        return std::shared_ptr<MultiplicationNode>(new MultiplicationNode({
            scalar,
            Partial(scalar->Argument(0), with_respect_to_ptr)
        }));
    }
    // d/dx { ln { x^2 } } = { x^2 }^{-1} * d/dx { x^2 } = 2 * x * x^{-2} = 2 * x^{-1}
    else if (scalar->Type() == "LnNode") {
        return std::shared_ptr<MultiplicationNode>(new MultiplicationNode({
            std::shared_ptr<ExponentiationNode>(new ExponentiationNode({
                scalar->Argument(0),
                std::shared_ptr<ConstantNode>(new ConstantNode(-1.0))
            })),
            Partial(scalar->Argument(0), with_respect_to_ptr)
        }));
    }
    else if (scalar->Type() == "SinNode") {
        return std::shared_ptr<MultiplicationNode>(new MultiplicationNode({
            std::shared_ptr<CosNode>(new CosNode({
                scalar->Argument(0)
            })),
            Partial(scalar->Argument(0), with_respect_to_ptr)
        }));
    }
    else if (scalar->Type() == "CosNode") {
        return std::shared_ptr<MultiplicationNode>(new MultiplicationNode({
            std::shared_ptr<MultiplicationNode>(new MultiplicationNode({
                std::shared_ptr<ConstantNode>(new ConstantNode(-1.0)),
                std::shared_ptr<SinNode>(new SinNode({
                    scalar->Argument(0)
                }))
            })),
            Partial(scalar->Argument(0), with_respect_to_ptr)
        }));
    }
    else {
        return std::shared_ptr<ConstantNode>(new ConstantNode(0.0));
    }
}

Matrix Calculus::Gradient(std::variant<Scalar, Matrix> const &node_variant, Scalar const &with_respect_to_x_ptr, Scalar const &with_respect_to_y_ptr, Scalar const &with_respect_to_z_ptr)
{
    if (!std::holds_alternative<Scalar>(node_variant)) {
        throw std::invalid_argument("Gradient only defined for Scalar");
    }

    Scalar scalar = std::get<Scalar>(node_variant);

    Matrix gradient_matrix(3, 1);

    gradient_matrix(0, 0) = Partial(scalar, with_respect_to_x_ptr);
    gradient_matrix(1, 0) = Partial(scalar, with_respect_to_y_ptr);
    gradient_matrix(2, 0) = Partial(scalar, with_respect_to_z_ptr);

    return gradient_matrix;
}

Scalar Calculus::Divergence(std::variant<Scalar, Matrix> const &node_variant, Scalar const &with_respect_to_x_ptr, Scalar const &with_respect_to_y_ptr, Scalar const &with_respect_to_z_ptr)
{
    if (!std::holds_alternative<Matrix>(node_variant)) {
        throw std::invalid_argument("Divergence only defined for Matrix");
    }

    Matrix matrix = std::get<Matrix>(node_variant);

    if (matrix.Rows() != 3 && matrix.Cols() != 1) {
        throw std::invalid_argument("Divergence requires a 3x1 matrix");
    }

    return std::shared_ptr<AdditionNode>(new AdditionNode({
        std::shared_ptr<AdditionNode>(new AdditionNode({
            Partial(matrix(0, 0), with_respect_to_x_ptr),
            Partial(matrix(1, 0), with_respect_to_y_ptr)
        })),
        Partial(matrix(2, 0), with_respect_to_z_ptr) 
    }));
}

Matrix Calculus::Curl(std::variant<Scalar, Matrix> const &node_variant, Scalar const &with_respect_to_x_ptr, Scalar const &with_respect_to_y_ptr, Scalar const &with_respect_to_z_ptr)
{
    if (!std::holds_alternative<Matrix>(node_variant)) {
        throw std::invalid_argument("Curl only defined for Matrix");
    }

    Matrix matrix = std::get<Matrix>(node_variant);

    if (matrix.Rows() != 3 && matrix.Cols() != 1) {
        throw std::invalid_argument("Divergence requires a 3x1 matrix");
    }

    Matrix curl_matrix(3, 1);

    curl_matrix(0, 0) = std::shared_ptr<SubtractionNode>(new SubtractionNode({ Partial(matrix(2, 0), with_respect_to_y_ptr), Partial(matrix(1, 0), with_respect_to_z_ptr) }));
    curl_matrix(1, 0) = std::shared_ptr<SubtractionNode>(new SubtractionNode({ Partial(matrix(0, 0), with_respect_to_z_ptr), Partial(matrix(2, 0), with_respect_to_x_ptr) }));
    curl_matrix(2, 0) = std::shared_ptr<SubtractionNode>(new SubtractionNode({ Partial(matrix(1, 0), with_respect_to_x_ptr), Partial(matrix(0, 0), with_respect_to_y_ptr) }));

    return curl_matrix;
}
