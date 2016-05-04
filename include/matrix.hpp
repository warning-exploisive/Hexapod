#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "constants.hpp"

custom::matrix operator*(const custom::matrix & leftMatrix_in, const custom::matrix & rightMatrix_in);
custom::vector operator*(const custom::matrix & m_in, const custom::vector & v_in);
custom::vector operator+(const custom::vector & leftVector_in, const custom::vector & rightVector_in);
custom::vector & operator+=(custom::vector & leftVector_in, const custom::vector & rightVector_in);
custom::vector operator-(const custom::vector & leftVector_in, const custom::vector & rightVector_in);
custom::vector operator-(const custom::vector & vector_in);
double findAbsValue(const custom::vector & vector_in);

custom::matrix trasposeMatrix(const custom::matrix & matrix_in);
custom::matrix rotationMatrixX(const double & alpha);
custom::matrix rotationMatrixY(const double & betta);
custom::matrix rotationMatrixZ(const double & gamma);
custom::matrix compositeRotationMatrix(const double & alpha, const double & betta, const double & gamma);

void printMatrix(const custom::matrix & matrix_in);
void printVector(const custom::vector & vector_in);

#endif // MATRIX_HPP
