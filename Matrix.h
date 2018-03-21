#ifndef UBLAS_MATRIX_H
#define UBLAS_MATRIX_H

#include <algorithm>
#include <iostream>
#include <vector>


template<typename T>
class Matrix {
    std::vector<T> data;
    size_t rows;
    size_t cols;

public:
    Matrix() //default constructor
            : data(0), rows(0), cols(0) {}

    explicit Matrix(size_t size, T fill = 0)  //constructor
            : data(size * size, fill), rows(size), cols(size) {}

    Matrix(size_t rows, size_t cols, T fill = 0) //constructor
            : data(rows * cols, fill), rows(rows), cols(cols) {}

    Matrix(const Matrix &m) //copy constructor, included for completeness
            : data(m.data), rows(m.rows), cols(m.cols) {}

    Matrix(Matrix &&m) noexcept //move constructor
            : Matrix() {
        swap(*this, m);
    }

    ~Matrix() = default; //destructor, included for completeness

    Matrix &operator=(const Matrix &other) { //assignment operator
        Matrix temp(other);
        swap(*this, temp);
        return *this;
    }

    friend void swap(Matrix &first, Matrix &second) { //swap function
        std::swap(first.rows, second.rows);
        std::swap(first.cols, second.cols);
        std::swap(first.data, second.data);
    }

    template<typename F>
    Matrix<T> &map(F func) { //sets (*this)[i,j] = func((*this)[i][j])
        for (int r = 0; r < this->getRows(); r++)
            for (int c = 0; c < this->getCols(); c++)
                (*this)[r][c] = func((*this)[r][c]);
        return *this;
    }

    template<typename F>
    Matrix<T> &map(const Matrix<T> &other, F func) { //sets (*this)[i,j] = func((*this)[i][j], other[i][j])
        for (int r = 0; r < this->getRows(); r++)
            for (int c = 0; c < this->getCols(); c++)
                (*this)[r][c] = func((*this)[r][c], other[r][c]);
        return *this;
    }

    Matrix<T> &operator*=(const T other) { //compound multiplication by scalar quantity
        return this->map([other](T i) { return i * other; });
    }

    Matrix<T> &operator*=(const Matrix<T> &other) { //compound multiplication template specialization for Matrices
        return *this = *this * other;
    }

    Matrix<T> &operator+=(const Matrix<T> &other) { //compound addition
        return this->map(other, [](T i, T j) { return i + j; });
    }

    Matrix<T> &operator-=(const Matrix<T> &other) { //compound subtraction
        return this->map(other, [](T i, T j) { return i - j; });
    }

    friend std::ostream &operator<<(std::ostream &os, const Matrix &m) { //stream insertion
        for (int r = 0; r < m.rows; r++) {
            os << "[";
            for (int c = 0; c < m.cols; c++) {
                os << m[r][c];
                if (c != m.cols - 1)
                    os << ", ";
            }
            os << "]";
            if (r != m.rows - 1)
                os << "\n";
        }
        return os;
    }

    T *operator[](int index) { return &this->data[index * cols]; } //non-const subscript
    const T *operator[](int index) const { return &this->data[index * cols]; } //const subscript

    size_t getRows() const { return this->rows; } //get rows
    size_t getCols() const { return this->cols; } //get cols

    void setRows(size_t rows) { this->rows = rows; } //set rows
    void setCols(size_t cols) { this->cols = cols; } //set cols
};

template<typename T>
Matrix<T> operator*(const Matrix<T> &lhs, const Matrix<T> &rhs) { //multiplication by a Matrix
    Matrix<T> m(lhs.getRows(), rhs.getCols());
    for (int r = 0; r < m.getRows(); r++)
        for (int c = 0; c < m.getCols(); c++)
            for (int i = 0; i < rhs.getRows(); i++)
                m[r][c] += lhs[r][i] * rhs[i][c];
    return m;
}

template<typename T>
Matrix<T> operator*(Matrix<T> lhs, const T &rhs) { //multiplication by a scalar
    return lhs *= rhs;
}

template<typename T>
Matrix<T> operator+(Matrix<T> lhs, const Matrix<T> &rhs) { //binary addition
    return lhs += rhs;
}

template<typename T>
Matrix<T> operator-(Matrix<T> lhs, const Matrix<T> &rhs) { //binary subtraction
    return lhs -= rhs;
}

#endif //UBLAS_MATRIX_H