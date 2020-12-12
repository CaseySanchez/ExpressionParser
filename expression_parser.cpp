/*
 * Copyright 2020 Casey Sanchez
 */

#include "expression_parser.hpp"

std::shared_ptr<ExpressionParserContext> const ExpressionParserContext::default_context(new ExpressionParserContext());

ExpressionParserContext::ExpressionParserContext() : m_expression_id(0), m_function_id(0), m_constant_id(0), m_matrix_id(0)
{
}

std::string ExpressionParserContext::NextExpressionName()
{
    return "E_" + std::to_string(m_expression_id++);
}

std::string ExpressionParserContext::NextFunctionName()
{
    return "F_" + std::to_string(m_function_id++);
}

std::string ExpressionParserContext::NextConstantName()
{
    return "C_" + std::to_string(m_constant_id++);
}

std::string ExpressionParserContext::NextMatrixName()
{
    return "M_" + std::to_string(m_matrix_id++);
}

ExpressionParser::ExpressionParser(std::string const &expression_str, std::map<std::string, std::shared_ptr<Node>> const &node_map, std::shared_ptr<ExpressionParserContext> const &parser_context, bool const &clean_and_verify) : m_expression_str(expression_str), m_node_map(node_map), m_parser_context(parser_context)
{
    if (clean_and_verify) {
        Clean();

        Verify();
    }
}

ExpressionParser::ExpressionParser(std::string const &expression_str, std::map<std::string, std::shared_ptr<Node>> const &node_map, std::shared_ptr<ExpressionParserContext> const &parser_context) : ExpressionParser(expression_str, node_map, parser_context, true)
{
}

std::shared_ptr<Node> ExpressionParser::Parse()
{
    std::string expression_str = Matrices(m_expression_str);

    return Brackets(expression_str);
}

void ExpressionParser::Clean()
{
    m_expression_str.erase(std::remove_if(std::begin(m_expression_str), std::end(m_expression_str), ::isspace), std::end(m_expression_str));

    if (m_expression_str.empty()) {
        throw std::invalid_argument("Expression is empty");
    }

    std::regex left_right_regex("\\\\left|\\\\right");

    m_expression_str = std::regex_replace(m_expression_str, left_right_regex, "");
}

void ExpressionParser::Verify()
{
    std::regex reserved_regex("^E_[0-9]+$|^F_[0-9]+$|^C_[0-9]+$|^M_[0-9]+$");

    std::smatch reserved_match;

    for (auto const &[node_name, node_ptr] : m_node_map) {
        if (std::regex_search(std::cbegin(node_name), std::cend(node_name), reserved_match, reserved_regex)) {
            throw std::invalid_argument("Reserved node name: " + node_name);
        }
    }
}

std::string ExpressionParser::Matrices(std::string const &expression_str)
{
    std::regex matrix_regex("^(.*)(\\\\begin\\{bmatrix\\})(.*?)(\\\\end\\{bmatrix\\})(.*)$");
    
    std::smatch matrix_match;

    if (std::regex_search(std::cbegin(expression_str), std::cend(expression_str), matrix_match, matrix_regex)) {
        std::vector<std::vector<std::shared_ptr<Node>>> elements;
        
        Rows(matrix_match[3].str(), elements);

        size_t rows = elements.size();
        size_t cols = elements.front().size();

        if (!std::all_of(std::next(std::cbegin(elements)), std::cend(elements), [&cols](std::vector<std::shared_ptr<Node>> const &element) -> bool { return element.size() == cols; })) {
            throw std::invalid_argument("Matrix is ill-formed: " + matrix_match[3].str());
        }
        
        std::vector<std::shared_ptr<Node>> flattened;

        for (std::vector<std::shared_ptr<Node>> const &element : elements) {
            std::copy(std::cbegin(element), std::cend(element), std::back_inserter(flattened));
        }

        std::string matrix_name = m_parser_context->NextMatrixName();

        std::shared_ptr<MatrixNode> matrix_ptr(new MatrixNode(Matrix(rows, cols, flattened)));

        m_node_map.emplace(matrix_name, matrix_ptr);

        return Matrices(matrix_match[1].str() + matrix_name + matrix_match[5].str());
    }

    return expression_str;
}

void ExpressionParser::Cols(std::string const &expression_str, std::vector<std::shared_ptr<Node>> &col_elements)
{
    std::regex matrix_col_regex("^(.*?)(\\&)(.*)$");
    std::smatch matrix_col_match;

    if (std::regex_search(std::cbegin(expression_str), std::cend(expression_str), matrix_col_match, matrix_col_regex)) {
        ExpressionParser expression_parser(matrix_col_match[1].str(), m_node_map, m_parser_context, false);

        col_elements.emplace_back(expression_parser.Parse());

        Cols(matrix_col_match[3].str(), col_elements);
    }
    else {
        ExpressionParser expression_parser(expression_str, m_node_map, m_parser_context, false);

        col_elements.emplace_back(expression_parser.Parse());
    }
}

void ExpressionParser::Rows(std::string const &expression_str, std::vector<std::vector<std::shared_ptr<Node>>> &row_elements)
{
    std::regex matrix_row_regex("^(.*?)(\\\\\\\\)(.*)$");
    std::smatch matrix_row_match;

    if (std::regex_search(std::cbegin(expression_str), std::cend(expression_str), matrix_row_match, matrix_row_regex)) {
        std::vector<std::shared_ptr<Node>> col_elements;

        Cols(matrix_row_match[1].str(), col_elements);
        
        row_elements.emplace_back(col_elements);

        Rows(matrix_row_match[3].str(), row_elements);
    }
    else {
        std::vector<std::shared_ptr<Node>> col_elements;

        Cols(expression_str, col_elements);

        row_elements.emplace_back(col_elements);
    }
}

std::shared_ptr<Node> ExpressionParser::Brackets(std::string const &expression_str)
{
    std::regex bracket_regex("^(.*)(\\(|\\[|\\{)(.*?)(\\)|\\]|\\})(.*)$");
    std::regex function_regex("^(.*(\\+|\\-|\\*|\\/|\\^|\\(|\\[|\\{))(.*)$|^(.*)$");
    
    std::smatch bracket_match;
    std::smatch function_match;
    
    if (std::regex_search(std::cbegin(expression_str), std::cend(expression_str), bracket_match, bracket_regex)) {
        if ((bracket_match[2].str() == "(" && bracket_match[4].str() == ")") || 
            (bracket_match[2].str() == "[" && bracket_match[4].str() == "]") || 
            (bracket_match[2].str() == "{" && bracket_match[4].str() == "}")) {
            std::shared_ptr<Node> expression_ptr = Functions(bracket_match[3].str());

            std::string expression_name = m_parser_context->NextExpressionName();

            m_node_map.emplace(expression_name, expression_ptr);

            return Brackets(bracket_match[1].str() + expression_name + bracket_match[5].str());
        }
        else {
            throw std::invalid_argument("Bracket mismatch: " + bracket_match[2].str() + " and " + bracket_match[4].str());
        }
    }
    
    return Functions(expression_str);
}

std::shared_ptr<Node> ExpressionParser::Functions(std::string const &expression_str)
{
    std::regex function_regex("^(.*)(cos|sin|tan|acos|asin|atan|sqrt|abs|exp|log|det|inv)(E_\\d+)(.*?)$|^(.*)(\\\\frac)(E_\\d+)(E_\\d+)(.*?)$");

    std::smatch function_match;

    if (std::regex_search(std::cbegin(expression_str), std::cend(expression_str), function_match, function_regex)) {    
        if (!function_match[2].str().empty()) {
            std::shared_ptr<Node> arg_ptr = m_node_map.at(function_match[3].str());
             
            std::shared_ptr<Node> function_ptr;

            if (function_match[2].str() == "cos") {
                function_ptr = std::shared_ptr<CosNode>(new CosNode({ arg_ptr }));
            }
            else if (function_match[2].str() == "sin") {
                function_ptr = std::shared_ptr<SinNode>(new SinNode({ arg_ptr }));
            }
            else if (function_match[2].str() == "tan") {
                function_ptr = std::shared_ptr<TanNode>(new TanNode({ arg_ptr }));
            }
            else if (function_match[2].str() == "acos") {
                function_ptr = std::shared_ptr<AcosNode>(new AcosNode({ arg_ptr }));   
            }
            else if (function_match[2].str() == "asin") {
                function_ptr = std::shared_ptr<AsinNode>(new AsinNode({ arg_ptr }));
            }
            else if (function_match[2].str() == "atan") {
                function_ptr = std::shared_ptr<AtanNode>(new AtanNode({ arg_ptr }));
            }
            else if (function_match[2].str() == "sqrt") {
                function_ptr = std::shared_ptr<SqrtNode>(new SqrtNode({ arg_ptr }));
            }
            else if (function_match[2].str() == "abs") {
                function_ptr = std::shared_ptr<AbsNode>(new AbsNode({ arg_ptr }));
            }
            else if (function_match[2].str() == "exp") {
                function_ptr = std::shared_ptr<ExpNode>(new ExpNode({ arg_ptr }));
            }
            else if (function_match[2].str() == "log") {
                function_ptr = std::shared_ptr<LogNode>(new LogNode({ arg_ptr }));
            }
            else if (function_match[2].str() == "det") {
                function_ptr = std::shared_ptr<DeterminantNode>(new DeterminantNode({ arg_ptr }));
            }
            else if (function_match[2].str() == "inv") {
                function_ptr = std::shared_ptr<InverseNode>(new InverseNode({ arg_ptr }));
            }
            else {
                throw std::invalid_argument("Unrecognized function: " + function_match[2].str());
            }
            
            std::string function_name = m_parser_context->NextFunctionName();

            m_node_map.emplace(function_name, function_ptr);
            
            return Functions(function_match[1].str() + function_name + function_match[4].str());
        }
        else if (!function_match[6].str().empty()) {
            std::shared_ptr<Node> lhs_arg_ptr = m_node_map.at(function_match[7].str());
            std::shared_ptr<Node> rhs_arg_ptr = m_node_map.at(function_match[8].str());

            std::shared_ptr<Node> function_ptr;
            
            if (function_match[6].str() == "\\frac") {
                function_ptr = std::shared_ptr<DivisionNode>(new DivisionNode({ lhs_arg_ptr, rhs_arg_ptr }));
            }
            else {
                throw std::invalid_argument("Unrecognized function: " + function_match[6].str());
            }

            std::string function_name = m_parser_context->NextFunctionName();

            m_node_map.emplace(function_name, function_ptr);
            
            return Functions(function_match[5].str() + function_name + function_match[9].str());
        }
    }

    return Operators(expression_str);
}

std::shared_ptr<Node> ExpressionParser::Operators(std::string const &expression_str)
{
    std::regex operator_regex("^(.*[^\\+\\-\\*\\/\\^])([\\+\\-])(.*)$|^(.*[^\\+\\-\\*\\/\\^])([\\*\\/])(.*)$|^(.*?[^\\+\\-\\*\\/\\^])([\\^])(.*)$|^([\\+\\-])(.*)$");
    
    std::smatch operator_match;

    if (std::regex_search(std::cbegin(expression_str), std::cend(expression_str), operator_match, operator_regex)) {
        if (operator_match[2].str() == "+") {
            std::shared_ptr<Node> lhs_arg_ptr = Operators(operator_match[1].str());
            std::shared_ptr<Node> rhs_arg_ptr = Operators(operator_match[3].str());

            return std::shared_ptr<AdditionNode>(new AdditionNode({ lhs_arg_ptr, rhs_arg_ptr }));
        }
        else if (operator_match[2].str() == "-") {
            std::shared_ptr<Node> lhs_arg_ptr = Operators(operator_match[1].str());
            std::shared_ptr<Node> rhs_arg_ptr = Operators(operator_match[3].str());

            return std::shared_ptr<SubtractionNode>(new SubtractionNode({ lhs_arg_ptr, rhs_arg_ptr }));
        }
        else if (operator_match[5].str() == "*") {
            std::shared_ptr<Node> lhs_arg_ptr = Operators(operator_match[4].str());
            std::shared_ptr<Node> rhs_arg_ptr = Operators(operator_match[6].str());

            return std::shared_ptr<MultiplicationNode>(new MultiplicationNode({ lhs_arg_ptr, rhs_arg_ptr }));
        }
        else if (operator_match[5].str() == "/") {
            std::shared_ptr<Node> lhs_arg_ptr = Operators(operator_match[4].str());
            std::shared_ptr<Node> rhs_arg_ptr = Operators(operator_match[6].str());

            return std::shared_ptr<DivisionNode>(new DivisionNode({ lhs_arg_ptr, rhs_arg_ptr }));
        }
        else if (operator_match[8].str() == "^") {
            std::shared_ptr<Node> lhs_arg_ptr = Operators(operator_match[7].str());
            std::shared_ptr<Node> rhs_arg_ptr = Operators(operator_match[9].str());

            return std::shared_ptr<ExponentiationNode>(new ExponentiationNode({ lhs_arg_ptr, rhs_arg_ptr }));
        }
        else if (operator_match[10].str() == "+") {
            std::shared_ptr<Node> arg_ptr = Operators(operator_match[11].str());

            return std::shared_ptr<MultiplicationNode>(new MultiplicationNode({ std::shared_ptr<ConstantNode>(new ConstantNode(1.0)), arg_ptr }));
        }
        else if (operator_match[10].str() == "-") {
            std::shared_ptr<Node> arg_ptr = Operators(operator_match[11].str());

            return std::shared_ptr<MultiplicationNode>(new MultiplicationNode({ std::shared_ptr<ConstantNode>(new ConstantNode(-1.0)), arg_ptr }));
        } 
    }

    return Nodes(expression_str);
}

std::shared_ptr<Node> ExpressionParser::Nodes(std::string const &expression_str)
{
    try {
        ComplexParser complex_parser(expression_str);

        std::complex<double> value = complex_parser.Parse();

        std::string constant_name = m_parser_context->NextConstantName();
        
        std::shared_ptr<ConstantNode> constant_ptr(new ConstantNode(value));

        m_node_map.emplace(constant_name, constant_ptr);

        return constant_ptr;
    }
    catch (std::invalid_argument const &) {
        try {
            return m_node_map.at(expression_str);
        }
        catch(std::out_of_range const &) {
            throw std::invalid_argument("No node provided for: " + expression_str);
        }
    }
}
