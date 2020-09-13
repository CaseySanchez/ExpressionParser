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

#include "node.hpp"
#include "operation.hpp"
#include "function.hpp"
#include "utils.hpp"

class ExpressionParserContext
{
    friend class ExpressionParser;

    uint32_t expression_id;
    uint32_t function_id;
    uint32_t constant_id;

public:
    static std::shared_ptr<ExpressionParserContext> const default_context;

    ExpressionParserContext();

private:
    std::string NextExpressionName();
    std::string NextFunctionName();
    std::string NextConstantName();
};

class ExpressionParser
{
    std::string expression_str;

    std::map<std::string, std::shared_ptr<Node>> node_map;

    std::shared_ptr<ExpressionParserContext> parser_context;

public:
    ExpressionParser(std::string const &expression_str, std::map<std::string, std::shared_ptr<Node>> const &node_map = { }, std::shared_ptr<ExpressionParserContext> const &context = ExpressionParserContext::default_context);

    std::shared_ptr<Node> Parse();

private:
    void Clean();
    void Verify();
    
    std::shared_ptr<Node> Brackets(std::string const &expression_str);
    std::shared_ptr<Node> Operations(std::string const &expression_str);
    std::shared_ptr<Node> Nodes(std::string const &node_str);
};
