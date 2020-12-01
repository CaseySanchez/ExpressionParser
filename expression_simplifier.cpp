/*
 * Copyright 2020 Casey Sanchez
 */

#include "expression_simplifier.hpp"

ExpressionSimplifier::ExpressionSimplifier(std::shared_ptr<Node> const &node_ptr, std::map<std::string, std::shared_ptr<Node>> const &node_map) : m_node_ptr(node_ptr), m_node_map(node_map)
{
}

std::shared_ptr<Node> ExpressionSimplifier::Simplify()
{
    return Identify(CombineAddends(Identify(CombineFactors(Identify(Distribute(Identify(m_node_ptr)))))));
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

    if (node_ptr->Type() == "Exponentiation") {
        if (node_ptr->Argument(1)->Type() == "Constant") {
            if (Approximately(std::get<std::complex<double>>(node_ptr->Argument(1)->Value()), 0.0)) {
                return std::shared_ptr<Constant>(new Constant(1.0));
            }
            else if (Approximately(std::get<std::complex<double>>(node_ptr->Argument(1)->Value()), 1.0)) {
                return node_ptr->Argument(0);
            }
            else if (node_ptr->Argument(0)->Type() == "Constant") {
                return std::shared_ptr<Constant>(new Constant(std::pow(std::get<std::complex<double>>(node_ptr->Argument(0)->Value()), std::get<std::complex<double>>(node_ptr->Argument(1)->Value()))));
            }
        }
    }
    else if (node_ptr->Type() == "Multiplication") {
        if (node_ptr->Argument(0)->Type() == "Constant") {
            if (Approximately(std::get<std::complex<double>>(node_ptr->Argument(0)->Value()), 0.0)) {
                return std::shared_ptr<Constant>(new Constant(0.0));
            }
            else if (Approximately(std::get<std::complex<double>>(node_ptr->Argument(0)->Value()), 1.0)) {
                return node_ptr->Argument(1);
            }
            else if (node_ptr->Argument(1)->Type() == "Constant") {
                return std::shared_ptr<Constant>(new Constant(std::get<std::complex<double>>(node_ptr->Argument(0)->Value()) * std::get<std::complex<double>>(node_ptr->Argument(1)->Value())));
            }
        }
        else if (node_ptr->Argument(1)->Type() == "Constant") {
            if (Approximately(std::get<std::complex<double>>(node_ptr->Argument(1)->Value()), 0.0)) {
                return std::shared_ptr<Constant>(new Constant(0.0));
            }
            else if (Approximately(std::get<std::complex<double>>(node_ptr->Argument(1)->Value()), 1.0)) {
                return node_ptr->Argument(0);
            }
        }
    }
    else if (node_ptr->Type() == "Division") {
        if (node_ptr->Argument(0)->Type() == "Constant") {
            if (Approximately(std::get<std::complex<double>>(node_ptr->Argument(0)->Value()), 0.0)) {
                return std::shared_ptr<Constant>(new Constant(0.0));
            }
            else if (node_ptr->Argument(1)->Type() == "Constant") {
                return std::shared_ptr<Constant>(new Constant(std::get<std::complex<double>>(node_ptr->Argument(0)->Value()) / std::get<std::complex<double>>(node_ptr->Argument(1)->Value())));
            }
        }
        else if (node_ptr->Argument(1)->Type() == "Constant") {
            if (Approximately(std::get<std::complex<double>>(node_ptr->Argument(1)->Value()), 1.0)) {
                return node_ptr->Argument(0);
            }
        }

        if (node_ptr->Argument(1)->Type() == "Exponentiation") {
            if (node_ptr->Argument(1)->Argument(1)->Type() == "Constant") {
                return std::shared_ptr<Multiplication>(new Multiplication({ node_ptr->Argument(0), std::shared_ptr<Exponentiation>(new Exponentiation({ node_ptr->Argument(1)->Argument(0), std::shared_ptr<Constant>(new Constant(-1.0 * std::get<std::complex<double>>(node_ptr->Argument(1)->Argument(1)->Value()))) })) }));
            }
        }
    }
    else if (node_ptr->Type() == "Addition") {
        if (node_ptr->Argument(0)->Type() == "Constant") {
            if (Approximately(std::get<std::complex<double>>(node_ptr->Argument(0)->Value()), 0.0)) {
                return node_ptr->Argument(1);
            }
            else if (node_ptr->Argument(1)->Type() == "Constant") {
                return std::shared_ptr<Constant>(new Constant(std::get<std::complex<double>>(node_ptr->Argument(0)->Value()) + std::get<std::complex<double>>(node_ptr->Argument(1)->Value())));
            }
        }
        else if (node_ptr->Argument(1)->Type() == "Constant") {
            if (Approximately(std::get<std::complex<double>>(node_ptr->Argument(1)->Value()), 0.0)) {
                return node_ptr->Argument(0);
            }
        }
    }
    else if (node_ptr->Type() == "Subtraction") {
        if (node_ptr->Argument(0)->Type() == "Constant") {
            if (Approximately(std::get<std::complex<double>>(node_ptr->Argument(0)->Value()), 0.0)) {
                return std::shared_ptr<Multiplication>(new Multiplication({ std::shared_ptr<Constant>(new Constant(-1.0)), node_ptr->Argument(1) }));
            }
            else if (node_ptr->Argument(1)->Type() == "Constant") {
                return std::shared_ptr<Constant>(new Constant(std::get<std::complex<double>>(node_ptr->Argument(0)->Value()) - std::get<std::complex<double>>(node_ptr->Argument(1)->Value())));
            }
        }
        else if (node_ptr->Argument(1)->Type() == "Constant") {
            if (Approximately(std::get<std::complex<double>>(node_ptr->Argument(1)->Value()), 0.0)) {
                return node_ptr->Argument(0);
            }
        }
        else {
            return std::shared_ptr<Addition>(new Addition({ node_ptr->Argument(0), std::shared_ptr<Multiplication>(new Multiplication({ std::shared_ptr<Constant>(new Constant(-1.0)), node_ptr->Argument(1) })) }));
        }
    }
    else if (node_ptr->Type() == "Sin") {
        if (node_ptr->Argument(0)->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::sin(std::get<std::complex<double>>(node_ptr->Argument(0)->Value()))));
        }
    }
    else if (node_ptr->Type() == "Cos") {
        if (node_ptr->Argument(0)->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::cos(std::get<std::complex<double>>(node_ptr->Argument(0)->Value()))));
        }
    }
    else if (node_ptr->Type() == "Tan") {
        if (node_ptr->Argument(0)->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::tan(std::get<std::complex<double>>(node_ptr->Argument(0)->Value()))));
        }
    }
    else if (node_ptr->Type() == "Asin") {
        if (node_ptr->Argument(0)->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::asin(std::get<std::complex<double>>(node_ptr->Argument(0)->Value()))));
        }
    }
    else if (node_ptr->Type() == "Acos") {
        if (node_ptr->Argument(0)->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::acos(std::get<std::complex<double>>(node_ptr->Argument(0)->Value()))));
        }
    }
    else if (node_ptr->Type() == "Atan") {
        if (node_ptr->Argument(0)->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::atan(std::get<std::complex<double>>(node_ptr->Argument(0)->Value()))));
        }
    }
    else if (node_ptr->Type() == "Sqrt") {
        if (node_ptr->Argument(0)->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::sqrt(std::get<std::complex<double>>(node_ptr->Argument(0)->Value()))));
        }
    }
    else if (node_ptr->Type() == "Abs") {
        if (node_ptr->Argument(0)->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::abs(std::get<std::complex<double>>(node_ptr->Argument(0)->Value()))));
        }
    }
    else if (node_ptr->Type() == "Exp") {
        if (node_ptr->Argument(0)->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::exp(std::get<std::complex<double>>(node_ptr->Argument(0)->Value()))));
        }
    }
    else if (node_ptr->Type() == "Log") {
        if (node_ptr->Argument(0)->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::log(std::get<std::complex<double>>(node_ptr->Argument(0)->Value()))));
        }
    }

    return node_ptr;
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

    std::vector<std::shared_ptr<Node>> factors = Factors(node_ptr);

    if (factors.size() > 0) {
        std::shared_ptr<Node> distributed_ptr = factors.front();

        for (std::shared_ptr<Node> const &factor : std::vector<std::shared_ptr<Node>>(std::next(std::cbegin(factors)), std::cend(factors))) {
            std::vector<std::shared_ptr<Node>> lhs_addends = Addends(distributed_ptr);
            std::vector<std::shared_ptr<Node>> rhs_addends = Addends(factor);       
                    
            distributed_ptr = std::shared_ptr<Constant>(new Constant(0.0));

            for (std::shared_ptr<Node> const &lhs_summand_ptr : lhs_addends) {
                for (std::shared_ptr<Node> const &rhs_summand_ptr : rhs_addends) {
                    std::shared_ptr<Multiplication> multiplication_ptr(new Multiplication({ lhs_summand_ptr, rhs_summand_ptr }));

                    distributed_ptr = std::shared_ptr<Addition>(new Addition({ distributed_ptr, multiplication_ptr }));
                }
            }
        }

        return distributed_ptr;
    }

    return node_ptr;
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

    std::vector<std::shared_ptr<Node>> factors = Factors(node_ptr);

    if (factors.size() > 0) {
        std::vector<std::shared_ptr<Node>> coefficients;
        std::vector<std::shared_ptr<Node>> variables;

        // Separate the factors into their coefficients and their variables
        std::partition_copy(std::cbegin(factors), std::cend(factors), std::back_inserter(coefficients), std::back_inserter(variables), [](std::shared_ptr<Node> const &factor_ptr) -> bool { return factor_ptr->Type() == "Constant"; });

        if (!variables.empty()) {
            // Assess factors with O(n*(n-1)/2) complexity
            for (auto lhs_variable_it = std::begin(variables); lhs_variable_it != std::end(variables); ++lhs_variable_it) {
                for (auto rhs_variable_it = std::next(lhs_variable_it); rhs_variable_it != std::end(variables); ++rhs_variable_it) {
                    std::shared_ptr<Node> lhs_variable;
                    std::shared_ptr<Node> rhs_variable;
                    
                    // Extract the variable of interest; 
                    // if the node is of type "Exponentiation" we want Argument(0) 
                    if ((*lhs_variable_it)->Type() == "Exponentiation") {
                        lhs_variable = (*lhs_variable_it)->Argument(0);
                    }
                    else {
                        lhs_variable = (*lhs_variable_it);
                    }
                    
                    if ((*rhs_variable_it)->Type() == "Exponentiation") {
                        rhs_variable = (*rhs_variable_it)->Argument(0);
                    }
                    else {
                        rhs_variable = (*rhs_variable_it);
                    }

                    // If the variables are equivalent sum their degrees
                    if (Node::Equivalent(lhs_variable, rhs_variable)) {
                        std::shared_ptr<Node> lhs_degree;
                        std::shared_ptr<Node> rhs_degree;
                        
                        // If the variable is of type "Exponentiation" then our degree is Argument(1)
                        // otherwise we have an implied degree of 1
                        if ((*lhs_variable_it)->Type() == "Exponentiation") {
                            lhs_degree = (*lhs_variable_it)->Argument(1);
                        }
                        else {
                            lhs_degree = std::shared_ptr<Constant>(new Constant(1.0));
                        }
                        
                        if ((*rhs_variable_it)->Type() == "Exponentiation") {
                            rhs_degree = (*rhs_variable_it)->Argument(1);
                        }
                        else {
                            rhs_degree = std::shared_ptr<Constant>(new Constant(1.0));
                        }

                        // Add their degrees and exponentiate
                        *lhs_variable_it = std::shared_ptr<Exponentiation>(new Exponentiation({ lhs_variable, std::shared_ptr<Addition>(new Addition({ lhs_degree, rhs_degree })) }));

                        // Remove the old variable
                        rhs_variable_it = variables.erase(rhs_variable_it);

                        if (rhs_variable_it == std::end(variables)) {
                            break;
                        }
                    }
                }
            }

            // Reduce the factors to a single combined node via repeated Multiplication nodes
            std::shared_ptr<Node> combined_factors = std::reduce(std::next(std::cbegin(variables)), std::cend(variables), variables.front(), [](std::shared_ptr<Node> const &combined_factors, std::shared_ptr<Node> const &combined_factor) { return std::shared_ptr<Multiplication>(new Multiplication({ combined_factors, combined_factor })); });
            
            // Reduce the coefficients to a single value via repeated multiplication
            if (!coefficients.empty()) {
                std::complex<double> coefficient = std::transform_reduce(std::cbegin(coefficients), std::cend(coefficients), std::complex<double>(1.0, 0.0), [](std::complex<double> const &product_coefficient, std::complex<double> const &coefficient) -> std::complex<double> { return product_coefficient * coefficient; }, [](std::shared_ptr<Node> const &coefficient) -> std::complex<double> { return std::get<std::complex<double>>(coefficient->Value()); });

                return std::shared_ptr<Multiplication>(new Multiplication({ std::shared_ptr<Constant>(new Constant(coefficient)), combined_factors}));
            }
            else {
                return combined_factors;
            }
        }
    }

    return node_ptr;
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

    std::vector<std::shared_ptr<Node>> addends = Addends(node_ptr);

    if (addends.size() > 0) {
        // Assess addends with O(n*(n-1)/2) complexity
        for (auto addend_lhs_it = std::begin(addends); addend_lhs_it != std::end(addends); ++addend_lhs_it) {
            for (auto addend_rhs_it = std::next(addend_lhs_it); addend_rhs_it != std::end(addends); ++addend_rhs_it) {                
                // Extract the factors from the lhs addend and the rhs addend                
                auto lhs_factors = Factors(*addend_lhs_it);
                auto rhs_factors = Factors(*addend_rhs_it);

                // Separate the factors into their coefficients and their variables
                std::vector<std::shared_ptr<Node>> lhs_coefficients;
                std::vector<std::shared_ptr<Node>> rhs_coefficients;
                std::vector<std::shared_ptr<Node>> lhs_variables;
                std::vector<std::shared_ptr<Node>> rhs_variables;

                std::partition_copy(std::cbegin(lhs_factors), std::cend(lhs_factors), std::back_inserter(lhs_coefficients), std::back_inserter(lhs_variables), [](std::shared_ptr<Node> const &factor_ptr) -> bool { return factor_ptr->Type() == "Constant"; });
                std::partition_copy(std::cbegin(rhs_factors), std::cend(rhs_factors), std::back_inserter(rhs_coefficients), std::back_inserter(rhs_variables), [](std::shared_ptr<Node> const &factor_ptr) -> bool { return factor_ptr->Type() == "Constant"; });

                // Check if all of the variables are equivalent
                bool all_of = true;

                for (auto lhs_variable : lhs_variables) {
                    all_of = all_of && std::find_if(std::cbegin(rhs_variables), std::cend(rhs_variables), [&lhs_variable](std::shared_ptr<Node> const &rhs_variable) -> bool { return Node::Equivalent(lhs_variable, rhs_variable); }) != std::cend(rhs_variables);
                }

                if (all_of) {
                    // If there are multiple coefficients per term, multiply them together and increment the coefficient                    
                    std::complex<double> coefficient;
                    
                    if (lhs_coefficients.empty()) {
                        coefficient += std::complex<double>(1.0, 0.0);
                    }
                    else {
                        coefficient += std::transform_reduce(std::cbegin(lhs_coefficients), std::cend(lhs_coefficients), std::complex<double>(1.0, 0.0), [](std::complex<double> const &product_coefficient, std::complex<double> const &coefficient) -> std::complex<double> { return product_coefficient * coefficient; }, [](std::shared_ptr<Node> const &lhs_coefficient) -> std::complex<double> { return std::get<std::complex<double>>(lhs_coefficient->Value()); });
                    }

                    if (rhs_coefficients.empty()) {
                        coefficient += std::complex<double>(1.0, 0.0);
                    }
                    else {
                        coefficient += std::transform_reduce(std::cbegin(rhs_coefficients), std::cend(rhs_coefficients), std::complex<double>(1.0, 0.0), [](std::complex<double> const &product_coefficient, std::complex<double> const &coefficient) -> std::complex<double> { return product_coefficient * coefficient; }, [](std::shared_ptr<Node> const &rhs_coefficient) -> std::complex<double> { return std::get<std::complex<double>>(rhs_coefficient->Value()); });
                    }

                    // Recombining the variables via repeated Multiplication
                    std::shared_ptr<Node> combined_variables = std::reduce(std::next(std::cbegin(lhs_variables)), std::cend(lhs_variables), lhs_variables.front(), [](std::shared_ptr<Node> const &combined_variables, std::shared_ptr<Node> const &combined_variable) -> std::shared_ptr<Node> { return std::shared_ptr<Multiplication>(new Multiplication({ combined_variables, combined_variable })); });

                    // Multiply by the coefficient
                    *addend_lhs_it = std::shared_ptr<Multiplication>(new Multiplication({ std::shared_ptr<Constant>(new Constant(coefficient)), combined_variables }));

                    // Remove the old term
                    addend_rhs_it = addends.erase(addend_rhs_it);

                    if (addend_rhs_it == std::end(addends)) {
                        break;
                    }
                }
            }
        }

        // Sum the addends for the final result
        std::shared_ptr<Node> combined_addends = std::reduce(std::next(std::cbegin(addends)), std::cend(addends), addends.front(), [](std::shared_ptr<Node> const &combined_addends, std::shared_ptr<Node> const &combined_addend) { return std::shared_ptr<Addition>(new Addition({ combined_addends, combined_addend })); });

        return combined_addends;
    }

    return node_ptr;
}

std::vector<std::shared_ptr<Node>> ExpressionSimplifier::Factors(std::shared_ptr<Node> const &node_ptr)
{
    std::vector<std::shared_ptr<Node>> factors;

    Factors(factors, node_ptr);

    return factors;
}

void ExpressionSimplifier::Factors(std::vector<std::shared_ptr<Node>> &factors, std::shared_ptr<Node> const &node_ptr)
{
    if (node_ptr->Type() == "Multiplication") {
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
    if (node_ptr->Type() == "Addition") {
        Addends(addends, node_ptr->Argument(0));
        Addends(addends, node_ptr->Argument(1));
    }
    else {
        addends.emplace_back(node_ptr);
    }
}