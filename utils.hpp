/*
 * Copyright 2020 Casey Sanchez
 */

#pragma once

#include <cmath>
#include <complex>

bool Approximately(std::complex<double> const &lhs, std::complex<double> const &rhs, double const epsilon = 1e-5);