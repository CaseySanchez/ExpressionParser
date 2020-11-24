/*
 * Copyright 2020 Casey Sanchez
 */

#include "expression_simplifier.hpp"

ExpressionSimplifier::ExpressionSimplifier(std::shared_ptr<Node> const &node_ptr) : m_node_ptr(node_ptr)
{
}

std::shared_ptr<Node> ExpressionSimplifier::Simplify()
{
    return Simplify(m_node_ptr);
}

std::shared_ptr<Node> ExpressionSimplifier::Simplify(std::shared_ptr<Node> const &node_ptr)
{
    if (node_ptr->Type() == "Affirmation") {
        std::shared_ptr<Affirmation> affirmation_ptr = std::dynamic_pointer_cast<Affirmation>(node_ptr);

        return Simplify(affirmation_ptr->Argument(0));
    }
    else if (node_ptr->Type() == "Negation") {
        std::shared_ptr<Negation> negation_ptr = std::dynamic_pointer_cast<Negation>(node_ptr);

        std::shared_ptr<Node> arg_ptr = negation_ptr->Argument(0);

        if (arg_ptr->Type() == "Constant") {
            return Simplify(std::shared_ptr<Constant>(new Constant(-1.0 * std::get<std::complex<double>>(arg_ptr->Value()))));
        }
    }
    else if (node_ptr->Type() == "Exponentiation") {
        std::shared_ptr<Exponentiation> exponentiation_ptr = std::dynamic_pointer_cast<Exponentiation>(node_ptr);

        std::shared_ptr<Node> lhs_arg_ptr = exponentiation_ptr->Argument(0);
        std::shared_ptr<Node> rhs_arg_ptr = exponentiation_ptr->Argument(1);

        if (rhs_arg_ptr->Type() == "Constant") {
            if (lhs_arg_ptr->Type() == "Constant") {
                return std::shared_ptr<Constant>(new Constant(std::pow(std::get<std::complex<double>>(lhs_arg_ptr->Value()), std::get<std::complex<double>>(rhs_arg_ptr->Value()))));
            }
            else if (Approximately(std::get<std::complex<double>>(rhs_arg_ptr->Value()), 0.0)) {
                return std::shared_ptr<Constant>(new Constant(1.0));
            }
            else if (Approximately(std::get<std::complex<double>>(rhs_arg_ptr->Value()), 1.0)) {
                return Simplify(lhs_arg_ptr);
            }
        }
    }
    else if (node_ptr->Type() == "Multiplication") {
        std::shared_ptr<Multiplication> multiplication_ptr = std::dynamic_pointer_cast<Multiplication>(node_ptr);

        std::shared_ptr<Node> lhs_arg_ptr = multiplication_ptr->Argument(0);
        std::shared_ptr<Node> rhs_arg_ptr = multiplication_ptr->Argument(1);

        if (lhs_arg_ptr->Type() == "Constant") {
            if (rhs_arg_ptr->Type() == "Constant") {
                return std::shared_ptr<Constant>(new Constant(std::get<std::complex<double>>(lhs_arg_ptr->Value()) * std::get<std::complex<double>>(rhs_arg_ptr->Value())));
            }
            else if (Approximately(std::get<std::complex<double>>(lhs_arg_ptr->Value()), 0.0)) {
                return std::shared_ptr<Constant>(new Constant(0.0));
            }
            else if (Approximately(std::get<std::complex<double>>(lhs_arg_ptr->Value()), 1.0)) {
                return Simplify(rhs_arg_ptr);
            }
        }
        else if (rhs_arg_ptr->Type() == "Constant") {
            if (Approximately(std::get<std::complex<double>>(rhs_arg_ptr->Value()), 0.0)) {
                return std::shared_ptr<Constant>(new Constant(0.0));
            }
            else if (Approximately(std::get<std::complex<double>>(rhs_arg_ptr->Value()), 1.0)) {
                return Simplify(lhs_arg_ptr);
            }
        }
    }
    else if (node_ptr->Type() == "Division") {
        std::shared_ptr<Division> division_ptr = std::dynamic_pointer_cast<Division>(node_ptr);

        std::shared_ptr<Node> lhs_arg_ptr = division_ptr->Argument(0);
        std::shared_ptr<Node> rhs_arg_ptr = division_ptr->Argument(1);

        if (lhs_arg_ptr->Type() == "Constant") {
            if (rhs_arg_ptr->Type() == "Constant") {
                return std::shared_ptr<Constant>(new Constant(std::get<std::complex<double>>(lhs_arg_ptr->Value()) / std::get<std::complex<double>>(rhs_arg_ptr->Value())));
            }
            else if (Approximately(std::get<std::complex<double>>(lhs_arg_ptr->Value()), 0.0)) {
                return std::shared_ptr<Constant>(new Constant(0.0));
            }
        }
        else if (rhs_arg_ptr->Type() == "Constant") {
            if (Approximately(std::get<std::complex<double>>(rhs_arg_ptr->Value()), 1.0)) {
                return Simplify(lhs_arg_ptr);
            }
        }
    }
    else if (node_ptr->Type() == "Addition") {
        std::shared_ptr<Addition> addition_ptr = std::dynamic_pointer_cast<Addition>(node_ptr);

        std::shared_ptr<Node> lhs_arg_ptr = addition_ptr->Argument(0);
        std::shared_ptr<Node> rhs_arg_ptr = addition_ptr->Argument(1);

        if (lhs_arg_ptr->Type() == "Constant") {
            if (rhs_arg_ptr->Type() == "Constant") {
                return std::shared_ptr<Constant>(new Constant(std::get<std::complex<double>>(lhs_arg_ptr->Value()) + std::get<std::complex<double>>(rhs_arg_ptr->Value())));
            }
            else if (Approximately(std::get<std::complex<double>>(lhs_arg_ptr->Value()), 0.0)) {
                return Simplify(rhs_arg_ptr);
            }
        }
        else if (rhs_arg_ptr->Type() == "Constant") {
            if (Approximately(std::get<std::complex<double>>(rhs_arg_ptr->Value()), 0.0)) {
                return Simplify(lhs_arg_ptr);
            }
        }
    }
    else if (node_ptr->Type() == "Subtraction") {
        std::shared_ptr<Subtraction> subtraction_ptr = std::dynamic_pointer_cast<Subtraction>(node_ptr);

        std::shared_ptr<Node> lhs_arg_ptr = subtraction_ptr->Argument(0);
        std::shared_ptr<Node> rhs_arg_ptr = subtraction_ptr->Argument(1);

        if (lhs_arg_ptr->Type() == "Constant") {
            if (rhs_arg_ptr->Type() == "Constant") {
                return std::shared_ptr<Constant>(new Constant(std::get<std::complex<double>>(lhs_arg_ptr->Value()) - std::get<std::complex<double>>(rhs_arg_ptr->Value())));
            }
            else if (Approximately(std::get<std::complex<double>>(lhs_arg_ptr->Value()), 0.0)) {
                return Simplify(std::shared_ptr<Negation>(new Negation({ rhs_arg_ptr })));
            }
        }
        else if (rhs_arg_ptr->Type() == "Constant") {
            if (Approximately(std::get<std::complex<double>>(rhs_arg_ptr->Value()), 0.0)) {
                return Simplify(lhs_arg_ptr);
            }
        }
    }
    else if (node_ptr->Type() == "Sin") {
        std::shared_ptr<Sin> sin_ptr = std::dynamic_pointer_cast<Sin>(node_ptr);

        std::shared_ptr<Node> arg_ptr = sin_ptr->Argument(0);

        if (arg_ptr->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::sin(std::get<std::complex<double>>(arg_ptr->Value()))));
        }
    }
    else if (node_ptr->Type() == "Cos") {
        std::shared_ptr<Cos> cos_ptr = std::dynamic_pointer_cast<Cos>(node_ptr);

        std::shared_ptr<Node> arg_ptr = cos_ptr->Argument(0);

        if (arg_ptr->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::cos(std::get<std::complex<double>>(arg_ptr->Value()))));
        }
    }
    else if (node_ptr->Type() == "Tan") {
        std::shared_ptr<Tan> tan_ptr = std::dynamic_pointer_cast<Tan>(node_ptr);

        std::shared_ptr<Node> arg_ptr = tan_ptr->Argument(0);

        if (arg_ptr->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::tan(std::get<std::complex<double>>(arg_ptr->Value()))));
        }
    }
    else if (node_ptr->Type() == "Asin") {
        std::shared_ptr<Asin> asin_ptr = std::dynamic_pointer_cast<Asin>(node_ptr);

        std::shared_ptr<Node> arg_ptr = asin_ptr->Argument(0);

        if (arg_ptr->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::asin(std::get<std::complex<double>>(arg_ptr->Value()))));
        }
    }
    else if (node_ptr->Type() == "Acos") {
        std::shared_ptr<Acos> acos_ptr = std::dynamic_pointer_cast<Acos>(node_ptr);

        std::shared_ptr<Node> arg_ptr = acos_ptr->Argument(0);

        if (arg_ptr->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::acos(std::get<std::complex<double>>(arg_ptr->Value()))));
        }
    }
    else if (node_ptr->Type() == "Atan") {
        std::shared_ptr<Atan> atan_ptr = std::dynamic_pointer_cast<Atan>(node_ptr);

        std::shared_ptr<Node> arg_ptr = atan_ptr->Argument(0);

        if (arg_ptr->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::atan(std::get<std::complex<double>>(arg_ptr->Value()))));
        }
    }
    else if (node_ptr->Type() == "Sqrt") {
        std::shared_ptr<Sqrt> sqrt_ptr = std::dynamic_pointer_cast<Sqrt>(node_ptr);

        std::shared_ptr<Node> arg_ptr = sqrt_ptr->Argument(0);

        if (arg_ptr->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::sqrt(std::get<std::complex<double>>(arg_ptr->Value()))));
        }
    }
    else if (node_ptr->Type() == "Abs") {
        std::shared_ptr<Abs> abs_ptr = std::dynamic_pointer_cast<Abs>(node_ptr);

        std::shared_ptr<Node> arg_ptr = abs_ptr->Argument(0);

        if (arg_ptr->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::abs(std::get<std::complex<double>>(arg_ptr->Value()))));
        }
    }
    else if (node_ptr->Type() == "Exp") {
        std::shared_ptr<Exp> exp_ptr = std::dynamic_pointer_cast<Exp>(node_ptr);

        std::shared_ptr<Node> arg_ptr = exp_ptr->Argument(0);

        if (arg_ptr->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::exp(std::get<std::complex<double>>(arg_ptr->Value()))));
        }
    }
    else if (node_ptr->Type() == "Log") {
        std::shared_ptr<Log> log_ptr = std::dynamic_pointer_cast<Log>(node_ptr);

        std::shared_ptr<Node> arg_ptr = log_ptr->Argument(0);

        if (arg_ptr->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::log(std::get<std::complex<double>>(arg_ptr->Value()))));
        }
    }

    return node_ptr;
}

std::shared_ptr<Node> ExpressionSimplifier::Distribute(std::shared_ptr<Node> const &node_ptr)
{
    std::vector<std::shared_ptr<Node>> factors = Factors(node_ptr);

    if (factors.size() > 0) {
        std::shared_ptr<Node> distributed;

        std::vector<std::shared_ptr<Node>> rhs_summands = Summands(factors.back());

        factors.pop_back();

        while (factors.size() > 0) {
            std::vector<std::shared_ptr<Node>> lhs_summands = Summands(factors.back());

            factors.pop_back();

            distributed = Distribute(lhs_summands, rhs_summands);

            rhs_summands = Summands(distributed);
        }

        return distributed;
    }

    return m_node_ptr;
}

std::shared_ptr<Node> ExpressionSimplifier::Distribute(std::vector<std::shared_ptr<Node>> const &lhs_summands, std::vector<std::shared_ptr<Node>> const &rhs_summands)
{
    std::shared_ptr<Node> distributed_ptr = std::shared_ptr<Constant>(new Constant(0.0));

    for (std::shared_ptr<Node> const &lhs_summand_ptr : lhs_summands) {
        for (std::shared_ptr<Node> const &rhs_summand_ptr : rhs_summands) {
            std::shared_ptr<Multiplication> multiplication_ptr(new Multiplication({ lhs_summand_ptr, rhs_summand_ptr }));

            distributed_ptr = std::shared_ptr<Addition>(new Addition({ distributed_ptr, multiplication_ptr }));
        }
    }

    return distributed_ptr;
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
        std::shared_ptr<Node> lhs_arg_ptr = node_ptr->Argument(0);
        std::shared_ptr<Node> rhs_arg_ptr = node_ptr->Argument(1);

        if (lhs_arg_ptr->Type() != "Multiplication") {
            factors.emplace_back(lhs_arg_ptr);
        }
        
        if (rhs_arg_ptr->Type() != "Multiplication") {
            factors.emplace_back(rhs_arg_ptr);
        }

        Factors(factors, lhs_arg_ptr);
        Factors(factors, rhs_arg_ptr);
    }
}

std::vector<std::shared_ptr<Node>> ExpressionSimplifier::Summands(std::shared_ptr<Node> const &node_ptr)
{
    std::vector<std::shared_ptr<Node>> summands;

    Summands(summands, node_ptr);

    return summands;
}

void ExpressionSimplifier::Summands(std::vector<std::shared_ptr<Node>> &summands, std::shared_ptr<Node> const &node_ptr)
{
    if (node_ptr->Type() == "Addition") {
        std::shared_ptr<Node> lhs_arg_ptr = node_ptr->Argument(0);
        std::shared_ptr<Node> rhs_arg_ptr = node_ptr->Argument(1);

        if (lhs_arg_ptr->Type() != "Addition") {// && lhs_arg_ptr->Type() != "Subtraction") {
            summands.emplace_back(lhs_arg_ptr);
        }

        if (rhs_arg_ptr->Type() != "Addition") {// && rhs_arg_ptr->Type() != "Subtraction") {
            summands.emplace_back(rhs_arg_ptr);
        }

        Summands(summands, lhs_arg_ptr);
        Summands(summands, rhs_arg_ptr);
    }
    /*else if (node_ptr->Type() == "Subtraction") {
        std::shared_ptr<Subtraction> subtraction_ptr = std::dynamic_pointer_cast<Subtraction>(node_ptr);

        std::shared_ptr<Node> lhs_arg_ptr = subtraction_ptr->Argument(0);
        std::shared_ptr<Node> rhs_arg_ptr = subtraction_ptr->Argument(1);

        if (lhs_arg_ptr->Type() != "Addition" && lhs_arg_ptr->Type() != "Subtraction") {
            summands.emplace_back(lhs_arg_ptr);
        }

        if (rhs_arg_ptr->Type() != "Addition" && rhs_arg_ptr->Type() != "Subtraction") {
            summands.emplace_back(std::shared_ptr<Negation>(new Negation({ rhs_arg_ptr })));
        }

        Summands(summands, lhs_arg_ptr);
        Summands(summands, rhs_arg_ptr);
    }*/
}