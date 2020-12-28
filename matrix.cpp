/*
* Copyright 2020 Casey Sanchez
*/

#include "matrix.hpp"
#include "node.hpp"
#include "operations.hpp"

Matrix Matrix::Identity(size_t const &dim)
{
    Matrix identity(dim, dim);

    for (size_t i = 0; i < dim; ++i) {
        for (size_t j = 0; j < dim; ++j) {
            if (i == j) {
                identity(i, j) = Scalar(new ConstantNode(1.0));
            }
            else {
                identity(i, j) = Scalar(new ConstantNode(0.0));
            }
        }
    }

    return identity;
}

Matrix::Matrix()
{
}

Matrix::Matrix(size_t const &rows, size_t const &cols) : m_rows(rows), m_cols(cols), m_elements(rows * cols)
{
}

Matrix::Matrix(size_t const &rows, size_t const &cols, std::vector<Scalar> const &elements) : m_rows(rows), m_cols(cols), m_elements(elements)
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

Scalar &Matrix::operator()(size_t const &row, size_t const &col)
{
    return m_elements[row * m_cols + col];
}

Scalar Matrix::operator()(size_t const &row, size_t const &col) const
{
    return m_elements[row * m_cols + col];
}

/*
Matrix Matrix::Pow(int32_t const &other) const
{
    if (Rows() != Cols()) {
        throw std::invalid_argument("Matrix is not square");
    }

    if (other == 0) {
        return Matrix::Identity(Rows());
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
*/

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

Scalar Matrix::Minor(size_t const &row, size_t const &col) const
{
    if (Rows() != Cols()) {
        throw std::invalid_argument("Matrix is not square");
    }

    return Submatrix(row, col).Determinant();
}

Scalar Matrix::Determinant() const
{
    if (Rows() != Cols()) {
        throw std::invalid_argument("Matrix is not square");
    }

    if (Rows() == 1) {
        return (*this)(0, 0);
    }

    Scalar determinant(new ConstantNode(0.0));

    for (size_t j = 0; j < Cols(); ++j) {
        Scalar product = Scalar(new MultiplicationNode({ Scalar(new MultiplicationNode({ (*this)(0, j), Minor(0, j) })), Scalar(new ConstantNode(j % 2 == 0 ? 1.0 : -1.0)) }));

        determinant = Scalar(new AdditionNode({ determinant, product }));
    }

    return determinant;
}

Matrix Matrix::Cofactor() const
{
    Matrix matrix(Rows(), Cols());

    for (size_t i = 0; i < Rows(); ++i) {
        for (size_t j = 0; j < Cols(); ++j) {
            matrix(i, j) = Scalar(new MultiplicationNode({ Minor(i, j), Scalar(new ConstantNode((i + j) % 2 == 0 ? 1.0 : -1.0)) })); 
        }
    }

    return matrix;
}

Matrix Matrix::Inverse() const
{
    if (Rows() != Cols()) {
        throw std::invalid_argument("Matrix must be square");
    }

    Scalar determinant = Determinant();

    if (std::fabs(determinant->Value()) < 1e-9) {
        throw std::invalid_argument("Matrix is singular");
    }

    Scalar determinant_inverse(new DivisionNode({ std::shared_ptr<ConstantNode>(new ConstantNode(1.0)), determinant }));

    if (Rows() == 1) {
        Matrix inverse(1, 1);

        inverse(0, 0) = determinant_inverse;

        return inverse;
    }

    Matrix cofactor = Cofactor();

    Matrix cofactor_transpose = cofactor.Transpose();
    
    Matrix determinant_matrix(Rows(), Cols());

    for (size_t i = 0; i < Rows(); ++i) {
        for (size_t j = 0; j < Cols(); ++j) {
            if (i == j) {
                determinant_matrix(i, j) = determinant_inverse;
            }
            else {
                determinant_matrix(i, j) = Scalar(new ConstantNode(0.0));
            }
        }
    }

    Matrix inverse(Rows(), Cols());

    for (size_t i = 0; i < Rows(); ++i) {
        for (size_t j = 0; j < Cols(); ++j) {
            Scalar sum(new ConstantNode(0.0));

            for (size_t k = 0; k < Cols(); ++k) {
                Scalar product(new MultiplicationNode({ cofactor_transpose(i, k), determinant_matrix(k, j) }));
                
                sum = Scalar(new AdditionNode({ sum, product }));
            }

            inverse(i, j) = sum;
        }
    }

    return inverse;
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
