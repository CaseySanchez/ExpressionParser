/*
* Copyright 2020 Casey Sanchez
*/

#include "calculus.hpp"

Calculus::Calculus(std::shared_ptr<Node> const &node_ptr, std::map<std::string, std::shared_ptr<Node>> const &node_map) : m_node_ptr(node_ptr), m_node_map(node_map)
{
}

std::shared_ptr<Node> Calculus::Partial(std::shared_ptr<Node> const &with_respect_to_ptr)
{
    return Partial(m_node_ptr, with_respect_to_ptr);
}

std::shared_ptr<Node> Calculus::Partial(std::shared_ptr<Node> const &node_ptr, std::shared_ptr<Node> const &with_respect_to_ptr)
{
    std::cout << ExpressionVisualizer(node_ptr, m_node_map) << std::endl;

    // d/dx { x } = 1
    if (Node::Equivalent(node_ptr, with_respect_to_ptr)) {
        return std::shared_ptr<ConstantNode>(new ConstantNode(1.0));
    }
    // d/dx { 1 } = 0
    else if (node_ptr->Type() == "ConstantNode") {
        return std::shared_ptr<ConstantNode>(new ConstantNode(0.0));
    }
    // d/dx { y } = 0
    else if (node_ptr->Type() == "VariableNode") {
        return std::shared_ptr<ConstantNode>(new ConstantNode(0.0));
    }
    // d/dx { f(x) + g(x) } = f'(x) + g'(x)
    else if (node_ptr->Type() == "AdditionNode") {
        return std::shared_ptr<AdditionNode>(new AdditionNode({
            Partial(node_ptr->Argument(0), with_respect_to_ptr),
            Partial(node_ptr->Argument(1), with_respect_to_ptr)
        }));
    }
    // d/dx { f(x) - g(x) } = f'(x) - g'(x)
    else if (node_ptr->Type() == "SubtractionNode") {
        return std::shared_ptr<SubtractionNode>(new SubtractionNode({
            Partial(node_ptr->Argument(0), with_respect_to_ptr),
            Partial(node_ptr->Argument(1), with_respect_to_ptr)
        }));
    }
    // d/dx { f(x) * g(x) } = f'(x) * g(x) + g'(x) * f(x)
    else if (node_ptr->Type() == "MultiplicationNode") {
        return std::shared_ptr<AdditionNode>(new AdditionNode({
            std::shared_ptr<MultiplicationNode>(new MultiplicationNode({ 
                Partial(node_ptr->Argument(0), with_respect_to_ptr),
                node_ptr->Argument(1)
            })),
            std::shared_ptr<MultiplicationNode>(new MultiplicationNode({
                Partial(node_ptr->Argument(1), with_respect_to_ptr),
                node_ptr->Argument(0)
            }))
        }));
    }
    // d/dx { f(x) / g(x) } = { f'(x) * g(x) - g'(x) * f(x) } / { g(x) }^2
    else if (node_ptr->Type() == "DivisionNode") {
        return std::shared_ptr<DivisionNode>(new DivisionNode({
            std::shared_ptr<SubtractionNode>(new SubtractionNode({
                std::shared_ptr<MultiplicationNode>(new MultiplicationNode({ 
                    Partial(node_ptr->Argument(0), with_respect_to_ptr),
                    node_ptr->Argument(1)
                })),
                std::shared_ptr<MultiplicationNode>(new MultiplicationNode({
                    Partial(node_ptr->Argument(1), with_respect_to_ptr),
                    node_ptr->Argument(0)
                }))
            })),
            std::shared_ptr<ExponentiationNode>(new ExponentiationNode({
                node_ptr->Argument(1),
                std::shared_ptr<ConstantNode>(new ConstantNode(2.0))
            }))
        }));
    }
    // d/dx { x^2 } = d/dx { e^{ ln(x) * 2 } } = e^{ ln(x) * 2 } * d/dx { ln(x) * 2 } = 2 * x^2 * x^{-1} = 2 * x
    else if (node_ptr->Type() == "ExponentiationNode") {
        return Partial(std::shared_ptr<ExpNode>(new ExpNode({
                std::shared_ptr<MultiplicationNode>(new MultiplicationNode({ 
                    std::shared_ptr<LnNode>(new LnNode({ node_ptr->Argument(0) })),
                    node_ptr->Argument(1)
                }))
            })), with_respect_to_ptr);
    }
    // d/dx { e^{ x^2 } } = e^{ x^2 } * d/dx { x^2 } = e^{ x^2 } * 2 * x
    else if (node_ptr->Type() == "ExpNode") {
        return std::shared_ptr<MultiplicationNode>(new MultiplicationNode({
            node_ptr,
            Partial(node_ptr->Argument(0), with_respect_to_ptr)
        }));
    }
    // d/dx { ln { x^2 } } = { x^2 }^{-1} * d/dx { x^2 } = 2 * x * x^{-2} = 2 * x^{-1}
    else if (node_ptr->Type() == "LnNode") {
        return std::shared_ptr<MultiplicationNode>(new MultiplicationNode({
            std::shared_ptr<ExponentiationNode>(new ExponentiationNode({
                node_ptr->Argument(0),
                std::shared_ptr<ConstantNode>(new ConstantNode(-1.0))
            })),
            Partial(node_ptr->Argument(0), with_respect_to_ptr)
        }));
    }
    else if (node_ptr->Type() == "SinNode") {
        return std::shared_ptr<MultiplicationNode>(new MultiplicationNode({
            std::shared_ptr<CosNode>(new CosNode({
                node_ptr->Argument(0)
            })),
            Partial(node_ptr->Argument(0), with_respect_to_ptr)
        }));
    }
    else if (node_ptr->Type() == "CosNode") {
        return std::shared_ptr<MultiplicationNode>(new MultiplicationNode({
            std::shared_ptr<MultiplicationNode>(new MultiplicationNode({
                std::shared_ptr<ConstantNode>(new ConstantNode(-1.0)),
                std::shared_ptr<SinNode>(new SinNode({
                    node_ptr->Argument(0)
                }))
            })),
            Partial(node_ptr->Argument(0), with_respect_to_ptr)
        }));
    }
    else {
        return std::shared_ptr<ConstantNode>(new ConstantNode(0.0));
    }
}

std::shared_ptr<Node> Calculus::Gradient(std::shared_ptr<Node> const &node_ptr, std::shared_ptr<Node> const &with_respect_to_x_ptr, std::shared_ptr<Node> const &with_respect_to_y_ptr, std::shared_ptr<Node> const &with_respect_to_z_ptr)
{
    Matrix gradient_matrix(3, 1);

    gradient_matrix(0, 0) = Partial(node_ptr, with_respect_to_x_ptr);
    gradient_matrix(1, 0) = Partial(node_ptr, with_respect_to_y_ptr);
    gradient_matrix(2, 0) = Partial(node_ptr, with_respect_to_z_ptr);

    return std::shared_ptr<MatrixNode>(new MatrixNode(gradient_matrix));
}

std::shared_ptr<Node> Calculus::Divergence(std::shared_ptr<Node> const &node_ptr, std::shared_ptr<Node> const &with_respect_to_x_ptr, std::shared_ptr<Node> const &with_respect_to_y_ptr, std::shared_ptr<Node> const &with_respect_to_z_ptr)
{
    if (node_ptr->Type() != "MatrixNode") {
        throw std::invalid_argument("Divergence requires a MatrixNode");
    }

    std::shared_ptr<MatrixNode> matrix_ptr = std::dynamic_pointer_cast<MatrixNode>(node_ptr);

    Matrix matrix = matrix_ptr->MatrixValue();

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

std::shared_ptr<Node> Calculus::Curl(std::shared_ptr<Node> const &node_ptr, std::shared_ptr<Node> const &with_respect_to_x_ptr, std::shared_ptr<Node> const &with_respect_to_y_ptr, std::shared_ptr<Node> const &with_respect_to_z_ptr)
{
    if (node_ptr->Type() != "MatrixNode") {
        throw std::invalid_argument("Curl requires a MatrixNode");
    }

    std::shared_ptr<MatrixNode> matrix_ptr = std::dynamic_pointer_cast<MatrixNode>(node_ptr);

    Matrix matrix = matrix_ptr->MatrixValue();

    if (matrix.Rows() != 3 && matrix.Cols() != 1) {
        throw std::invalid_argument("Curl requires a 3x1 matrix");
    }

    Matrix curl_matrix(3, 1);

    curl_matrix(0, 0) = std::shared_ptr<SubtractionNode>(new SubtractionNode({ Partial(matrix(2, 0), with_respect_to_y_ptr), Partial(matrix(1, 0), with_respect_to_z_ptr) }));
    curl_matrix(1, 0) = std::shared_ptr<SubtractionNode>(new SubtractionNode({ Partial(matrix(0, 0), with_respect_to_z_ptr), Partial(matrix(2, 0), with_respect_to_x_ptr) }));
    curl_matrix(2, 0) = std::shared_ptr<SubtractionNode>(new SubtractionNode({ Partial(matrix(1, 0), with_respect_to_x_ptr), Partial(matrix(0, 0), with_respect_to_y_ptr) }));

    return std::shared_ptr<MatrixNode>(new MatrixNode(curl_matrix));
}
