/*
 * Copyright 2020 Casey Sanchez
 */

#pragma once

#include <exception>
#include <memory>
#include <string>
#include <map>
#include <algorithm>
#include <regex>
#include <variant>
#include <iostream>

#include "node.hpp"
#include "operations.hpp"
#include "functions.hpp"
#include "utils.hpp"
#include "matrix.hpp"
#include "complex_parser.hpp"

class ExpressionParserContext
{
    friend class ExpressionParser;

    uint32_t m_expression_id;
    uint32_t m_function_id;
    uint32_t m_constant_id;
    uint32_t m_matrix_id;

public:
    static std::shared_ptr<ExpressionParserContext> const default_context;

    ExpressionParserContext();

private:
    std::string NextExpressionName();
    std::string NextFunctionName();
    std::string NextConstantName();
    std::string NextMatrixName();
};

class ExpressionParser
{
    std::string m_expression_str;

    std::map<std::string, std::shared_ptr<Node>> m_node_map;

    std::shared_ptr<ExpressionParserContext> m_parser_context;

    ExpressionParser(std::string const &expression_str, std::map<std::string, std::shared_ptr<Node>> const &node_map, std::shared_ptr<ExpressionParserContext> const &parser_context, bool const &clean_and_verify);

public:
    ExpressionParser(std::string const &expression_str, std::map<std::string, std::shared_ptr<Node>> const &node_map = { }, std::shared_ptr<ExpressionParserContext> const &parser_context = ExpressionParserContext::default_context);

    std::shared_ptr<Node> Parse();

private:
    void Clean();
    void Verify();

    std::string Matrices(std::string const &expression_str);
    void Cols(std::string const &expression_str, std::vector<std::shared_ptr<Node>> &col_elements);
    void Rows(std::string const &expression_str, std::vector<std::vector<std::shared_ptr<Node>>> &row_elements);

    std::shared_ptr<Node> Brackets(std::string const &expression_str);
    std::shared_ptr<Node> Functions(std::string const &expression_str);
    std::shared_ptr<Node> Operators(std::string const &expression_str);
    std::shared_ptr<Node> Nodes(std::string const &expression_str);
};
