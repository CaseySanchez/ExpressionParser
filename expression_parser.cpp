#include "expression_parser.hpp"

ExpressionParser::ExpressionParser(std::string const &expression, std::map<std::string, std::shared_ptr<Term>> const &terms) : expression(expression), terms(terms), expression_id(0), function_id(0), constant_id(0)
{
    Clean();

    Verify();
}

std::shared_ptr<Term> ExpressionParser::Parse()
{
    return Brackets(expression);
}

void ExpressionParser::Clean()
{
    expression.erase(std::remove_if(std::begin(expression), std::end(expression), ::isspace), std::end(expression));

    if (expression.empty()) {
        throw std::invalid_argument("Expression is empty");
    }
}

void ExpressionParser::Verify()
{
    std::regex reserved_regex("E_[0-9]|F_[0-9]|C_[0-9]");

    std::smatch reserved_match;

    for (auto [term_name, term_ptr] : terms) {
        if (std::regex_search(std::cbegin(term_name), std::cend(term_name), reserved_match, reserved_regex)) {
            throw std::invalid_argument("Term name " + term_name + " is reserved");
        }
    }
}

std::shared_ptr<Term> ExpressionParser::Brackets(std::string const &expression)
{
    std::cout << expression << std::endl;

    std::regex bracket_regex("^(.*)([\\(\\[\\{])(.*?)([\\)\\]\\}])(.*)$");
    std::regex function_regex("^(.*[\\+\\-\\*\\/\\^\\(\\[\\{])?(.*)$");
    
    std::smatch bracket_match;
    std::smatch function_match;

    if (std::regex_search(std::cbegin(expression), std::cend(expression), bracket_match, bracket_regex)) {
        if ((bracket_match[2].str() == "(" && bracket_match[4].str() == ")") || 
            (bracket_match[2].str() == "[" && bracket_match[4].str() == "]") || 
            (bracket_match[2].str() == "{" && bracket_match[4].str() == "}")) {
            std::shared_ptr<Term> expression_ptr = Operations(bracket_match[3].str());

            if (std::regex_search(bracket_match[1].first, bracket_match[1].second, function_match, function_regex)) {
                if (!function_match[2].str().empty()) {
                    std::shared_ptr<Term> function_ptr;

                    if (function_match[2].str() == "cos") {
                        function_ptr = std::shared_ptr<Cos>(new Cos(expression_ptr));
                    }
                    else if (function_match[2].str() == "sin") {
                        function_ptr = std::shared_ptr<Sin>(new Sin(expression_ptr));
                    }
                    else if (function_match[2].str() == "tan") {
                        function_ptr = std::shared_ptr<Tan>(new Tan(expression_ptr));
                    }
                    else if (function_match[2].str() == "acos") {
                        function_ptr = std::shared_ptr<Acos>(new Acos(expression_ptr));
                    }
                    else if (function_match[2].str() == "asin") {
                        function_ptr = std::shared_ptr<Asin>(new Asin(expression_ptr));
                    }
                    else if (function_match[2].str() == "atan") {
                        function_ptr = std::shared_ptr<Atan>(new Atan(expression_ptr));
                    }
                    else if (function_match[2].str() == "sqrt") {
                        function_ptr = std::shared_ptr<Sqrt>(new Sqrt(expression_ptr));
                    }
                    else if (function_match[2].str() == "abs") {
                        function_ptr = std::shared_ptr<Abs>(new Abs(expression_ptr));
                    }
                    else if (function_match[2].str() == "exp") {
                        function_ptr = std::shared_ptr<Exp>(new Exp(expression_ptr));
                    }
                    else if (function_match[2].str() == "log") {
                        function_ptr = std::shared_ptr<Log>(new Log(expression_ptr));
                    }
                    else {
                        throw std::invalid_argument("Unrecognized function: " + function_match[2].str());
                    }
                    
                    std::string function_name = "F_" + std::to_string(function_id++);

                    terms.try_emplace(function_name, function_ptr);

                    return Brackets(function_match[1].str() + function_name + bracket_match[5].str());
                }
            }

            std::string expression_name = "E_" + std::to_string(expression_id++);

            terms.try_emplace(expression_name, expression_ptr);

            return Brackets(bracket_match[1].str() + expression_name + bracket_match[5].str());
        }
        else {
            throw std::invalid_argument("Bracket mismatch: " + bracket_match[2].str() + " and " + bracket_match[4].str());
        }
    }
    
    return Operations(expression);
}

std::shared_ptr<Term> ExpressionParser::Operations(std::string const &expression)
{
    std::regex operator_regex("^(.*[^\\+\\-\\*\\/\\^])([\\+\\-])(.*)$|^(.*[^\\+\\-\\*\\/\\^])([\\*\\/])(.*)$|^(.*?[^\\+\\-\\*\\/\\^])([\\^])(.*)$|^([\\+\\-])(.*)$");
    
    std::smatch operator_match;

    if (std::regex_search(std::cbegin(expression), std::cend(expression), operator_match, operator_regex)) {
        if (operator_match[2].str() == "+") {
            std::shared_ptr<Term> lhs_ptr = Operations(operator_match[1].str());
            std::shared_ptr<Term> rhs_ptr = Operations(operator_match[3].str());

            return std::shared_ptr<Addition>(new Addition(lhs_ptr, rhs_ptr));
        }
        else if (operator_match[2].str() == "-") {
            std::shared_ptr<Term> lhs_ptr = Operations(operator_match[1].str());
            std::shared_ptr<Term> rhs_ptr = Operations(operator_match[3].str());

            return std::shared_ptr<Subtraction>(new Subtraction(lhs_ptr, rhs_ptr));
        }
        else if (operator_match[5].str() == "*") {
            std::shared_ptr<Term> lhs_ptr = Operations(operator_match[4].str());
            std::shared_ptr<Term> rhs_ptr = Operations(operator_match[6].str());

            return std::shared_ptr<Multiplication>(new Multiplication(lhs_ptr, rhs_ptr));
        }
        else if (operator_match[5].str() == "/") {
            std::shared_ptr<Term> lhs_ptr = Operations(operator_match[4].str());
            std::shared_ptr<Term> rhs_ptr = Operations(operator_match[6].str());

            return std::shared_ptr<Division>(new Division(lhs_ptr, rhs_ptr));
        }
        else if (operator_match[8].str() == "^") {
            std::shared_ptr<Term> lhs_ptr = Operations(operator_match[7].str());
            std::shared_ptr<Term> rhs_ptr = Operations(operator_match[9].str());

            return std::shared_ptr<Exponentiation>(new Exponentiation(lhs_ptr, rhs_ptr));
        }
        else if (operator_match[10].str() == "+") {
            std::shared_ptr<Term> term_ptr = Operations(operator_match[11].str());

            return std::shared_ptr<Affirmation>(new Affirmation(term_ptr));
        }
        else if (operator_match[10].str() == "-") {
            std::shared_ptr<Term> term_ptr = Operations(operator_match[11].str());

            return std::shared_ptr<Negation>(new Negation(term_ptr));
        } 
    }

    return Terms(expression);
}

std::shared_ptr<Term> ExpressionParser::Terms(std::string const &term)
{
    try {
        double value = std::stod(term);

        std::string constant_name = "C_" + std::to_string(constant_id++);
        
        std::shared_ptr<Constant> constant_ptr(new Constant(value));

        terms.try_emplace(constant_name, constant_ptr);

        return constant_ptr;
    }
    catch (std::invalid_argument const &) {
        try {
            return terms.at(term);
        }
        catch(std::out_of_range const &) {
            throw std::invalid_argument("No term provided for: " + term);
        }
    }
}
