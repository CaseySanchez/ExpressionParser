/*
 * Copyright 2020 Casey Sanchez
 */ 

#include "equation_parser.hpp"

EquationParser::EquationParser(std::string const &equation_str, std::map<std::string, std::variant<Scalar, Matrix>> const &node_map, std::shared_ptr<ExpressionParserContext> const &parser_context)
{
    std::regex equation_regex("^(.*)=(.*)$");

    std::smatch equation_match;

    if (!std::regex_search(std::cbegin(equation_str), std::cend(equation_str), equation_match, equation_regex)) {
        throw std::invalid_argument("No equals sign (=) found");
    }

    ExpressionParser lhs_expression_parser(equation_match[1].str(), node_map, parser_context);
    ExpressionParser rhs_expression_parser(equation_match[2].str(), node_map, parser_context);

    m_lhs_variant = lhs_expression_parser.Parse();
    m_rhs_variant = rhs_expression_parser.Parse();
}

bool EquationParser::Equal() const
{
    return std::visit(Visitor{ }, m_lhs_variant, m_rhs_variant);
}
