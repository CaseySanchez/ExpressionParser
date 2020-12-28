/*
 * Copyright 2020 Casey Sanchez
 */

#pragma once

#include <vector>
#include <complex>
#include <cmath>
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <variant>

#include "node.hpp"

class Matrix
{
    size_t m_rows;
    size_t m_cols;

    std::vector<Scalar> m_elements;
    
public:
    static Matrix Identity(size_t const &dim);

    Matrix();
    Matrix(size_t const &rows, size_t const &cols);
    Matrix(size_t const &rows, size_t const &cols, std::vector<Scalar> const &elements);

    size_t Rows() const;
    size_t Cols() const;

    Scalar &operator()(size_t const &row, size_t const &col);
    Scalar operator()(size_t const &row, size_t const &col) const;
    
    /*
    Matrix Pow(int32_t const &other) const;
    */

    Matrix Submatrix(size_t const &row, size_t const &col) const;
    Matrix Transpose() const;
    Scalar Minor(size_t const &row, size_t const &col) const;
    Scalar Determinant() const;
    Matrix Cofactor() const;
    Matrix Inverse() const;

    friend std::ostream &operator<<(std::ostream &ostream, Matrix const &matrix);
};
