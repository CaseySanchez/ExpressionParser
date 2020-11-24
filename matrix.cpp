/*
* Copyright 2020 Casey Sanchez
*/

#include "matrix.hpp"
#include "node.hpp"
#include "operations.hpp"

Matrix::Matrix()
{
}

Matrix::Matrix(size_t const &rows, size_t const &cols) : m_rows(rows), m_cols(cols), m_elements(rows * cols)
{
}

Matrix::Matrix(size_t const &rows, size_t const &cols, std::vector<std::shared_ptr<Node>> const &elements) : m_rows(rows), m_cols(cols), m_elements(elements)
{
}

size_t Matrix::Rows() const
{
    return m_rows;
}

size_t Matrix::Cols() const
{
    return m_cols;
}

std::shared_ptr<Node> &Matrix::operator()(size_t const &row, size_t const &col)
{
    return m_elements[row * m_cols + col];
}

std::shared_ptr<Node> Matrix::operator()(size_t const &row, size_t const &col) const
{
    return m_elements[row * m_cols + col];
}

Matrix Matrix::operator+(Matrix const &other) const
{
    if (Rows() != other.Rows() || Cols() != other.Cols()) {
        throw std::invalid_argument("Dimensions are not equal");
    }

    Matrix matrix(Rows(), Cols());

    for (size_t i = 0; i < Rows(); ++i) {
        for (size_t j = 0; j < Cols(); ++j) {
            matrix(i, j) = std::shared_ptr<Addition>(new Addition({ (*this)(i, j), other(i, j) }));
        }
    }

    return matrix;
}

Matrix Matrix::operator+(std::complex<double> const &other) const
{
    Matrix matrix(Rows(), Cols());

    std::shared_ptr<Constant> constant_ptr(new Constant(other));

    for (size_t i = 0; i < Rows(); ++i) {
        for (size_t j = 0; j < Cols(); ++j) {
            matrix(i, j) = std::shared_ptr<Addition>(new Addition({ (*this)(i, j), constant_ptr }));
        }
    }

    return matrix;
}

Matrix Matrix::operator-(Matrix const &other) const
{
    if (Rows() != other.Rows() || Cols() != other.Cols()) {
        throw std::invalid_argument("Dimensions are not equal");
    }

    Matrix matrix(Rows(), Cols());

    for (size_t i = 0; i < Rows(); ++i) {
        for (size_t j = 0; j < Cols(); ++j) {
            matrix(i, j) = std::shared_ptr<Subtraction>(new Subtraction({ (*this)(i, j), other(i, j) }));
        }
    }

    return matrix;
}

Matrix Matrix::operator-(std::complex<double> const &other) const
{
    Matrix matrix(Rows(), Cols());

    std::shared_ptr<Constant> constant_ptr(new Constant(other));

    for (size_t i = 0; i < Rows(); ++i) {
        for (size_t j = 0; j < Cols(); ++j) {
            matrix(i, j) = std::shared_ptr<Subtraction>(new Subtraction({ (*this)(i, j), constant_ptr }));
        }
    }

    return matrix;
}

Matrix Matrix::operator*(Matrix const &other) const
{
    if (Cols() != other.Rows()) {
        throw std::invalid_argument("LHS cols must equal RHS rows");
    }

    Matrix matrix(Rows(), other.Cols());

    for (size_t i = 0; i < Rows(); ++i) {
        for (size_t j = 0; j < other.Cols(); ++j) {
            std::shared_ptr<Node> sum(new Constant(0.0));

            for (size_t k = 0; k < Cols(); ++k) {
                std::shared_ptr<Multiplication> product(new Multiplication({ (*this)(i, k), other(k, j) }));
                
                sum = std::shared_ptr<Addition>(new Addition({ sum, product }));
            }

            matrix(i, j) = sum;
        }
    }

    return matrix;
}

Matrix Matrix::operator*(std::complex<double> const &other) const
{
    Matrix matrix(Rows(), Cols());

    std::shared_ptr<Constant> constant_ptr(new Constant(other));

    for (size_t i = 0; i < Rows(); ++i) {
        for (size_t j = 0; j < Cols(); ++j) {
            matrix(i, j) = std::shared_ptr<Multiplication>(new Multiplication({ (*this)(i, j), constant_ptr }));
        }
    }

    return matrix; 
}

Matrix Matrix::operator^(int32_t const &other) const
{
    if (Rows() != Cols()) {
        throw std::invalid_argument("Matrix is not square");
    }

    if (other == 0) {
        Matrix identity(Rows(), Cols());

        for (size_t i = 0; i < Rows(); ++i) {
            for (size_t j = 0; j < Cols(); ++j) {
                if (i == j) {
                    identity(i, j) = std::shared_ptr<Constant>(new Constant(1.0));
                }
                else {
                    identity(i, j) = std::shared_ptr<Constant>(new Constant(0.0));
                }
            }
        }

        return identity;
    }
    else if (other > 0) {
        Matrix pow = (*this);

        for (size_t i = 1; i < other; ++i) {
            pow = pow * (*this);
        }

        return pow;
    }
    else {
        return Inverse() ^ (other * -1);
    }
}

std::shared_ptr<Node> Matrix::Minor(size_t const &row, size_t const &col) const
{
    if (Rows() != Cols()) {
        throw std::invalid_argument("Matrix is not square");
    }

    Matrix submatrix = Submatrix(row, col);

    return submatrix.Determinant();
}

std::shared_ptr<Node> Matrix::Determinant() const
{
    if (Rows() != Cols()) {
        throw std::invalid_argument("Matrix is not square");
    }

    if (Rows() == 1) {
        return (*this)(0, 0);
    }

    std::shared_ptr<Node> determinant(new Constant(0.0));

    for (size_t j = 0; j < Cols(); ++j) {
        std::shared_ptr<Multiplication> product(new Multiplication({ (*this)(0, j), Minor(0, j) }));

        product = std::shared_ptr<Multiplication>(new Multiplication({ product, std::shared_ptr<Constant>(new Constant(j % 2 == 0 ? 1.0 : -1.0)) }));

        determinant = std::shared_ptr<Addition>(new Addition({ determinant, product }));
    }

    return determinant;
}

Matrix Matrix::Submatrix(size_t const &row, size_t const &col) const
{
    if (Rows() <= 1 || Cols() <= 1 || Rows() <= row || Cols() <= col) {
        throw std::out_of_range("Invalid submatrix indices");
    }

    Matrix matrix(Rows() - 1, Cols() - 1);

    for (size_t i = 0; i < Rows() - 1; ++i) {
        for (size_t j = 0; j < Cols() - 1; ++j) {
            matrix(i, j) = (*this)(i + (i >= row ? 1 : 0), j + (j >= col ? 1 : 0));
        }
    }

    return matrix;
}

Matrix Matrix::Transpose() const
{
    Matrix matrix(Cols(), Rows());

    for (size_t i = 0; i < Rows(); ++i) {
        for (size_t j = 0; j < Cols(); ++j) {
            matrix(j, i) = (*this)(i, j);
        }
    }

    return matrix;
}

Matrix Matrix::Inverse() const
{
    if (Rows() != Cols()) {
        throw std::invalid_argument("Matrix must be square");
    }

    std::shared_ptr<Node> determinant = Determinant();

    if (std::fabs(std::get<std::complex<double>>(determinant->Value())) < 10e-7) {
        throw std::invalid_argument("Matrix is singular");
    }

    std::shared_ptr<Division> determinant_inverse(new Division({ std::shared_ptr<Constant>(new Constant(1.0)), determinant }));

    if (Rows() == 1) {
        Matrix matrix(1, 1);

        matrix(0, 0) = determinant_inverse;

        return matrix;
    }

    Matrix cofactor(Rows(), Cols());

    for (size_t i = 0; i < Rows(); ++i) {
        for (size_t j = 0; j < Cols(); ++j) {
            cofactor(i, j) = std::shared_ptr<Multiplication>(new Multiplication({ Minor(i, j), std::shared_ptr<Constant>(new Constant((i + j) % 2 == 0 ? 1.0 : -1.0)) })); 
        }
    }

    Matrix determinant_matrix(Rows(), Cols());

    for (size_t i = 0; i < Rows(); ++i) {
        for (size_t j = 0; j < Cols(); ++j) {
            if (i == j) {
                determinant_matrix(i, j) = determinant_inverse;
            }
            else {
                determinant_matrix(i, j) = std::shared_ptr<Constant>(new Constant(0.0));
            }
        }
    }

    return cofactor.Transpose() * determinant_matrix;
}

std::ostream &operator<<(std::ostream &ostream, Matrix const &matrix)
{
    ostream << "\\begin{bmatrix}";

    for (size_t i = 0; i < matrix.Rows(); ++i) {
        for (size_t j = 0; j < matrix.Cols(); ++j) {
            ostream << matrix(i, j);

            if (j < matrix.Cols() - 1) {
                ostream << "&";
            }
        }

        if (i < matrix.Rows() - 1) {
            ostream << "\\\\";
        }
    }

    ostream << "\\end{bmatrix}";

    return ostream;
}

std::ostream &operator<<(std::ostream &ostream, std::shared_ptr<Matrix> const &matrix_ptr)
{
    return ostream << *matrix_ptr;
}
