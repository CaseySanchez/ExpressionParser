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

ExpressionParser::ExpressionParser(std::string const &expression_str, std::map<std::string, std::variant<Scalar, Matrix>> const &node_map, std::shared_ptr<ExpressionParserContext> const &parser_context, bool const &clean_and_verify) : m_expression_str(expression_str), m_node_map(node_map), m_parser_context(parser_context)
{
    if (clean_and_verify) {
        Clean();

        Verify();
    }
}

ExpressionParser::ExpressionParser(std::string const &expression_str, std::map<std::string, std::variant<Scalar, Matrix>> const &node_map, std::shared_ptr<ExpressionParserContext> const &parser_context) : ExpressionParser(expression_str, node_map, parser_context, true)
{
}

std::variant<Scalar, Matrix> ExpressionParser::Parse()
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
        std::vector<std::vector<Scalar>> elements;
        
        Rows(matrix_match[3].str(), elements);

        size_t rows = elements.size();
        size_t cols = elements.front().size();

        if (!std::all_of(std::next(std::cbegin(elements)), std::cend(elements), [&cols](std::vector<Scalar> const &element) -> bool { return element.size() == cols; })) {
            throw std::invalid_argument("Matrix is ill-formed: " + matrix_match[3].str());
        }
        
        std::vector<Scalar> flattened;

        for (std::vector<Scalar> const &element : elements) {
            std::copy(std::cbegin(element), std::cend(element), std::back_inserter(flattened));
        }

        std::string matrix_name = m_parser_context->NextMatrixName();


        Matrix matrix(rows, cols, flattened);
        
        m_node_map.emplace(matrix_name, matrix);

        return Matrices(matrix_match[1].str() + matrix_name + matrix_match[5].str());
    }

    return expression_str;
}

void ExpressionParser::Cols(std::string const &expression_str, std::vector<Scalar> &col_elements)
{
    std::regex matrix_col_regex("^(.*?)(\\&)(.*)$");
    std::smatch matrix_col_match;

    if (std::regex_search(std::cbegin(expression_str), std::cend(expression_str), matrix_col_match, matrix_col_regex)) {
        ExpressionParser expression_parser(matrix_col_match[1].str(), m_node_map, m_parser_context, false);

        col_elements.emplace_back(std::get<Scalar>(expression_parser.Parse()));

        Cols(matrix_col_match[3].str(), col_elements);
    }
    else {
        ExpressionParser expression_parser(expression_str, m_node_map, m_parser_context, false);

        col_elements.emplace_back(std::get<Scalar>(expression_parser.Parse()));
    }
}

void ExpressionParser::Rows(std::string const &expression_str, std::vector<std::vector<Scalar>> &row_elements)
{
    std::regex matrix_row_regex("^(.*?)(\\\\\\\\)(.*)$");
    std::smatch matrix_row_match;

    if (std::regex_search(std::cbegin(expression_str), std::cend(expression_str), matrix_row_match, matrix_row_regex)) {
        std::vector<Scalar> col_elements;

        Cols(matrix_row_match[1].str(), col_elements);
        
        row_elements.emplace_back(col_elements);

        Rows(matrix_row_match[3].str(), row_elements);
    }
    else {
        std::vector<Scalar> col_elements;

        Cols(expression_str, col_elements);

        row_elements.emplace_back(col_elements);
    }
}

std::variant<Scalar, Matrix> ExpressionParser::Brackets(std::string const &expression_str)
{
    std::regex bracket_regex("^(.*)(\\(|\\[|\\{)(.*?)(\\)|\\]|\\})(.*)$");
    std::regex function_regex("^(.*(\\+|\\-|\\*|\\/|\\^|\\(|\\[|\\{))(.*)$|^(.*)$");
    
    std::smatch bracket_match;
    std::smatch function_match;
    
    if (std::regex_search(std::cbegin(expression_str), std::cend(expression_str), bracket_match, bracket_regex)) {
        if ((bracket_match[2].str() == "(" && bracket_match[4].str() == ")") || 
            (bracket_match[2].str() == "[" && bracket_match[4].str() == "]") || 
            (bracket_match[2].str() == "{" && bracket_match[4].str() == "}")) {
            std::variant<Scalar, Matrix> expression_variant = Functions(bracket_match[3].str());

            std::string expression_name = m_parser_context->NextExpressionName();

            m_node_map.emplace(expression_name, expression_variant);

            return Brackets(bracket_match[1].str() + expression_name + bracket_match[5].str());
        }
        else {
            throw std::invalid_argument("Bracket mismatch: " + bracket_match[2].str() + " and " + bracket_match[4].str());
        }
    }
    
    return Functions(expression_str);
}

std::variant<Scalar, Matrix> ExpressionParser::Functions(std::string const &expression_str)
{
    std::regex function_regex("^(.*)(cos|sin|tan|acos|asin|atan|sqrt|abs|exp|ln|det|inv)(E_\\d+)(.*?)$|^(.*)(\\\\frac)(E_\\d+)(E_\\d+)(.*?)$");

    std::smatch function_match;

    if (std::regex_search(std::cbegin(expression_str), std::cend(expression_str), function_match, function_regex)) {    
        if (!function_match[2].str().empty()) {
            std::variant<Scalar, Matrix> arg_variant = m_node_map.at(function_match[3].str());
             
            std::variant<Scalar, Matrix> function_variant;

            if (function_match[2].str() == "cos") {
                function_variant = std::visit(CosVisitor{ }, arg_variant);
            }
            else if (function_match[2].str() == "sin") {
                function_variant = std::visit(SinVisitor{ }, arg_variant);;
            }
            else if (function_match[2].str() == "tan") {
                function_variant = std::visit(TanVisitor{ }, arg_variant);
            }
            else if (function_match[2].str() == "acos") {
                function_variant = std::visit(AcosVisitor{ }, arg_variant);
            }
            else if (function_match[2].str() == "asin") {
                function_variant = std::visit(AsinVisitor{ }, arg_variant);
            }
            else if (function_match[2].str() == "atan") {
                function_variant = std::visit(AtanVisitor{ }, arg_variant);
            }
            else if (function_match[2].str() == "sqrt") {
                function_variant = std::visit(SqrtVisitor{ }, arg_variant);
            }
            else if (function_match[2].str() == "abs") {
                function_variant = std::visit(AbsVisitor{ }, arg_variant);
            }
            else if (function_match[2].str() == "exp") {
                function_variant = std::visit(ExpVisitor{ }, arg_variant);
            }
            else if (function_match[2].str() == "ln") {
                function_variant = std::visit(LnVisitor{ }, arg_variant);
            }
            else if (function_match[2].str() == "det") {
                function_variant = std::visit(DeterminantVisitor{ }, arg_variant);
            }
            else if (function_match[2].str() == "inv") {
                function_variant = std::visit(InverseVisitor{ }, arg_variant);
            }
            else {
                throw std::invalid_argument("Unrecognized function: " + function_match[2].str());
            }
            
            std::string function_name = m_parser_context->NextFunctionName();

            m_node_map.emplace(function_name, function_variant);
            
            return Functions(function_match[1].str() + function_name + function_match[4].str());
        }
        else if (!function_match[6].str().empty()) {
            std::variant<Scalar, Matrix> lhs_arg_variant = m_node_map.at(function_match[7].str());
            std::variant<Scalar, Matrix> rhs_arg_variant = m_node_map.at(function_match[8].str());

            std::variant<Scalar, Matrix> function_variant;
            
            if (function_match[6].str() == "\\frac") {
                function_variant = std::visit(DivisionVisitor{ }, lhs_arg_variant, rhs_arg_variant);
            }
            else {
                throw std::invalid_argument("Unrecognized function: " + function_match[6].str());
            }

            std::string function_name = m_parser_context->NextFunctionName();

            m_node_map.emplace(function_name, function_variant);
            
            return Functions(function_match[5].str() + function_name + function_match[9].str());
        }
    }

    return Operators(expression_str);
}

std::variant<Scalar, Matrix> ExpressionParser::Operators(std::string const &expression_str)
{
    std::regex operator_regex("^(.*[^\\+\\-\\*\\/\\^])([\\+\\-])(.*)$|^(.*[^\\+\\-\\*\\/\\^])([\\*\\/])(.*)$|^(.*?[^\\+\\-\\*\\/\\^])([\\^])(.*)$|^([\\+\\-])(.*)$");
    
    std::smatch operator_match;

    if (std::regex_search(std::cbegin(expression_str), std::cend(expression_str), operator_match, operator_regex)) {
        if (operator_match[2].str() == "+") {
            std::variant<Scalar, Matrix> lhs_arg_variant = Operators(operator_match[1].str());
            std::variant<Scalar, Matrix> rhs_arg_variant = Operators(operator_match[3].str());

            return std::visit(AdditionVisitor{ }, lhs_arg_variant, rhs_arg_variant);
        }
        else if (operator_match[2].str() == "-") {
            std::variant<Scalar, Matrix> lhs_arg_variant = Operators(operator_match[1].str());
            std::variant<Scalar, Matrix> rhs_arg_variant = Operators(operator_match[3].str());

            return std::visit(SubtractionVisitor{ }, lhs_arg_variant, rhs_arg_variant);
        }
        else if (operator_match[5].str() == "*") {
            std::variant<Scalar, Matrix> lhs_arg_variant = Operators(operator_match[4].str());
            std::variant<Scalar, Matrix> rhs_arg_variant = Operators(operator_match[6].str());

            return std::visit(MultiplicationVisitor{ }, lhs_arg_variant, rhs_arg_variant);
        }
        else if (operator_match[5].str() == "/") {
            std::variant<Scalar, Matrix> lhs_arg_variant = Operators(operator_match[4].str());
            std::variant<Scalar, Matrix> rhs_arg_variant = Operators(operator_match[6].str());

            return std::visit(DivisionVisitor{ }, lhs_arg_variant, rhs_arg_variant);
        }
        else if (operator_match[8].str() == "^") {
            std::variant<Scalar, Matrix> lhs_arg_variant = Operators(operator_match[7].str());
            std::variant<Scalar, Matrix> rhs_arg_variant = Operators(operator_match[9].str());

            return std::visit(ExponentiationVisitor{ }, lhs_arg_variant, rhs_arg_variant);
        }
        else if (operator_match[10].str() == "+") {
            std::variant<Scalar, Matrix> arg_variant = Operators(operator_match[11].str());

            return std::visit(MultiplicationVisitor{ }, std::variant<Scalar, Matrix>(Scalar(std::shared_ptr<ConstantNode>(new ConstantNode(1.0)))), arg_variant);
        }
        else if (operator_match[10].str() == "-") {
            std::variant<Scalar, Matrix> arg_variant = Operators(operator_match[11].str());

            return std::visit(MultiplicationVisitor{ }, std::variant<Scalar, Matrix>(Scalar(std::shared_ptr<ConstantNode>(new ConstantNode(-1.0)))), arg_variant);
        }
    }

    return Nodes(expression_str);
}

std::variant<Scalar, Matrix> ExpressionParser::Nodes(std::string const &expression_str)
{
    try {
        ComplexParser complex_parser(expression_str);

        std::complex<double> value = complex_parser.Parse();

        std::string constant_name = m_parser_context->NextConstantName();

        std::variant<Scalar, Matrix> constant_variant = Scalar(new ConstantNode(value));

        m_node_map.emplace(constant_name, constant_variant);

        return constant_variant;
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

std::variant<Scalar, Matrix> ExpressionParser::AdditionVisitor::operator()(Scalar const &lhs, Scalar const &rhs)
{
    return Scalar(new AdditionNode({ lhs, rhs }));
}

std::variant<Scalar, Matrix> ExpressionParser::AdditionVisitor::operator()(Scalar const &lhs, Matrix const &rhs)
{
    Matrix matrix(rhs.Rows(), rhs.Cols());

    for (size_t i = 0; i < matrix.Rows(); ++i) {
        for (size_t j = 0; j < matrix.Cols(); ++j) {
            matrix(i, j) = Scalar(new AdditionNode({ lhs, rhs(i, j) }));
        }
    }
    
    return matrix;
}

std::variant<Scalar, Matrix> ExpressionParser::AdditionVisitor::operator()(Matrix const &lhs, Scalar const &rhs)
{
    Matrix matrix(lhs.Rows(), lhs.Cols());

    for (size_t i = 0; i < matrix.Rows(); ++i) {
        for (size_t j = 0; j < matrix.Cols(); ++j) {
            matrix(i, j) = Scalar(new AdditionNode({ lhs(i, j), rhs }));
        }
    }
    
    return matrix;
}

std::variant<Scalar, Matrix> ExpressionParser::AdditionVisitor::operator()(Matrix const &lhs, Matrix const &rhs)
{
    if (lhs.Rows() != rhs.Rows() || lhs.Cols() != rhs.Cols()) {
        throw std::invalid_argument("AdditionVisitor: Dimensions are not equal");
    }

    Matrix matrix(lhs.Rows(), lhs.Cols());

    for (size_t i = 0; i < matrix.Rows(); ++i) {
        for (size_t j = 0; j < matrix.Cols(); ++j) {
            matrix(i, j) = Scalar(new AdditionNode({ lhs(i, j), rhs(i, j) }));
        }
    }
    
    return matrix;
}

std::variant<Scalar, Matrix> ExpressionParser::SubtractionVisitor::operator()(Scalar const &lhs, Scalar const &rhs)
{
    return Scalar(new SubtractionNode({ lhs, rhs }));
}

std::variant<Scalar, Matrix> ExpressionParser::SubtractionVisitor::operator()(Scalar const &lhs, Matrix const &rhs)
{
    Matrix matrix(rhs.Rows(), rhs.Cols());

    for (size_t i = 0; i < matrix.Rows(); ++i) {
        for (size_t j = 0; j < matrix.Cols(); ++j) {
            matrix(i, j) = Scalar(new SubtractionNode({ lhs, rhs(i, j) }));
        }
    }
    
    return matrix;
}

std::variant<Scalar, Matrix> ExpressionParser::SubtractionVisitor::operator()(Matrix const &lhs, Scalar const &rhs)
{
    Matrix matrix(lhs.Rows(), lhs.Cols());

    for (size_t i = 0; i < matrix.Rows(); ++i) {
        for (size_t j = 0; j < matrix.Cols(); ++j) {
            matrix(i, j) = Scalar(new SubtractionNode({ lhs(i, j), rhs }));
        }
    }
    
    return matrix;
}

std::variant<Scalar, Matrix> ExpressionParser::SubtractionVisitor::operator()(Matrix const &lhs, Matrix const &rhs)
{
    if (lhs.Rows() != rhs.Rows() || lhs.Cols() != rhs.Cols()) {
        throw std::invalid_argument("SubtractionVisitor: Dimensions are not equal");
    }

    Matrix matrix(lhs.Rows(), lhs.Cols());

    for (size_t i = 0; i < matrix.Rows(); ++i) {
        for (size_t j = 0; j < matrix.Cols(); ++j) {
            matrix(i, j) = Scalar(new SubtractionNode({ lhs(i, j), rhs(i, j) }));
        }
    }
    
    return matrix;
}

std::variant<Scalar, Matrix> ExpressionParser::MultiplicationVisitor::operator()(Scalar const &lhs, Scalar const &rhs)
{
    return Scalar(new MultiplicationNode({ lhs, rhs }));
}

std::variant<Scalar, Matrix> ExpressionParser::MultiplicationVisitor::operator()(Scalar const &lhs, Matrix const &rhs)
{
    Matrix matrix(rhs.Rows(), rhs.Cols());

    for (size_t i = 0; i < matrix.Rows(); ++i) {
        for (size_t j = 0; j < matrix.Cols(); ++j) {
            matrix(i, j) = Scalar(new MultiplicationNode({ lhs, rhs(i, j) }));
        }
    }
    
    return matrix;
}

std::variant<Scalar, Matrix> ExpressionParser::MultiplicationVisitor::operator()(Matrix const &lhs, Scalar const &rhs)
{
    Matrix matrix(lhs.Rows(), lhs.Cols());

    for (size_t i = 0; i < matrix.Rows(); ++i) {
        for (size_t j = 0; j < matrix.Cols(); ++j) {
            matrix(i, j) = Scalar(new MultiplicationNode({ lhs(i, j), rhs }));
        }
    }
    
    return matrix;
}

std::variant<Scalar, Matrix> ExpressionParser::MultiplicationVisitor::operator()(Matrix const &lhs, Matrix const &rhs)
{
    if (lhs.Cols() != rhs.Rows()) {
        throw std::invalid_argument("MultiplicationVisitor: LHS cols must equal RHS rows");
    }

    Matrix matrix(lhs.Rows(), rhs.Cols());

    for (size_t i = 0; i < lhs.Rows(); ++i) {
        for (size_t j = 0; j < rhs.Cols(); ++j) {
            Scalar sum(new ConstantNode(0.0));

            for (size_t k = 0; k < lhs.Cols(); ++k) {
                Scalar product(new MultiplicationNode({ lhs(i, k), rhs(k, j) }));
                
                sum = Scalar(new AdditionNode({ sum, product }));
            }

            matrix(i, j) = sum;
        }
    }

    return matrix;
}

std::variant<Scalar, Matrix> ExpressionParser::DivisionVisitor::operator()(Scalar const &lhs, Scalar const &rhs)
{
    return Scalar(new DivisionNode({ lhs, rhs }));
}

std::variant<Scalar, Matrix> ExpressionParser::DivisionVisitor::operator()(Scalar const &lhs, Matrix const &rhs)
{
    throw std::invalid_argument("DivisionVisitor unrecognized arguments Scalar and Matrix");
}

std::variant<Scalar, Matrix> ExpressionParser::DivisionVisitor::operator()(Matrix const &lhs, Scalar const &rhs)
{
    throw std::invalid_argument("DivisionVisitor unrecognized arguments Matrix and Scalar");
}

std::variant<Scalar, Matrix> ExpressionParser::DivisionVisitor::operator()(Matrix const &lhs, Matrix const &rhs)
{
    throw std::invalid_argument("DivisionVisitor unrecognized arguments Matrix and Matrix");
}

std::variant<Scalar, Matrix> ExpressionParser::ExponentiationVisitor::operator()(Scalar const &lhs, Scalar const &rhs)
{
    return Scalar(new ExponentiationNode({ lhs, rhs }));
}

std::variant<Scalar, Matrix> ExpressionParser::ExponentiationVisitor::operator()(Scalar const &lhs, Matrix const &rhs)
{
    throw std::invalid_argument("ExponentiationVisitor unrecognized arguments Scalar and Matrix");
}

std::variant<Scalar, Matrix> ExpressionParser::ExponentiationVisitor::operator()(Matrix const &lhs, Scalar const &rhs)
{
    throw std::invalid_argument("ExponentiationVisitor unrecognized arguments Matrix and Scalar");
}

std::variant<Scalar, Matrix> ExpressionParser::ExponentiationVisitor::operator()(Matrix const &lhs, Matrix const &rhs)
{
    throw std::invalid_argument("ExponentiationVisitor unrecognized arguments Matrix and Matrix");
}

std::variant<Scalar, Matrix> ExpressionParser::CosVisitor::operator()(Scalar const &arg)
{
    return Scalar(std::shared_ptr<CosNode>(new CosNode({ arg })));
}

std::variant<Scalar, Matrix> ExpressionParser::CosVisitor::operator()(Matrix const &arg)
{
    Matrix matrix(arg.Rows(), arg.Cols());

    for (size_t i = 0; i < matrix.Rows(); ++i) {
        for (size_t j = 0; j < matrix.Cols(); ++j) {
            matrix(i, j) = Scalar(new CosNode({ arg(i, j) }));
        }
    }
    
    return matrix;
}

std::variant<Scalar, Matrix> ExpressionParser::SinVisitor::operator()(Scalar const &arg)
{
    return Scalar(new SinNode({ arg }));
}

std::variant<Scalar, Matrix> ExpressionParser::SinVisitor::operator()(Matrix const &arg)
{
    Matrix matrix(arg.Rows(), arg.Cols());

    for (size_t i = 0; i < matrix.Rows(); ++i) {
        for (size_t j = 0; j < matrix.Cols(); ++j) {
            matrix(i, j) = Scalar(new SinNode({ arg(i, j) }));
        }
    }
    
    return matrix;
}

std::variant<Scalar, Matrix> ExpressionParser::TanVisitor::operator()(Scalar const &arg)
{
    return Scalar(new TanNode({ arg }));
}

std::variant<Scalar, Matrix> ExpressionParser::TanVisitor::operator()(Matrix const &arg)
{
    Matrix matrix(arg.Rows(), arg.Cols());

    for (size_t i = 0; i < matrix.Rows(); ++i) {
        for (size_t j = 0; j < matrix.Cols(); ++j) {
            matrix(i, j) = Scalar(new TanNode({ arg(i, j) }));
        }
    }
    
    return matrix;
}

std::variant<Scalar, Matrix> ExpressionParser::AcosVisitor::operator()(Scalar const &arg)
{
    return Scalar(new AcosNode({ arg }));
}

std::variant<Scalar, Matrix> ExpressionParser::AcosVisitor::operator()(Matrix const &arg)
{
    Matrix matrix(arg.Rows(), arg.Cols());

    for (size_t i = 0; i < matrix.Rows(); ++i) {
        for (size_t j = 0; j < matrix.Cols(); ++j) {
            matrix(i, j) = Scalar(new AcosNode({ arg(i, j) }));
        }
    }
    
    return matrix;
}

std::variant<Scalar, Matrix> ExpressionParser::AsinVisitor::operator()(Scalar const &arg)
{
    return Scalar(std::shared_ptr<AsinNode>(new AsinNode({ arg })));
}

std::variant<Scalar, Matrix> ExpressionParser::AsinVisitor::operator()(Matrix const &arg)
{
    Matrix matrix(arg.Rows(), arg.Cols());

    for (size_t i = 0; i < matrix.Rows(); ++i) {
        for (size_t j = 0; j < matrix.Cols(); ++j) {
            matrix(i, j) = Scalar(new AsinNode({ arg(i, j) }));
        }
    }
    
    return matrix;
}

std::variant<Scalar, Matrix> ExpressionParser::AtanVisitor::operator()(Scalar const &arg)
{
    return Scalar(new AtanNode({ arg }));
}

std::variant<Scalar, Matrix> ExpressionParser::AtanVisitor::operator()(Matrix const &arg)
{
    Matrix matrix(arg.Rows(), arg.Cols());

    for (size_t i = 0; i < matrix.Rows(); ++i) {
        for (size_t j = 0; j < matrix.Cols(); ++j) {
            matrix(i, j) = Scalar(new AtanNode({ arg(i, j) }));
        }
    }
    
    return matrix;
}

std::variant<Scalar, Matrix> ExpressionParser::SqrtVisitor::operator()(Scalar const &arg)
{
    return Scalar(new SqrtNode({ arg }));
}

std::variant<Scalar, Matrix> ExpressionParser::SqrtVisitor::operator()(Matrix const &arg)
{
    Matrix matrix(arg.Rows(), arg.Cols());

    for (size_t i = 0; i < matrix.Rows(); ++i) {
        for (size_t j = 0; j < matrix.Cols(); ++j) {
            matrix(i, j) = Scalar(new SqrtNode({ arg(i, j) }));
        }
    }
    
    return matrix;
}

std::variant<Scalar, Matrix> ExpressionParser::AbsVisitor::operator()(Scalar const &arg)
{
    return Scalar(new AbsNode({ arg }));
}

std::variant<Scalar, Matrix> ExpressionParser::AbsVisitor::operator()(Matrix const &arg)
{
    Matrix matrix(arg.Rows(), arg.Cols());

    for (size_t i = 0; i < matrix.Rows(); ++i) {
        for (size_t j = 0; j < matrix.Cols(); ++j) {
            matrix(i, j) = Scalar(new AbsNode({ arg(i, j) }));
        }
    }
    
    return matrix;
}

std::variant<Scalar, Matrix> ExpressionParser::ExpVisitor::operator()(Scalar const &arg)
{
    return Scalar(new ExpNode({ arg }));
}

std::variant<Scalar, Matrix> ExpressionParser::ExpVisitor::operator()(Matrix const &arg)
{
    Matrix matrix(arg.Rows(), arg.Cols());

    for (size_t i = 0; i < matrix.Rows(); ++i) {
        for (size_t j = 0; j < matrix.Cols(); ++j) {
            matrix(i, j) = Scalar(new ExpNode({ arg(i, j) }));
        }
    }
    
    return matrix;
}

std::variant<Scalar, Matrix> ExpressionParser::LnVisitor::operator()(Scalar const &arg)
{
    return Scalar(new LnNode({ arg }));
}

std::variant<Scalar, Matrix> ExpressionParser::LnVisitor::operator()(Matrix const &arg)
{
    Matrix matrix(arg.Rows(), arg.Cols());

    for (size_t i = 0; i < matrix.Rows(); ++i) {
        for (size_t j = 0; j < matrix.Cols(); ++j) {
            matrix(i, j) = Scalar(new LnNode({ arg(i, j) }));
        }
    }
    
    return matrix;
}

ExpressionParser::SubmatrixVisitor::SubmatrixVisitor(size_t const &row, size_t const &col) : m_row(row), m_col(col)
{
}

std::variant<Scalar, Matrix> ExpressionParser::SubmatrixVisitor::operator()(Scalar const &arg)
{
    throw std::invalid_argument("SubmatrixVisitor unrecognized argument Scalar");
}

std::variant<Scalar, Matrix> ExpressionParser::SubmatrixVisitor::operator()(Matrix const &arg)
{
    return arg.Submatrix(m_row, m_col);
}

std::variant<Scalar, Matrix> ExpressionParser::TransposeVisitor::operator()(Scalar const &arg)
{
    throw std::invalid_argument("TransposeVisitor unrecognized argument Scalar");
}

std::variant<Scalar, Matrix> ExpressionParser::TransposeVisitor::operator()(Matrix const &arg)
{
    return arg.Transpose();
}

ExpressionParser::MinorVisitor::MinorVisitor(size_t const &row, size_t const &col) : m_row(row), m_col(col)
{
}

std::variant<Scalar, Matrix> ExpressionParser::MinorVisitor::operator()(Scalar const &arg)
{
    throw std::invalid_argument("MinorVisitor unrecognized argument Scalar");
}

std::variant<Scalar, Matrix> ExpressionParser::MinorVisitor::operator()(Matrix const &arg)
{
    return arg.Minor(m_row, m_col);
}

std::variant<Scalar, Matrix> ExpressionParser::DeterminantVisitor::operator()(Scalar const &arg)
{
    throw std::invalid_argument("DeterminantVisitor unrecognized argument Scalar");
}

std::variant<Scalar, Matrix> ExpressionParser::DeterminantVisitor::operator()(Matrix const &arg)
{
    return arg.Determinant();
}

std::variant<Scalar, Matrix> ExpressionParser::CofactorVisitor::operator()(Scalar const &arg)
{
    throw std::invalid_argument("CofactorVisitor unrecognized argument Scalar");
}

std::variant<Scalar, Matrix> ExpressionParser::CofactorVisitor::operator()(Matrix const &arg)
{
    return arg.Cofactor();
}

std::variant<Scalar, Matrix> ExpressionParser::InverseVisitor::operator()(Scalar const &arg)
{
    throw std::invalid_argument("InverseVisitor unrecognized argument Scalar");
}

std::variant<Scalar, Matrix> ExpressionParser::InverseVisitor::operator()(Matrix const &arg)
{
    return arg.Inverse();
}
