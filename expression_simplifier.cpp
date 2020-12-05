/*
 * Copyright 2020 Casey Sanchez
 */

#include "expression_simplifier.hpp"

ExpressionSimplifier::ExpressionSimplifier(std::shared_ptr<Node> const &node_ptr, std::map<std::string, std::shared_ptr<Node>> const &node_map) : m_node_ptr(node_ptr), m_node_map(node_map)
{
}

std::shared_ptr<Node> ExpressionSimplifier::Identify()
{
    return Identify(m_node_ptr);
}

std::shared_ptr<Node> ExpressionSimplifier::Identify(std::shared_ptr<Node> const &node_ptr)
{
    for (auto &argument : node_ptr->Arguments()) {
        argument = Identify(argument);
    }

    if (m_node_ptr->Type() == "MatrixNode") {
        Matrix matrix = m_node_ptr->MatrixValue();

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = Identify(matrix(i, j));
            }
        }

        return std::shared_ptr<MatrixNode>(new MatrixNode(matrix));
    }
    else {
        if (node_ptr->Type() == "ExponentiationNode") {
            if (node_ptr->Argument(1)->Type() == "ConstantNode") {
                if (Approximately(node_ptr->Argument(1)->ComplexValue(), 0.0)) {
                    return std::shared_ptr<ConstantNode>(new ConstantNode(1.0));
                }
                else if (Approximately(node_ptr->Argument(1)->ComplexValue(), 1.0)) {
                    return node_ptr->Argument(0);
                }
                else if (node_ptr->Argument(0)->Type() == "ConstantNode") {
                    return std::shared_ptr<ConstantNode>(new ConstantNode(std::pow(node_ptr->Argument(0)->ComplexValue(), node_ptr->Argument(1)->ComplexValue())));
                }
            }
        }
        else if (node_ptr->Type() == "MultiplicationNode") {
            if (node_ptr->Argument(0)->Type() == "ConstantNode") {
                if (Approximately(node_ptr->Argument(0)->ComplexValue(), 0.0)) {
                    return std::shared_ptr<ConstantNode>(new ConstantNode(0.0));
                }
                else if (Approximately(node_ptr->Argument(0)->ComplexValue(), 1.0)) {
                    return node_ptr->Argument(1);
                }
                else if (node_ptr->Argument(1)->Type() == "ConstantNode") {
                    return std::shared_ptr<ConstantNode>(new ConstantNode(node_ptr->Argument(0)->ComplexValue() * node_ptr->Argument(1)->ComplexValue()));
                }
            }
            else if (node_ptr->Argument(1)->Type() == "ConstantNode") {
                if (Approximately(node_ptr->Argument(1)->ComplexValue(), 0.0)) {
                    return std::shared_ptr<ConstantNode>(new ConstantNode(0.0));
                }
                else if (Approximately(node_ptr->Argument(1)->ComplexValue(), 1.0)) {
                    return node_ptr->Argument(0);
                }
            }
        }
        else if (node_ptr->Type() == "DivisionNode") {
            if (node_ptr->Argument(0)->Type() == "ConstantNode") {
                if (Approximately(node_ptr->Argument(0)->ComplexValue(), 0.0)) {
                    return std::shared_ptr<ConstantNode>(new ConstantNode(0.0));
                }
                else if (node_ptr->Argument(1)->Type() == "ConstantNode") {
                    return std::shared_ptr<ConstantNode>(new ConstantNode(node_ptr->Argument(0)->ComplexValue() / node_ptr->Argument(1)->ComplexValue()));
                }
            }
            else if (node_ptr->Argument(1)->Type() == "ConstantNode") {
                if (Approximately(node_ptr->Argument(1)->ComplexValue(), 1.0)) {
                    return node_ptr->Argument(0);
                }
            }

            if (node_ptr->Argument(1)->Type() == "ExponentiationNode") {
                if (node_ptr->Argument(1)->Argument(1)->Type() == "ConstantNode") {
                    return std::shared_ptr<MultiplicationNode>(new MultiplicationNode({ node_ptr->Argument(0), std::shared_ptr<ExponentiationNode>(new ExponentiationNode({ node_ptr->Argument(1)->Argument(0), std::shared_ptr<ConstantNode>(new ConstantNode(-1.0 * node_ptr->Argument(1)->Argument(1)->ComplexValue())) })) }));
                }
            }
        }
        else if (node_ptr->Type() == "AdditionNode") {
            if (node_ptr->Argument(0)->Type() == "ConstantNode") {
                if (Approximately(node_ptr->Argument(0)->ComplexValue(), 0.0)) {
                    return node_ptr->Argument(1);
                }
                else if (node_ptr->Argument(1)->Type() == "ConstantNode") {
                    return std::shared_ptr<ConstantNode>(new ConstantNode(node_ptr->Argument(0)->ComplexValue() + node_ptr->Argument(1)->ComplexValue()));
                }
            }
            else if (node_ptr->Argument(1)->Type() == "ConstantNode") {
                if (Approximately(node_ptr->Argument(1)->ComplexValue(), 0.0)) {
                    return node_ptr->Argument(0);
                }
            }
        }
        else if (node_ptr->Type() == "SubtractionNode") {
            if (node_ptr->Argument(0)->Type() == "ConstantNode") {
                if (Approximately(node_ptr->Argument(0)->ComplexValue(), 0.0)) {
                    return std::shared_ptr<MultiplicationNode>(new MultiplicationNode({ std::shared_ptr<ConstantNode>(new ConstantNode(-1.0)), node_ptr->Argument(1) }));
                }
                else if (node_ptr->Argument(1)->Type() == "ConstantNode") {
                    return std::shared_ptr<ConstantNode>(new ConstantNode(node_ptr->Argument(0)->ComplexValue() - node_ptr->Argument(1)->ComplexValue()));
                }
            }
            else if (node_ptr->Argument(1)->Type() == "ConstantNode") {
                if (Approximately(node_ptr->Argument(1)->ComplexValue(), 0.0)) {
                    return node_ptr->Argument(0);
                }
            }
            else {
                return std::shared_ptr<AdditionNode>(new AdditionNode({ node_ptr->Argument(0), std::shared_ptr<MultiplicationNode>(new MultiplicationNode({ std::shared_ptr<ConstantNode>(new ConstantNode(-1.0)), node_ptr->Argument(1) })) }));
            }
        }
        else if (node_ptr->Type() == "SinNode") {
            if (node_ptr->Argument(0)->Type() == "ConstantNode") {
                return std::shared_ptr<ConstantNode>(new ConstantNode(std::sin(node_ptr->Argument(0)->ComplexValue())));
            }
        }
        else if (node_ptr->Type() == "CosNode") {
            if (node_ptr->Argument(0)->Type() == "ConstantNode") {
                return std::shared_ptr<ConstantNode>(new ConstantNode(std::cos(node_ptr->Argument(0)->ComplexValue())));
            }
        }
        else if (node_ptr->Type() == "TanNode") {
            if (node_ptr->Argument(0)->Type() == "ConstantNode") {
                return std::shared_ptr<ConstantNode>(new ConstantNode(std::tan(node_ptr->Argument(0)->ComplexValue())));
            }
        }
        else if (node_ptr->Type() == "AsinNode") {
            if (node_ptr->Argument(0)->Type() == "ConstantNode") {
                return std::shared_ptr<ConstantNode>(new ConstantNode(std::asin(node_ptr->Argument(0)->ComplexValue())));
            }
        }
        else if (node_ptr->Type() == "AcosNode") {
            if (node_ptr->Argument(0)->Type() == "ConstantNode") {
                return std::shared_ptr<ConstantNode>(new ConstantNode(std::acos(node_ptr->Argument(0)->ComplexValue())));
            }
        }
        else if (node_ptr->Type() == "AtanNode") {
            if (node_ptr->Argument(0)->Type() == "ConstantNode") {
                return std::shared_ptr<ConstantNode>(new ConstantNode(std::atan(node_ptr->Argument(0)->ComplexValue())));
            }
        }
        else if (node_ptr->Type() == "SqrtNode") {
            if (node_ptr->Argument(0)->Type() == "ConstantNode") {
                return std::shared_ptr<ConstantNode>(new ConstantNode(std::sqrt(node_ptr->Argument(0)->ComplexValue())));
            }
        }
        else if (node_ptr->Type() == "AbsNode") {
            if (node_ptr->Argument(0)->Type() == "ConstantNode") {
                return std::shared_ptr<ConstantNode>(new ConstantNode(std::abs(node_ptr->Argument(0)->ComplexValue())));
            }
        }
        else if (node_ptr->Type() == "Exp") {
            if (node_ptr->Argument(0)->Type() == "ConstantNode") {
                return std::shared_ptr<ConstantNode>(new ConstantNode(std::exp(node_ptr->Argument(0)->ComplexValue())));
            }
        }
        else if (node_ptr->Type() == "Log") {
            if (node_ptr->Argument(0)->Type() == "ConstantNode") {
                return std::shared_ptr<ConstantNode>(new ConstantNode(std::log(node_ptr->Argument(0)->ComplexValue())));
            }
        }

        return node_ptr;
    }
}

std::shared_ptr<Node> ExpressionSimplifier::Distribute()
{
    return Distribute(m_node_ptr);
}

std::shared_ptr<Node> ExpressionSimplifier::Distribute(std::shared_ptr<Node> const &node_ptr)
{
    for (auto &argument : node_ptr->Arguments()) {
        argument = Distribute(argument);
    }

    if (m_node_ptr->Type() == "MatrixNode") {
        Matrix matrix = m_node_ptr->MatrixValue();

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = Distribute(matrix(i, j));
            }
        }

        return std::shared_ptr<MatrixNode>(new MatrixNode(matrix));
    }
    else {
        std::vector<std::shared_ptr<Node>> factors = Factors(node_ptr);

        if (factors.size() > 0) {
            std::shared_ptr<Node> distributed_ptr = factors.front();

            for (std::shared_ptr<Node> const &factor : std::vector<std::shared_ptr<Node>>(std::next(std::cbegin(factors)), std::cend(factors))) {
                std::vector<std::shared_ptr<Node>> lhs_addends = Addends(distributed_ptr);
                std::vector<std::shared_ptr<Node>> rhs_addends = Addends(factor);       

                for (auto lhs_addend_it = std::cbegin(lhs_addends); lhs_addend_it != std::cend(lhs_addends); ++lhs_addend_it) {
                    for (auto rhs_addend_it = std::cbegin(rhs_addends); rhs_addend_it != std::cend(rhs_addends); ++rhs_addend_it) {
                        std::shared_ptr<MultiplicationNode> multiplication_ptr(new MultiplicationNode({ *lhs_addend_it, *rhs_addend_it }));

                        if (lhs_addend_it == std::cbegin(lhs_addends) && rhs_addend_it == std::cbegin(rhs_addends)) {
                            distributed_ptr = multiplication_ptr;
                        }
                        else {
                            distributed_ptr = std::shared_ptr<AdditionNode>(new AdditionNode({ distributed_ptr, multiplication_ptr }));
                        }
                    }
                }
            }

            return distributed_ptr;
        }

        return node_ptr;
    }
}

std::shared_ptr<Node> ExpressionSimplifier::CombineFactors()
{
    return CombineFactors(m_node_ptr);
}

std::shared_ptr<Node> ExpressionSimplifier::CombineFactors(std::shared_ptr<Node> const &node_ptr)
{
    for (auto &argument : node_ptr->Arguments()) {
        argument = CombineFactors(argument);
    }

    if (node_ptr->Type() == "MatrixNode") {
        Matrix matrix = node_ptr->MatrixValue();

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = CombineFactors(matrix(i, j));
            }
        }

        return std::shared_ptr<MatrixNode>(new MatrixNode(matrix));
    }
    else {
        std::vector<std::shared_ptr<Node>> factors = Factors(node_ptr);

        if (factors.size() > 0) {
            std::vector<std::shared_ptr<Node>> constants;
            std::vector<std::shared_ptr<Node>> variables;

            // Separate the factors into their constants and their variables
            std::partition_copy(std::cbegin(factors), std::cend(factors), std::back_inserter(constants), std::back_inserter(variables), [](std::shared_ptr<Node> const &factor_ptr) -> bool { return factor_ptr->Type() == "ConstantNode"; });

            if (!variables.empty()) {
                // Assess factors with O(n*(n-1)/2) complexity
                for (auto lhs_variable_it = std::begin(variables); lhs_variable_it != std::end(variables); ++lhs_variable_it) {
                    for (auto rhs_variable_it = std::next(lhs_variable_it); rhs_variable_it != std::end(variables); ++rhs_variable_it) {
                        std::shared_ptr<Node> lhs_variable;
                        std::shared_ptr<Node> rhs_variable;
                        
                        // Extract the variable of interest; 
                        // if the node is of type "ExponentiationNode" we want Argument(0) 
                        if ((*lhs_variable_it)->Type() == "ExponentiationNode") {
                            lhs_variable = (*lhs_variable_it)->Argument(0);
                        }
                        else {
                            lhs_variable = (*lhs_variable_it);
                        }
                        
                        if ((*rhs_variable_it)->Type() == "ExponentiationNode") {
                            rhs_variable = (*rhs_variable_it)->Argument(0);
                        }
                        else {
                            rhs_variable = (*rhs_variable_it);
                        }

                        // If the variables are equivalent sum their degrees
                        if (Node::Equivalent(lhs_variable, rhs_variable)) {
                            std::shared_ptr<Node> lhs_degree;
                            std::shared_ptr<Node> rhs_degree;
                            
                            // If the variable is of type "ExponentiationNode" then our degree is Argument(1)
                            // otherwise we have an implied degree of 1
                            if ((*lhs_variable_it)->Type() == "ExponentiationNode") {
                                lhs_degree = (*lhs_variable_it)->Argument(1);
                            }
                            else {
                                lhs_degree = std::shared_ptr<ConstantNode>(new ConstantNode(1.0));
                            }
                            
                            if ((*rhs_variable_it)->Type() == "ExponentiationNode") {
                                rhs_degree = (*rhs_variable_it)->Argument(1);
                            }
                            else {
                                rhs_degree = std::shared_ptr<ConstantNode>(new ConstantNode(1.0));
                            }

                            // Add their degrees and exponentiate
                            *lhs_variable_it = std::shared_ptr<ExponentiationNode>(new ExponentiationNode({ lhs_variable, std::shared_ptr<AdditionNode>(new AdditionNode({ lhs_degree, rhs_degree })) }));

                            // Remove the old variable
                            rhs_variable_it = variables.erase(rhs_variable_it);

                            if (rhs_variable_it == std::end(variables)) {
                                break;
                            }
                        }
                    }
                }

                // Reduce the factors to a single combined node via repeated MultiplicationNode nodes
                std::shared_ptr<Node> combined_factors = std::reduce(std::next(std::cbegin(variables)), std::cend(variables), variables.front(), 
                    [](std::shared_ptr<Node> const &combined_factors, std::shared_ptr<Node> const &combined_factor) { 
                        return std::shared_ptr<MultiplicationNode>(new MultiplicationNode({ combined_factors, combined_factor })); 
                    });
                
                // Reduce the constants to a single value via repeated multiplication
                if (!constants.empty()) {
                    std::complex<double> coefficient = std::transform_reduce(std::cbegin(constants), std::cend(constants), std::complex<double>(1.0, 0.0), 
                        [](std::complex<double> const &product_coefficient, std::complex<double> const &coefficient) -> std::complex<double> { 
                            return product_coefficient * coefficient; 
                        }, [](std::shared_ptr<Node> const &coefficient) -> std::complex<double> { 
                            return coefficient->ComplexValue(); 
                        });

                    return std::shared_ptr<MultiplicationNode>(new MultiplicationNode({ std::shared_ptr<ConstantNode>(new ConstantNode(coefficient)), combined_factors}));
                }
                else {
                    return combined_factors;
                }
            }
        }

        return node_ptr;
    }
}

std::shared_ptr<Node> ExpressionSimplifier::CombineAddends()
{
    return CombineAddends(m_node_ptr);
}

std::shared_ptr<Node> ExpressionSimplifier::CombineAddends(std::shared_ptr<Node> const &node_ptr)
{
    for (auto &argument : node_ptr->Arguments()) {
        argument = CombineAddends(argument);
    }

    if (m_node_ptr->Type() == "MatrixNode") {
        Matrix matrix = m_node_ptr->MatrixValue();

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = CombineAddends(matrix(i, j));
            }
        }

        return std::shared_ptr<MatrixNode>(new MatrixNode(matrix));
    }
    else {
        std::vector<std::shared_ptr<Node>> addends = Addends(node_ptr);

        if (addends.size() > 0) {
            // Assess addends with O(n*(n-1)/2) complexity
            for (auto addend_lhs_it = std::begin(addends); addend_lhs_it != std::end(addends); ++addend_lhs_it) {
                for (auto addend_rhs_it = std::next(addend_lhs_it); addend_rhs_it != std::end(addends); ++addend_rhs_it) {                
                    // Extract the factors from the lhs addend and the rhs addend                
                    auto lhs_factors = Factors(*addend_lhs_it);
                    auto rhs_factors = Factors(*addend_rhs_it);

                    // Separate the factors into their constants and their variables
                    std::vector<std::shared_ptr<Node>> lhs_constants;
                    std::vector<std::shared_ptr<Node>> rhs_constants;

                    std::vector<std::shared_ptr<Node>> lhs_variables;
                    std::vector<std::shared_ptr<Node>> rhs_variables;

                    std::partition_copy(std::cbegin(lhs_factors), std::cend(lhs_factors), std::back_inserter(lhs_constants), std::back_inserter(lhs_variables), [](std::shared_ptr<Node> const &factor_ptr) -> bool { return factor_ptr->Type() == "ConstantNode"; });
                    std::partition_copy(std::cbegin(rhs_factors), std::cend(rhs_factors), std::back_inserter(rhs_constants), std::back_inserter(rhs_variables), [](std::shared_ptr<Node> const &factor_ptr) -> bool { return factor_ptr->Type() == "ConstantNode"; });

                    if (lhs_variables.size() > 0 && rhs_variables.size() > 0 && lhs_variables.size() == rhs_variables.size()) {
                        // Check if all of the variables are equivalent
                        bool all_of = true;

                        for (auto lhs_variable : lhs_variables) {
                            all_of = all_of && std::find_if(std::cbegin(rhs_variables), std::cend(rhs_variables), [&lhs_variable](std::shared_ptr<Node> const &rhs_variable) -> bool { return Node::Equivalent(lhs_variable, rhs_variable); }) != std::cend(rhs_variables);
                        }

                        if (all_of) {
                            // If there are multiple constants per term, multiply them together and increment the coefficient                    
                            std::complex<double> coefficient;
                            
                            if (lhs_constants.empty()) {
                                coefficient += std::complex<double>(1.0, 0.0);
                            }
                            else {
                                coefficient += std::transform_reduce(std::cbegin(lhs_constants), std::cend(lhs_constants), std::complex<double>(1.0, 0.0), 
                                    [](std::complex<double> const &product_coefficient, std::complex<double> const &coefficient) -> std::complex<double> { 
                                        return product_coefficient * coefficient; 
                                    }, [](std::shared_ptr<Node> const &lhs_coefficient) -> std::complex<double> { 
                                        return lhs_coefficient->ComplexValue(); 
                                    });
                            }

                            if (rhs_constants.empty()) {
                                coefficient += std::complex<double>(1.0, 0.0);
                            }
                            else {
                                coefficient += std::transform_reduce(std::cbegin(rhs_constants), std::cend(rhs_constants), std::complex<double>(1.0, 0.0), 
                                    [](std::complex<double> const &product_coefficient, std::complex<double> const &coefficient) -> std::complex<double> { 
                                        return product_coefficient * coefficient; 
                                    }, [](std::shared_ptr<Node> const &rhs_coefficient) -> std::complex<double> { 
                                        return rhs_coefficient->ComplexValue(); 
                                    });
                            }

                            // Recombining the variables via repeated MultiplicationNode
                            std::shared_ptr<Node> combined_variables = std::reduce(std::next(std::cbegin(lhs_variables)), std::cend(lhs_variables), lhs_variables.front(), 
                                [](std::shared_ptr<Node> const &combined_variables, std::shared_ptr<Node> const &combined_variable) -> std::shared_ptr<Node> { 
                                    return std::shared_ptr<MultiplicationNode>(new MultiplicationNode({ combined_variables, combined_variable })); 
                                });

                            // Multiply by the coefficient
                            *addend_lhs_it = std::shared_ptr<MultiplicationNode>(new MultiplicationNode({ std::shared_ptr<ConstantNode>(new ConstantNode(coefficient)), combined_variables }));

                            // Remove the old term
                            addend_rhs_it = addends.erase(addend_rhs_it);

                            // Make sure we don't segfault by attempting to access out-of-bounds memory!
                            if (addend_rhs_it == std::end(addends)) {
                                break;
                            }
                        }
                    }
                    else if (lhs_constants.size() > 0 && rhs_constants.size() > 0 && lhs_variables.size() == 0 && rhs_variables.size() == 0) {
                        // Sum the constants
                        std::complex<double> combined_constants;
                        
                        for (auto lhs_constant : lhs_constants) {
                            combined_constants += lhs_constant->ComplexValue();
                        }

                        for (auto rhs_constant : rhs_constants) {
                            combined_constants += rhs_constant->ComplexValue();
                        }

                        *addend_lhs_it = std::shared_ptr<ConstantNode>(new ConstantNode(combined_constants));

                        // Remove the old term
                        addend_rhs_it = addends.erase(addend_rhs_it);

                        // Make sure we don't segfault by attempting to access out-of-bounds memory!
                        if (addend_rhs_it == std::end(addends)) {
                            break;
                        }
                    }
                }
            }

            // Sum the addends for the final result
            std::shared_ptr<Node> combined_addends = std::reduce(std::next(std::cbegin(addends)), std::cend(addends), addends.front(), 
                [](std::shared_ptr<Node> const &combined_addends, std::shared_ptr<Node> const &combined_addend) { 
                    return std::shared_ptr<AdditionNode>(new AdditionNode({ combined_addends, combined_addend })); 
                });

            return combined_addends;
        }

        return node_ptr;
    }
}

std::vector<std::shared_ptr<Node>> ExpressionSimplifier::Factors(std::shared_ptr<Node> const &node_ptr)
{
    std::vector<std::shared_ptr<Node>> factors;

    Factors(factors, node_ptr);

    return factors;
}

void ExpressionSimplifier::Factors(std::vector<std::shared_ptr<Node>> &factors, std::shared_ptr<Node> const &node_ptr)
{
    if (node_ptr->Type() == "MultiplicationNode") {
        Factors(factors, node_ptr->Argument(0));
        Factors(factors, node_ptr->Argument(1));
    }
    else {
        factors.emplace_back(node_ptr);
    }
}

std::vector<std::shared_ptr<Node>> ExpressionSimplifier::Addends(std::shared_ptr<Node> const &node_ptr)
{
    std::vector<std::shared_ptr<Node>> addends;

    Addends(addends, node_ptr);

    return addends;
}

void ExpressionSimplifier::Addends(std::vector<std::shared_ptr<Node>> &addends, std::shared_ptr<Node> const &node_ptr)
{
    if (node_ptr->Type() == "AdditionNode") {
        Addends(addends, node_ptr->Argument(0));
        Addends(addends, node_ptr->Argument(1));
    }
    else {
        addends.emplace_back(node_ptr);
    }
}