#include "matrix.hpp"
#include "conversions.hpp"
#include <iostream>

custom::matrix operator*(const custom::matrix & leftMatrix_in, const custom::matrix & rightMatrix_in)
{
    custom::matrix matrix_out = {0.0};
    for(int row = 0; row < AXIS; ++row)
        for(int col = 0; col < AXIS; ++col)
            for(int inner = 0; inner < AXIS; ++inner)
                matrix_out[row][col] += leftMatrix_in[row][inner] * rightMatrix_in[inner][col];
    return matrix_out;
}

custom::vector operator*(const custom::matrix & matrix_in, const custom::vector & vector_in)
{
    custom::vector vector_out = {0.0};
    for(int row = 0; row < AXIS; ++row)
        for(int inner = 0; inner < AXIS; ++inner)
            vector_out[row] += matrix_in[row][inner] * vector_in[inner];
    return vector_out;
}

custom::vector operator+(const custom::vector & leftVector_in, const custom::vector & rightVector_in)
{
    custom::vector vector_out = {0.0};
    for(int inner = 0; inner < AXIS; ++inner)
        vector_out[inner] = leftVector_in[inner] + rightVector_in[inner];
    return vector_out;
}

custom::vector & operator+=(custom::vector & leftVector_in, const custom::vector & rightVector_in)
{
    leftVector_in = leftVector_in + rightVector_in;
    return leftVector_in;
}

custom::vector operator-(const custom::vector & leftVector_in, const custom::vector & rightVector_in)
{
    custom::vector vector_out = {0.0};
    for(int inner = 0; inner < AXIS; ++inner)
        vector_out[inner] = leftVector_in[inner] - rightVector_in[inner];
    return vector_out;
}

custom::vector operator-(const custom::vector & vector_in)
{
    const custom::vector zeroVector = {0};
    return zeroVector - vector_in;
}

double findAbsValue(const custom::vector & vector_in)
{
    double out = 0.0;
    for(int inner = 0; inner < AXIS; ++inner)
        out += vector_in[inner] * vector_in[inner];
    return sqrt(out);
}

custom::matrix trasposeMatrix(const custom::matrix & matrix_in)
{
    custom::matrix matrix_out = {0};
    for(int row = 0; row < AXIS; ++row)
        for(int col = 0; col < AXIS; ++col)
            matrix_out[col][row] = matrix_in[row][col];
    return matrix_out;
}

custom::matrix rotationMatrixX(const double & alpha)
{
    custom::matrix matrix_out = {0.0};
    matrix_out[X][X] = 1.0;
    matrix_out[Y][Y] = cos(degToRad(alpha));
    matrix_out[Z][Z] = cos(degToRad(alpha));
    matrix_out[Y][Z] = - sin(degToRad(alpha));
    matrix_out[Z][Y] = sin(degToRad(alpha));
    return matrix_out;
}

custom::matrix rotationMatrixY(const double & betta)
{
    custom::matrix matrix_out = {0.0};
    matrix_out[X][X] = cos(degToRad(betta));
    matrix_out[Y][Y] = 1.0;
    matrix_out[Z][Z] = cos(degToRad(betta));
    matrix_out[X][Z] = sin(degToRad(betta));
    matrix_out[Z][X] = - sin(degToRad(betta));
    return matrix_out;
}

custom::matrix rotationMatrixZ(const double & gamma)
{
    custom::matrix matrix_out = {0.0};
    matrix_out[X][X] = cos(degToRad(gamma));
    matrix_out[Y][Y] = cos(degToRad(gamma));
    matrix_out[Z][Z] = 1.0;
    matrix_out[X][Y] = - sin(degToRad(gamma));
    matrix_out[Y][X] = sin(degToRad(gamma));
    return matrix_out;
}

custom::matrix compositeRotationMatrix(const double & alpha, const double & betta, const double & gamma)
{
    custom::matrix matrix_out = {0.0};
    matrix_out = rotationMatrixZ(gamma) * rotationMatrixY(betta) * rotationMatrixX(alpha);
    return matrix_out;
}

void printMatrix(const custom::matrix & matrix_in)
{
    for(int row = 0; row < AXIS; ++row)
    {
        for(int col = 0; col < AXIS; ++col)
            std::cout << matrix_in[row][col] << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void printVector(const custom::vector & vector_in)
{
    for(int col = 0; col < AXIS; ++col)
        std::cout << vector_in[col] << " ";
    std::cout << std::endl << std::endl;
}
