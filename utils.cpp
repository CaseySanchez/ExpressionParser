/*
 * Copyright 2020 Casey Sanchez
 */

#include "utils.hpp"

bool Approximately(double const &lhs, double const &rhs, double const epsilon)
{
    return std::fabs(lhs - rhs) <= epsilon;
}