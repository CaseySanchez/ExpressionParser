/*
 * Copyright 2020 Casey Sanchez
 */

#include "complex_parser.hpp"

ComplexParser::ComplexParser(std::string const &complex_str) : m_complex_str(complex_str)
{
    Clean();
}

std::complex<double> ComplexParser::Parse()
{
    std::regex complex_regex("^([+-]?[0-9]*\\.?[0-9]+([eE][+-]?[0-9]+)?)[+-]([+-]?[0-9]*\\.?[0-9]+([eE][+-]?[0-9]+)?)?(i)$");
    std::regex real_imag_regex("^([+-]?[0-9]*\\.?[0-9]+([eE][+-]?[0-9]+)?)(i)?$");

    std::smatch complex_match;
    std::smatch real_imag_match;

    if (std::regex_search(std::cbegin(m_complex_str), std::cend(m_complex_str), complex_match, complex_regex)) {
        return std::complex<double>(std::stod(complex_match[1].str()), !complex_match[4].str().empty() ? std::stod(complex_match[4].str()) : 1.0);
    }
    else if (std::regex_search(std::cbegin(m_complex_str), std::cend(m_complex_str), real_imag_match, real_imag_regex)) {
        if (!real_imag_match[3].str().empty()) {
            return std::complex<double>(0.0, std::stod(real_imag_match[1].str()));
        }
        else {
            return std::complex<double>(std::stod(real_imag_match[1].str()), 0.0);
        }
    }

    throw std::invalid_argument("\"" + m_complex_str + "\" does not contain a complex number");
}

void ComplexParser::Clean()
{
    m_complex_str.erase(std::remove_if(std::begin(m_complex_str), std::end(m_complex_str), ::isspace), std::end(m_complex_str));

    if (m_complex_str.empty()) {
        throw std::invalid_argument("Complex is empty");
    }
}