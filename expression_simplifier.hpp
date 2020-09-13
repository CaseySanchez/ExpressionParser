/*
 * Copyright 2020 Casey Sanchez
 */

#pragma once

#include <memory>
#include <string>

#include "node.hpp"
#include "operation.hpp"
#include "function.hpp"
#include "utils.hpp"

class ExpressionSimplifier
{
    std::shared_ptr<Node> node_ptr;

public:
    ExpressionSimplifier(std::shared_ptr<Node> const &node_ptr);

    std::shared_ptr<Node> Simplify();

private:
    std::shared_ptr<Node> Simplify(std::shared_ptr<Node> const &node_ptr);

    /*
    std::shared_ptr<Node> Distribute(std::shared_ptr<Node> const &node_ptr)
    {
        if (node_ptr->Type() == "Multiplication") {
            std::shared_ptr<Multiplication> multiplication_ptr = std::dynamic_pointer_cast<Multiplication>(node_ptr);

            std::shared_ptr<Node> lhs_ptr = multiplication_ptr->LhsPtr();
            std::shared_ptr<Node> rhs_ptr = multiplication_ptr->RhsPtr();

            std::vector<std::shared_ptr<Node>> lhs_flat_vec = Flatten(lhs_ptr);
            std::vector<std::shared_ptr<Node>> rhs_flat_vec = Flatten(rhs_ptr);

            if (lhs_flat_vec.size() > 1 && rhs_flat_vec.size() > 1) {
                for (std::shared_ptr<Node> const &lhs_arg : lhs_flat_vec) {
                    for (std::shared_ptr<Node> const &rhs_arg : rhs_flat_vec) {
                        
                    }
                }
            }
        }

        return node_ptr;
    }

    std::vector<std::shared_ptr<Node>> Flatten(std::shared_ptr<Node> const &node_ptr)
    {
        std::vector<std::shared_ptr<Node>> flat_vec;

        Flatten(flat_vec, node_ptr);

        return flat_vec;
    }

    void Flatten(std::vector<std::shared_ptr<Node>> &flat_vec, std::shared_ptr<Node> const &node_ptr, bool const &negate = false)
    {
        if (node_ptr->Type() == "Addition") {
            std::shared_ptr<Addition> addition_ptr = std::dynamic_pointer_cast<Addition>(node_ptr);

            std::shared_ptr<Node> lhs_ptr = addition_ptr->LhsPtr();
            std::shared_ptr<Node> rhs_ptr = addition_ptr->RhsPtr();

            Flatten(flat_vec, lhs_ptr);
            Flatten(flat_vec, rhs_ptr);
        }
        else if (node_ptr->Type() == "Subtraction") {
            std::shared_ptr<Subtraction> subtraction_ptr = std::dynamic_pointer_cast<Subtraction>(node_ptr);

            std::shared_ptr<Node> lhs_ptr = subtraction_ptr->LhsPtr();
            std::shared_ptr<Node> rhs_ptr = subtraction_ptr->RhsPtr();

            Flatten(flat_vec, lhs_ptr);
            Flatten(flat_vec, rhs_ptr, true);
        }
        else if (negate) {
            std::shared_ptr<Negation> negation_ptr(new Negation(node_ptr));

            flat_vec.emplace_back(negation_ptr);
        }
        else {
            flat_vec.emplace_back(node_ptr);
        }
    }
    */
};