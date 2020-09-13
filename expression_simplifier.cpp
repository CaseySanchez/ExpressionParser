/*
 * Copyright 2020 Casey Sanchez
 */

#include "expression_simplifier.hpp"

ExpressionSimplifier::ExpressionSimplifier(std::shared_ptr<Node> const &node_ptr) : node_ptr(node_ptr)
{
}

std::shared_ptr<Node> ExpressionSimplifier::Simplify()
{
    return Simplify(node_ptr);
}

std::shared_ptr<Node> ExpressionSimplifier::Simplify(std::shared_ptr<Node> const &node_ptr)
{
    if (node_ptr->Type() == "Affirmation") {
        std::shared_ptr<Affirmation> affirmation_ptr = std::dynamic_pointer_cast<Affirmation>(node_ptr);

        return affirmation_ptr->NodePtr();
    }
    else if (node_ptr->Type() == "Negation") {
        std::shared_ptr<Negation> negation_ptr = std::dynamic_pointer_cast<Negation>(node_ptr);

        std::shared_ptr<Node> node = negation_ptr->NodePtr();

        if (node->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(-1.0 * node->Value()));
        }
    }
    else if (node_ptr->Type() == "Exponentiation") {
        std::shared_ptr<Exponentiation> exponentiation_ptr = std::dynamic_pointer_cast<Exponentiation>(node_ptr);

        std::shared_ptr<Node> lhs_ptr = exponentiation_ptr->LhsPtr();
        std::shared_ptr<Node> rhs_ptr = exponentiation_ptr->RhsPtr();

        if (rhs_ptr->Type() == "Constant") {
            if (Approximately(rhs_ptr->Value(), 0.0)) {
                return std::shared_ptr<Constant>(new Constant(1.0));
            }
            else if (Approximately(rhs_ptr->Value(), 1.0)) {
                return lhs_ptr;
            }
            else if (lhs_ptr->Type() == "Constant") {
                return std::shared_ptr<Constant>(new Constant(std::pow(lhs_ptr->Value(), rhs_ptr->Value())));
            }
        }
    }
    else if (node_ptr->Type() == "Multiplication") {
        std::shared_ptr<Multiplication> multiplication_ptr = std::dynamic_pointer_cast<Multiplication>(node_ptr);

        std::shared_ptr<Node> lhs_ptr = multiplication_ptr->LhsPtr();
        std::shared_ptr<Node> rhs_ptr = multiplication_ptr->RhsPtr();

        if (lhs_ptr->Type() == "Constant") {
            if (Approximately(lhs_ptr->Value(), 0.0)) {
                return std::shared_ptr<Constant>(new Constant(0.0));
            }
            else if (Approximately(lhs_ptr->Value(), 1.0)) {
                return rhs_ptr;
            }
            else if (rhs_ptr->Type() == "Constant") {
                return std::shared_ptr<Constant>(new Constant(lhs_ptr->Value() * rhs_ptr->Value()));
            }
        }
        else if (rhs_ptr->Type() == "Constant") {
            if (Approximately(rhs_ptr->Value(), 0.0)) {
                return std::shared_ptr<Constant>(new Constant(0.0));
            }
            else if (Approximately(rhs_ptr->Value(), 1.0)) {
                return lhs_ptr;
            }
        }
    }
    else if (node_ptr->Type() == "Division") {
        std::shared_ptr<Division> division_ptr = std::dynamic_pointer_cast<Division>(node_ptr);

        std::shared_ptr<Node> lhs_ptr = division_ptr->LhsPtr();
        std::shared_ptr<Node> rhs_ptr = division_ptr->RhsPtr();

        if (lhs_ptr->Type() == "Constant") {
            if (Approximately(lhs_ptr->Value(), 0.0)) {
                return std::shared_ptr<Constant>(new Constant(0.0));
            }
            else if (rhs_ptr->Type() == "Constant") {
                return std::shared_ptr<Constant>(new Constant(lhs_ptr->Value() / rhs_ptr->Value()));
            }
        }
        else if (rhs_ptr->Type() == "Constant") {
            if (Approximately(rhs_ptr->Value(), 1.0)) {
                return lhs_ptr;
            }
        }
    }
    else if (node_ptr->Type() == "Addition") {
        std::shared_ptr<Addition> addition_ptr = std::dynamic_pointer_cast<Addition>(node_ptr);

        std::shared_ptr<Node> lhs_ptr = addition_ptr->LhsPtr();
        std::shared_ptr<Node> rhs_ptr = addition_ptr->RhsPtr();

        if (lhs_ptr->Type() == "Constant") {
            if (Approximately(lhs_ptr->Value(), 0.0)) {
                return rhs_ptr;
            }
            else if (rhs_ptr->Type() == "Constant") {
                return std::shared_ptr<Constant>(new Constant(lhs_ptr->Value() + rhs_ptr->Value()));
            }
        }
        else if (rhs_ptr->Type() == "Constant") {
            if (Approximately(rhs_ptr->Value(), 0.0)) {
                return lhs_ptr;
            }
        }
    }
    else if (node_ptr->Type() == "Subtraction") {
        std::shared_ptr<Subtraction> subtraction_ptr = std::dynamic_pointer_cast<Subtraction>(node_ptr);

        std::shared_ptr<Node> lhs_ptr = subtraction_ptr->LhsPtr();
        std::shared_ptr<Node> rhs_ptr = subtraction_ptr->RhsPtr();

        if (lhs_ptr->Type() == "Constant") {
            if (rhs_ptr->Type() == "Constant") {
                return std::shared_ptr<Constant>(new Constant(lhs_ptr->Value() - rhs_ptr->Value()));
            }
            else if (Approximately(lhs_ptr->Value(), 0.0)) {
                return std::shared_ptr<Negation>(new Negation(rhs_ptr));
            }
        }
        else if (rhs_ptr->Type() == "Constant") {
            if (Approximately(rhs_ptr->Value(), 0.0)) {
                return lhs_ptr;
            }
        }
    }
    else if (node_ptr->Type() == "Sin") {
        std::shared_ptr<Sin> sin_ptr = std::dynamic_pointer_cast<Sin>(node_ptr);

        std::shared_ptr<Node> node = sin_ptr->NodePtr();

        if (node->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::sin(node->Value())));
        }
    }
    else if (node_ptr->Type() == "Cos") {
        std::shared_ptr<Cos> cos_ptr = std::dynamic_pointer_cast<Cos>(node_ptr);

        std::shared_ptr<Node> node = cos_ptr->NodePtr();

        if (node->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::cos(node->Value())));
        }
    }
    else if (node_ptr->Type() == "Tan") {
        std::shared_ptr<Tan> tan_ptr = std::dynamic_pointer_cast<Tan>(node_ptr);

        std::shared_ptr<Node> node = tan_ptr->NodePtr();

        if (node->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::tan(node->Value())));
        }
    }
    else if (node_ptr->Type() == "Asin") {
        std::shared_ptr<Asin> asin_ptr = std::dynamic_pointer_cast<Asin>(node_ptr);

        std::shared_ptr<Node> node = asin_ptr->NodePtr();

        if (node->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::asin(node->Value())));
        }
    }
    else if (node_ptr->Type() == "Acos") {
        std::shared_ptr<Acos> acos_ptr = std::dynamic_pointer_cast<Acos>(node_ptr);

        std::shared_ptr<Node> node = acos_ptr->NodePtr();

        if (node->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::acos(node->Value())));
        }
    }
    else if (node_ptr->Type() == "Atan") {
        std::shared_ptr<Atan> atan_ptr = std::dynamic_pointer_cast<Atan>(node_ptr);

        std::shared_ptr<Node> node = atan_ptr->NodePtr();

        if (node->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::atan(node->Value())));
        }
    }
    else if (node_ptr->Type() == "Sqrt") {
        std::shared_ptr<Sqrt> sqrt_ptr = std::dynamic_pointer_cast<Sqrt>(node_ptr);

        std::shared_ptr<Node> node = sqrt_ptr->NodePtr();

        if (node->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::sqrt(node->Value())));
        }
    }
    else if (node_ptr->Type() == "Abs") {
        std::shared_ptr<Abs> abs_ptr = std::dynamic_pointer_cast<Abs>(node_ptr);

        std::shared_ptr<Node> node = abs_ptr->NodePtr();

        if (node->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::abs(node->Value())));
        }
    }
    else if (node_ptr->Type() == "Exp") {
        std::shared_ptr<Exp> exp_ptr = std::dynamic_pointer_cast<Exp>(node_ptr);

        std::shared_ptr<Node> node = exp_ptr->NodePtr();

        if (node->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::exp(node->Value())));
        }
    }
    else if (node_ptr->Type() == "Log") {
        std::shared_ptr<Log> log_ptr = std::dynamic_pointer_cast<Log>(node_ptr);

        std::shared_ptr<Node> node = log_ptr->NodePtr();

        if (node->Type() == "Constant") {
            return std::shared_ptr<Constant>(new Constant(std::log(node->Value())));
        }
    }

    return node_ptr;
}