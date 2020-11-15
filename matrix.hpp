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

class Node;

class Matrix
{
    size_t m_rows;
    size_t m_cols;

    std::vector<std::shared_ptr<Node>> m_elements;
    
public:
    Matrix();
    Matrix(size_t const &rows, size_t const &cols);
    Matrix(size_t const &rows, size_t const &cols, std::vector<std::shared_ptr<Node>> const &elements);

    size_t Rows() const;
    size_t Cols() const;

    std::shared_ptr<Node> &operator()(size_t const &row, size_t const &col);
    std::shared_ptr<Node> operator()(size_t const &row, size_t const &col) const;
    
    Matrix operator+(Matrix const &other) const;
    Matrix operator+(std::complex<double> const &other) const;
    Matrix operator-(Matrix const &other) const;
    Matrix operator-(std::complex<double> const &other) const;
    Matrix operator*(Matrix const &other) const;
    Matrix operator*(std::complex<double> const &other) const;

    std::shared_ptr<Node> Minor(size_t const &row, size_t const &col) const;
    std::shared_ptr<Node> Determinant() const;
    Matrix Submatrix(size_t const &row, size_t const &col) const;
    Matrix Transpose() const;
    Matrix Inverse() const;

    friend std::ostream &operator<<(std::ostream &ostream, Matrix const &matrix);
    friend std::ostream &operator<<(std::ostream &ostream, std::shared_ptr<Matrix> const &matrix_ptr);
};
