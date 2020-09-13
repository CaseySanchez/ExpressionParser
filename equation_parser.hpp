/*
 * Copyright 2020 Casey Sanchez
 */

#pragma once

#include "expression_parser.hpp"

class EquationParser
{
    std::shared_ptr<Node> lhs_ptr;
    std::shared_ptr<Node> rhs_ptr;

public:
    EquationParser(std::string const &equation_str, std::map<std::string, std::shared_ptr<Node>> const &node_map = { }, std::shared_ptr<ExpressionParserContext> const &parser_context = ExpressionParserContext::default_context);

    bool Equal() const;
};
