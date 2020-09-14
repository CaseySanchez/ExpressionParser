/*
 * Copyright 2020 Casey Sanchez
 */ 

#include "equation_parser.hpp"

EquationParser::EquationParser(std::string const &equation_str, std::map<std::string, std::shared_ptr<Node>> const &node_map, std::shared_ptr<ExpressionParserContext> const &parser_context)
{
    std::regex equation_regex("^(.*)=(.*)$");

    std::smatch equation_match;

    if (!std::regex_search(std::cbegin(equation_str), std::cend(equation_str), equation_match, equation_regex)) {
        throw std::invalid_argument("No equals sign (=) found");
    }

    ExpressionParser lhs_expression_parser(equation_match[1].str(), node_map, parser_context);
    ExpressionParser rhs_expression_parser(equation_match[2].str(), node_map, parser_context);

    lhs_ptr = lhs_expression_parser.Parse();
    rhs_ptr = rhs_expression_parser.Parse();
}

bool EquationParser::Equal() const
{
    return Approximately(lhs_ptr->Value(), rhs_ptr->Value());
}
