/*
 * Copyright 2020 Casey Sanchez
 */

#pragma once

#include <string>
#include <complex>
#include <regex>

class ComplexParser
{
    std::string m_complex_str;

public:
    ComplexParser(std::string const &complex_str);

    std::complex<double> Parse();

private:
    void Clean();
};