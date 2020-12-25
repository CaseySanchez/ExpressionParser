/*
 * Copyright 2020 Casey Sanchez
 */

#pragma once

#include "expression_parser.hpp"

class EquationParser
{
    std::variant<Scalar, Matrix> m_lhs_variant;
    std::variant<Scalar, Matrix> m_rhs_variant;

    struct Visitor
    {
        bool operator()(Scalar const &lhs, Scalar const &rhs)
        {
            return Approximately(lhs->Value(), rhs->Value());
        }

        bool operator()(Scalar const &lhs, Matrix const &rhs)
        {
            return false;
        }

        bool operator()(Matrix const &lhs, Scalar const &rhs) 
        {
            return false;
        }

        bool operator()(Matrix const &lhs, Matrix const &rhs)
        {
            if (lhs.Rows() != rhs.Rows() || lhs.Cols() != rhs.Cols()) {
                return false;
            }

            for (size_t i = 0; i < lhs.Rows(); ++i) {
                for (size_t j = 0; j < lhs.Cols(); ++j) {
                    if (!Approximately(lhs(i, j)->Value(), rhs(i, j)->Value())) {
                        return false;
                    }
                }
            }

            return true;
        }
    };

public:
    EquationParser(std::string const &equation_str, std::map<std::string, std::variant<Scalar, Matrix>> const &node_map = { }, std::shared_ptr<ExpressionParserContext> const &parser_context = ExpressionParserContext::default_context);

    bool Equal() const;
};
