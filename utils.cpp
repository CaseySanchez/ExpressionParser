/*
 * Copyright 2020 Casey Sanchez
 */

#include "utils.hpp"

bool Approximately(std::complex<double> const &lhs, std::complex<double> const &rhs, double const epsilon)
{
    return std::fabs(lhs.real() - rhs.real()) <= epsilon && std::fabs(lhs.imag() - rhs.imag()) <= epsilon;
}