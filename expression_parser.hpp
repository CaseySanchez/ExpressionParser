#pragma once

#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <algorithm>
#include <regex>

#include "term.hpp"
#include "operation.hpp"
#include "function.hpp"

class ExpressionParser
{
    std::string expression;

    size_t expression_id;
    size_t function_id;
    
    std::map<std::string, std::shared_ptr<Term>> terms;

public:
    ExpressionParser(std::string const &expression, std::map<std::string, std::shared_ptr<Term>> const &variables);

    std::shared_ptr<Term> Parse();

private:
    void Clean();
    
    std::shared_ptr<Term> Brackets(std::string const &expression);
    std::shared_ptr<Term> Operations(std::string const &expression);
    std::shared_ptr<Term> Terms(std::string const &term);
};
