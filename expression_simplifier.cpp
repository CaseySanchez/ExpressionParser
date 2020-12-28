/*
 * Copyright 2020 Casey Sanchez
 */

#include "expression_simplifier.hpp"

ExpressionSimplifier::ExpressionSimplifier(std::variant<Scalar, Matrix> const &node_variant, std::map<std::string, std::variant<Scalar, Matrix>> const &node_map) : m_node_variant(node_variant), m_node_map(node_map)
{
}

std::variant<Scalar, Matrix> ExpressionSimplifier::Simplify()
{
    std::variant<Scalar, Matrix> distributed_ptr = ExpressionSimplifier(m_node_variant, m_node_map).Distribute();
    std::variant<Scalar, Matrix> combined_factors_ptr = ExpressionSimplifier(distributed_ptr, m_node_map).CombineFactors();
    std::variant<Scalar, Matrix> combined_addends_ptr = ExpressionSimplifier(combined_factors_ptr, m_node_map).CombineAddends();
    std::variant<Scalar, Matrix> factorized_ptr = ExpressionSimplifier(combined_addends_ptr, m_node_map).Factorize();

    return factorized_ptr;
}

std::variant<Scalar, Matrix> ExpressionSimplifier::Identify()
{
    return Identify(m_node_variant);
}

std::variant<Scalar, Matrix> ExpressionSimplifier::Identify(std::variant<Scalar, Matrix> const &node_variant)
{
    if (std::holds_alternative<Matrix>(node_variant)) {
        Matrix matrix = std::get<Matrix>(node_variant);

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = std::get<Scalar>(Identify(matrix(i, j)));
            }
        }

        return matrix;
    }
    else if (std::holds_alternative<Scalar>(node_variant)) {
        Scalar scalar = std::get<Scalar>(node_variant);

        for (auto &argument : scalar->Arguments()) {
            argument = std::get<Scalar>(Identify(argument));
        }
        
        if (scalar->Type() == "ExponentiationNode") {
            if (scalar->Argument(1)->Type() == "ConstantNode") {
                if (Approximately(scalar->Argument(1)->Value(), 0.0)) {
                    return Scalar(new ConstantNode(1.0));
                }
                else if (Approximately(scalar->Argument(1)->Value(), 1.0)) {
                    return scalar->Argument(0);
                }
                else if (scalar->Argument(0)->Type() == "ConstantNode") {
                    return Scalar(new ConstantNode(std::pow(scalar->Argument(0)->Value(), scalar->Argument(1)->Value())));
                }
            }
        }
        else if (scalar->Type() == "MultiplicationNode") {
            if (scalar->Argument(0)->Type() == "ConstantNode") {
                if (Approximately(scalar->Argument(0)->Value(), 0.0)) {
                    return Scalar(new ConstantNode(0.0));
                }
                else if (Approximately(scalar->Argument(0)->Value(), 1.0)) {
                    return scalar->Argument(1);
                }
                else if (scalar->Argument(1)->Type() == "ConstantNode") {
                    return Scalar(new ConstantNode(scalar->Argument(0)->Value() * scalar->Argument(1)->Value()));
                }
            }
            else if (scalar->Argument(1)->Type() == "ConstantNode") {
                if (Approximately(scalar->Argument(1)->Value(), 0.0)) {
                    return Scalar(new ConstantNode(0.0));
                }
                else if (Approximately(scalar->Argument(1)->Value(), 1.0)) {
                    return scalar->Argument(0);
                }
            }
        }
        else if (scalar->Type() == "DivisionNode") {
            if (scalar->Argument(0)->Type() == "ConstantNode") {
                if (Approximately(scalar->Argument(0)->Value(), 0.0)) {
                    return Scalar(new ConstantNode(0.0));
                }
                else if (scalar->Argument(1)->Type() == "ConstantNode") {
                    return Scalar(new ConstantNode(scalar->Argument(0)->Value() / scalar->Argument(1)->Value()));
                }
            }
            else if (scalar->Argument(1)->Type() == "ConstantNode") {
                if (Approximately(scalar->Argument(1)->Value(), 1.0)) {
                    return scalar->Argument(0);
                }
            }

            if (scalar->Argument(1)->Type() == "ExponentiationNode") {
                if (scalar->Argument(1)->Argument(1)->Type() == "ConstantNode") {
                    return Scalar(new MultiplicationNode({ scalar->Argument(0), Scalar(new ExponentiationNode({ scalar->Argument(1)->Argument(0), Scalar(new ConstantNode(-1.0 * scalar->Argument(1)->Argument(1)->Value())) })) }));
                }
            }
        }
        else if (scalar->Type() == "AdditionNode") {
            if (scalar->Argument(0)->Type() == "ConstantNode") {
                if (Approximately(scalar->Argument(0)->Value(), 0.0)) {
                    return scalar->Argument(1);
                }
                else if (scalar->Argument(1)->Type() == "ConstantNode") {
                    return Scalar(new ConstantNode(scalar->Argument(0)->Value() + scalar->Argument(1)->Value()));
                }
            }
            else if (scalar->Argument(1)->Type() == "ConstantNode") {
                if (Approximately(scalar->Argument(1)->Value(), 0.0)) {
                    return scalar->Argument(0);
                }
            }
        }
        else if (scalar->Type() == "SubtractionNode") {
            if (scalar->Argument(0)->Type() == "ConstantNode") {
                if (Approximately(scalar->Argument(0)->Value(), 0.0)) {
                    return Scalar(new MultiplicationNode({ Scalar(new ConstantNode(-1.0)), scalar->Argument(1) }));
                }
                else if (scalar->Argument(1)->Type() == "ConstantNode") {
                    return Scalar(new ConstantNode(scalar->Argument(0)->Value() - scalar->Argument(1)->Value()));
                }
            }
            else if (scalar->Argument(1)->Type() == "ConstantNode") {
                if (Approximately(scalar->Argument(1)->Value(), 0.0)) {
                    return scalar->Argument(0);
                }
            }
            else {
                return Scalar(new AdditionNode({ scalar->Argument(0), Scalar(new MultiplicationNode({ Scalar(new ConstantNode(-1.0)), scalar->Argument(1) })) }));
            }
        }
        else if (scalar->Type() == "SinNode") {
            if (scalar->Argument(0)->Type() == "ConstantNode") {
                return Scalar(new ConstantNode(std::sin(scalar->Argument(0)->Value())));
            }
        }
        else if (scalar->Type() == "CosNode") {
            if (scalar->Argument(0)->Type() == "ConstantNode") {
                return Scalar(new ConstantNode(std::cos(scalar->Argument(0)->Value())));
            }
        }
        else if (scalar->Type() == "TanNode") {
            if (scalar->Argument(0)->Type() == "ConstantNode") {
                return Scalar(new ConstantNode(std::tan(scalar->Argument(0)->Value())));
            }
        }
        else if (scalar->Type() == "AsinNode") {
            if (scalar->Argument(0)->Type() == "ConstantNode") {
                return Scalar(new ConstantNode(std::asin(scalar->Argument(0)->Value())));
            }
        }
        else if (scalar->Type() == "AcosNode") {
            if (scalar->Argument(0)->Type() == "ConstantNode") {
                return Scalar(new ConstantNode(std::acos(scalar->Argument(0)->Value())));
            }
        }
        else if (scalar->Type() == "AtanNode") {
            if (scalar->Argument(0)->Type() == "ConstantNode") {
                return Scalar(new ConstantNode(std::atan(scalar->Argument(0)->Value())));
            }
        }
        else if (scalar->Type() == "SqrtNode") {
            if (scalar->Argument(0)->Type() == "ConstantNode") {
                return Scalar(new ConstantNode(std::sqrt(scalar->Argument(0)->Value())));
            }
        }
        else if (scalar->Type() == "AbsNode") {
            if (scalar->Argument(0)->Type() == "ConstantNode") {
                return Scalar(new ConstantNode(std::abs(scalar->Argument(0)->Value())));
            }
        }
        else if (scalar->Type() == "ExpNode") {
            if (scalar->Argument(0)->Type() == "ConstantNode") {
                return Scalar(new ConstantNode(std::exp(scalar->Argument(0)->Value())));
            }
            else {
                std::vector<Scalar> factors = Factors(scalar->Argument(0));

                std::vector<Scalar> base_factors;
                std::vector<Scalar> exp_factors;

                std::partition_copy(std::cbegin(factors), std::cend(factors), std::back_inserter(base_factors), std::back_inserter(exp_factors), [](Scalar const &factor_ptr) -> bool { return factor_ptr->Type() == "LnNode"; });

                if (base_factors.size() > 0) {
                    Scalar base_ptr = std::reduce(std::next(std::cbegin(base_factors)), std::cend(base_factors), base_factors.front()->Argument(0), 
                        [](Scalar const &base_ptr, Scalar const &factor_ptr) { 
                            return Scalar(new AdditionNode({ base_ptr, factor_ptr->Argument(0) })); 
                        });

                    if (exp_factors.size() > 0) {
                        Scalar exp_ptr = std::reduce(std::next(std::cbegin(exp_factors)), std::cend(exp_factors), exp_factors.front(), 
                            [](Scalar const &exp_ptr, Scalar const &factor_ptr) { 
                                return Scalar(new MultiplicationNode({ exp_ptr, factor_ptr })); 
                            });

                        return Scalar(new ExponentiationNode({
                            base_ptr,
                            exp_ptr
                        }));
                    }

                    return base_ptr;
                }
            }
        }
        else if (scalar->Type() == "LnNode") {
            if (scalar->Argument(0)->Type() == "ConstantNode") {
                return Scalar(new ConstantNode(std::log(scalar->Argument(0)->Value())));
            }
        }

        return scalar;
    }

    throw std::invalid_argument("std::variant<Scalar, Matrix> holds neither");
}

std::variant<Scalar, Matrix> ExpressionSimplifier::Distribute()
{
    return Identify(Distribute(m_node_variant));
}

std::variant<Scalar, Matrix> ExpressionSimplifier::Distribute(std::variant<Scalar, Matrix> const &node_variant)
{
    if (std::holds_alternative<Matrix>(node_variant)) {
        Matrix matrix = std::get<Matrix>(node_variant);

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = std::get<Scalar>(Distribute(matrix(i, j)));
            }
        }

        return matrix;
    }
    else if (std::holds_alternative<Scalar>(node_variant)) {
        Scalar scalar = std::get<Scalar>(node_variant);

        for (auto &argument : scalar->Arguments()) {
            argument = std::get<Scalar>(Distribute(argument));
        }
        
        std::vector<Scalar> factors = Factors(scalar);

        if (factors.size() > 0) {
            Scalar distributed_ptr = factors.front();

            for (Scalar const &factor : std::vector<Scalar>(std::next(std::cbegin(factors)), std::cend(factors))) {
                std::vector<Scalar> lhs_addends = Addends(distributed_ptr);
                std::vector<Scalar> rhs_addends = Addends(factor);       

                for (auto lhs_addend_it = std::cbegin(lhs_addends); lhs_addend_it != std::cend(lhs_addends); ++lhs_addend_it) {
                    for (auto rhs_addend_it = std::cbegin(rhs_addends); rhs_addend_it != std::cend(rhs_addends); ++rhs_addend_it) {
                        Scalar multiplication_ptr(new MultiplicationNode({ 
                            *lhs_addend_it, 
                            *rhs_addend_it 
                        }));

                        if (lhs_addend_it == std::cbegin(lhs_addends) && rhs_addend_it == std::cbegin(rhs_addends)) {
                            distributed_ptr = multiplication_ptr;
                        }
                        else {
                            distributed_ptr = Scalar(new AdditionNode({ distributed_ptr, multiplication_ptr }));
                        }
                    }
                }
            }

            return distributed_ptr;
        }

        return scalar;
    }

    throw std::invalid_argument("std::variant<Scalar, Matrix> holds neither");
}

std::variant<Scalar, Matrix> ExpressionSimplifier::CombineFactors()
{
    return Identify(CombineFactors(m_node_variant));
}

std::variant<Scalar, Matrix> ExpressionSimplifier::CombineFactors(std::variant<Scalar, Matrix> const &node_variant)
{
    if (std::holds_alternative<Matrix>(node_variant)) {
        Matrix matrix = std::get<Matrix>(node_variant);

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = std::get<Scalar>(CombineFactors(matrix(i, j)));
            }
        }

        return matrix;
    }
    else if (std::holds_alternative<Scalar>(node_variant)) {
        Scalar scalar = std::get<Scalar>(node_variant);

        for (auto &argument : scalar->Arguments()) {
            argument = std::get<Scalar>(CombineFactors(argument));
        }
        
        std::vector<Scalar> factors = Factors(scalar);

        if (factors.size() > 0) {
            std::vector<Scalar> constants;
            std::vector<Scalar> variables;

            // Separate the factors into their constants and their variables
            std::partition_copy(std::cbegin(factors), std::cend(factors), std::back_inserter(constants), std::back_inserter(variables), [](Scalar const &factor_ptr) -> bool { return factor_ptr->Type() == "ConstantNode"; });

            if (!variables.empty()) {
                // Assess factors with O(n*(n-1)/2) complexity
                for (auto lhs_variable_it = std::begin(variables); lhs_variable_it != std::end(variables); ++lhs_variable_it) {
                    for (auto rhs_variable_it = std::next(lhs_variable_it); rhs_variable_it != std::end(variables); ++rhs_variable_it) {
                        Scalar lhs_variable;
                        Scalar rhs_variable;
                        
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
                            Scalar lhs_degree;
                            Scalar rhs_degree;
                            
                            // If the variable is of type "ExponentiationNode" then our degree is Argument(1)
                            // otherwise we have an implied degree of 1
                            if ((*lhs_variable_it)->Type() == "ExponentiationNode") {
                                lhs_degree = (*lhs_variable_it)->Argument(1);
                            }
                            else {
                                lhs_degree = Scalar(new ConstantNode(1.0));
                            }
                            
                            if ((*rhs_variable_it)->Type() == "ExponentiationNode") {
                                rhs_degree = (*rhs_variable_it)->Argument(1);
                            }
                            else {
                                rhs_degree = Scalar(new ConstantNode(1.0));
                            }

                            // Add their degrees and exponentiate
                            *lhs_variable_it = Scalar(new ExponentiationNode({ lhs_variable, Scalar(new AdditionNode({ lhs_degree, rhs_degree })) }));

                            // Remove the old variable
                            rhs_variable_it = variables.erase(rhs_variable_it);

                            if (rhs_variable_it == std::end(variables)) {
                                break;
                            }
                        }
                    }
                }

                // Reduce the factors to a single combined node via repeated MultiplicationNode nodes
                Scalar combined_factors = std::reduce(std::next(std::cbegin(variables)), std::cend(variables), variables.front(), 
                    [](Scalar const &combined_factors, Scalar const &combined_factor) { 
                        return Scalar(new MultiplicationNode({ combined_factors, combined_factor })); 
                    });
                
                // Reduce the constants to a single value via repeated multiplication
                if (!constants.empty()) {
                    std::complex<double> coefficient = std::transform_reduce(std::cbegin(constants), std::cend(constants), std::complex<double>(1.0, 0.0), 
                        [](std::complex<double> const &product_coefficient, std::complex<double> const &coefficient) -> std::complex<double> { 
                            return product_coefficient * coefficient; 
                        }, [](Scalar const &coefficient) -> std::complex<double> { 
                            return coefficient->Value(); 
                        });

                    return Scalar(new MultiplicationNode({ Scalar(new ConstantNode(coefficient)), combined_factors}));
                }
                else {
                    return combined_factors;
                }
            }
        }

        return scalar;
    }

    throw std::invalid_argument("std::variant<Scalar, Matrix> holds neither");
}

std::variant<Scalar, Matrix> ExpressionSimplifier::CombineAddends()
{
    return Identify(CombineAddends(m_node_variant));
}

std::variant<Scalar, Matrix> ExpressionSimplifier::CombineAddends(std::variant<Scalar, Matrix> const &node_variant)
{
    if (std::holds_alternative<Matrix>(node_variant)) {
        Matrix matrix = std::get<Matrix>(node_variant);

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = std::get<Scalar>(CombineAddends(matrix(i, j)));
            }
        }

        return matrix;
    }
    else if (std::holds_alternative<Scalar>(node_variant)) {
        Scalar scalar = std::get<Scalar>(node_variant);

        for (auto &argument : scalar->Arguments()) {
            argument = std::get<Scalar>(CombineAddends(argument));
        }

        std::vector<Scalar> addends = Addends(scalar);

        if (addends.size() > 0) {
            // Assess addends with O(n*(n-1)/2) complexity
            for (auto addend_lhs_it = std::begin(addends); addend_lhs_it != std::end(addends); ++addend_lhs_it) {
                for (auto addend_rhs_it = std::next(addend_lhs_it); addend_rhs_it != std::end(addends); ++addend_rhs_it) {                
                    // Extract the factors from the lhs addend and the rhs addend                
                    auto lhs_factors = Factors(*addend_lhs_it);
                    auto rhs_factors = Factors(*addend_rhs_it);

                    // Separate the factors into their constants and their variables
                    std::vector<Scalar> lhs_constants;
                    std::vector<Scalar> rhs_constants;

                    std::vector<Scalar> lhs_variables;
                    std::vector<Scalar> rhs_variables;

                    std::partition_copy(std::cbegin(lhs_factors), std::cend(lhs_factors), std::back_inserter(lhs_constants), std::back_inserter(lhs_variables), [](Scalar const &factor_ptr) -> bool { return factor_ptr->Type() == "ConstantNode"; });
                    std::partition_copy(std::cbegin(rhs_factors), std::cend(rhs_factors), std::back_inserter(rhs_constants), std::back_inserter(rhs_variables), [](Scalar const &factor_ptr) -> bool { return factor_ptr->Type() == "ConstantNode"; });

                    if (lhs_variables.size() > 0 && lhs_variables.size() == rhs_variables.size()) {
                        // Check if all of the variables are equivalent, std::equal will not work for this
                        auto compare_variables = [](std::vector<Scalar> lhs_variables, std::vector<Scalar> rhs_variables) -> bool { 
                            for (auto lhs_variable : lhs_variables) {
                                auto rhs_variable_it = std::find_if(std::cbegin(rhs_variables), std::cend(rhs_variables), 
                                    [&lhs_variable](Scalar const &rhs_variable) -> bool { 
                                        return Node::Equivalent(lhs_variable, rhs_variable); 
                                    });

                                if (rhs_variable_it == std::cend(rhs_variables)) {
                                    return false;
                                }

                                rhs_variables.erase(rhs_variable_it);
                            }

                            return true;
                        };

                        if (compare_variables(lhs_variables, rhs_variables)) {
                            // If there are multiple constants per term, multiply them together and increment the coefficient                    
                            std::complex<double> coefficient;
                            
                            if (lhs_constants.empty()) {
                                coefficient += std::complex<double>(1.0, 0.0);
                            }
                            else {
                                coefficient += std::transform_reduce(std::cbegin(lhs_constants), std::cend(lhs_constants), std::complex<double>(1.0, 0.0), 
                                    [](std::complex<double> const &product_coefficient, std::complex<double> const &coefficient) -> std::complex<double> { 
                                        return product_coefficient * coefficient; 
                                    }, [](Scalar const &lhs_coefficient) -> std::complex<double> { 
                                        return lhs_coefficient->Value(); 
                                    });
                            }

                            if (rhs_constants.empty()) {
                                coefficient += std::complex<double>(1.0, 0.0);
                            }
                            else {
                                coefficient += std::transform_reduce(std::cbegin(rhs_constants), std::cend(rhs_constants), std::complex<double>(1.0, 0.0), 
                                    [](std::complex<double> const &product_coefficient, std::complex<double> const &coefficient) -> std::complex<double> { 
                                        return product_coefficient * coefficient; 
                                    }, [](Scalar const &rhs_coefficient) -> std::complex<double> { 
                                        return rhs_coefficient->Value(); 
                                    });
                            }

                            // Recombining the variables via repeated MultiplicationNode
                            Scalar combined_variables = std::reduce(std::next(std::cbegin(lhs_variables)), std::cend(lhs_variables), lhs_variables.front(), 
                                [](Scalar const &combined_variables, Scalar const &combined_variable) -> Scalar { 
                                    return Scalar(new MultiplicationNode({ combined_variables, combined_variable })); 
                                });

                            // Multiply by the coefficient
                            *addend_lhs_it = Scalar(new MultiplicationNode({ std::shared_ptr<ConstantNode>(new ConstantNode(coefficient)), combined_variables }));

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
                            combined_constants += lhs_constant->Value();
                        }

                        for (auto rhs_constant : rhs_constants) {
                            combined_constants += rhs_constant->Value();
                        }

                        *addend_lhs_it = Scalar(new ConstantNode(combined_constants));

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
            Scalar combined_addends = std::reduce(std::next(std::cbegin(addends)), std::cend(addends), addends.front(), 
                [](Scalar const &combined_addends, Scalar const &combined_addend) { 
                    return Scalar(new AdditionNode({ combined_addends, combined_addend })); 
                });

            return combined_addends;
        }

        return scalar;
    }

    throw std::invalid_argument("std::variant<Scalar, Matrix> holds neither");
}

std::variant<Scalar, Matrix> ExpressionSimplifier::Factorize()
{
    return Identify(Factorize(m_node_variant));
}

std::variant<Scalar, Matrix> ExpressionSimplifier::Factorize(std::variant<Scalar, Matrix> const &node_variant)
{
    if (std::holds_alternative<Matrix>(node_variant)) {
        Matrix matrix = std::get<Matrix>(node_variant);

        for (size_t i = 0; i < matrix.Rows(); ++i) {
            for (size_t j = 0; j < matrix.Cols(); ++j) {
                matrix(i, j) = std::get<Scalar>(Factorize(matrix(i, j)));
            }
        }

        return matrix;
    }
    else if (std::holds_alternative<Scalar>(node_variant)) {
        Scalar scalar = std::get<Scalar>(node_variant);

        for (auto &argument : scalar->Arguments()) {
            argument = std::get<Scalar>(Factorize(argument));
        }

        // [ 2*w*x, 2*w*y, 2*w*z ]
        std::vector<Scalar> addends = Addends(scalar);

        // [ [ 2, w, x ], [ 2, w, y ], [ 2, w, z ] ]
        std::vector<std::vector<Scalar>> factors_of_addends;

        std::transform(std::cbegin(addends), std::cend(addends), std::back_inserter(factors_of_addends), 
            [](Scalar const &addend_ptr) -> std::vector<Scalar> { 
                return Factors(addend_ptr);
            });

        if (factors_of_addends.size() > 1) {
            // [ 2, w, x ] ∩ [ 2, w, y ] ∩ [ 2, w, z ] = [ 2, w ]
            std::vector<Scalar> common_factors = std::reduce(std::next(std::cbegin(factors_of_addends)), std::cend(factors_of_addends), factors_of_addends.front(), 
                [](std::vector<Scalar> const &common_factors, std::vector<Scalar> const &factors) -> std::vector<Scalar> {
                    std::vector<Scalar> factors_intersection;
                    
                    std::set_intersection(std::cbegin(factors), std::cend(factors), std::cbegin(common_factors), std::cend(common_factors), std::back_inserter(factors_intersection), 
                        [](Scalar const &lhs_ptr, Scalar const &rhs_ptr) -> bool { 
                            return !Node::Equivalent(lhs_ptr, rhs_ptr); 
                        });

                    return factors_intersection;
                });

            if (common_factors.size() > 0) {
                // [ 2, w, x ] - [ 2, w ] = [ x ] ...
                // [ 2, w, y ] - [ 2, w ] = [ x, y ] ...
                // [ 2, w, z ] - [ 2, w ] = [ x, y, z ]
                std::vector<Scalar> uncommon_factors = std::reduce(std::cbegin(factors_of_addends), std::cend(factors_of_addends), std::vector<Scalar>(),
                    [&common_factors](std::vector<Scalar> const &uncommon_factors, std::vector<Scalar> const &factors) -> std::vector<Scalar> {
                        std::vector<Scalar> factors_difference;
                        
                        std::set_difference(std::cbegin(factors), std::cend(factors), std::cbegin(common_factors), std::cend(common_factors), std::back_inserter(factors_difference), 
                            [](Scalar const &lhs_ptr, Scalar const &rhs_ptr) -> bool { 
                                return !Node::Equivalent(lhs_ptr, rhs_ptr); 
                            });

                        std::copy(std::cbegin(uncommon_factors), std::cend(uncommon_factors), std::back_inserter(factors_difference));

                        return factors_difference;
                    });

                if (uncommon_factors.size() > 0) {
                    // [ 2, w ] = 2*w
                    Scalar common_factors_ptr = std::reduce(std::next(std::cbegin(common_factors)), std::cend(common_factors), common_factors.front(), 
                        [](Scalar const &common_factors_ptr, Scalar const &common_factor) -> Scalar { 
                            return Scalar(new MultiplicationNode({ common_factors_ptr, common_factor }));
                        });

                    // [ x, y, z ] = x+y+z
                    Scalar uncommon_factors_ptr = std::reduce(std::next(std::cbegin(uncommon_factors)), std::cend(uncommon_factors), uncommon_factors.front(), 
                        [](Scalar const &uncommon_factors_ptr, Scalar const &uncommon_factor) -> Scalar { 
                            return Scalar(new AdditionNode({ uncommon_factors_ptr, uncommon_factor }));
                        });

                    return Scalar(new MultiplicationNode({
                        common_factors_ptr,
                        uncommon_factors_ptr
                    }));
                }
            }
        }

        return scalar;
    }

    throw std::invalid_argument("std::variant<Scalar, Matrix> holds neither");
}

std::vector<Scalar> ExpressionSimplifier::Factors(Scalar const &node_scalar)
{
    std::vector<Scalar> factors;

    Factors(factors, node_scalar);

    return factors;
}

void ExpressionSimplifier::Factors(std::vector<Scalar> &factors, Scalar const &node_scalar)
{
    if (node_scalar->Type() == "MultiplicationNode") {
        Factors(factors, node_scalar->Argument(0));
        Factors(factors, node_scalar->Argument(1));
    }
    else {
        factors.emplace_back(node_scalar);
    }
}

std::vector<Scalar> ExpressionSimplifier::Addends(Scalar const &node_scalar)
{
    std::vector<Scalar> addends;

    Addends(addends, node_scalar);

    return addends;
}

void ExpressionSimplifier::Addends(std::vector<Scalar> &addends, Scalar const &node_scalar)
{
    if (node_scalar->Type() == "AdditionNode") {
        Addends(addends, node_scalar->Argument(0));
        Addends(addends, node_scalar->Argument(1));
    }
    else {
        addends.emplace_back(node_scalar);
    }
}