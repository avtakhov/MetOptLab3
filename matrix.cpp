#include "matrix.h"

matrix_t::real_t vector_matrix::get(size_t i, size_t j) const
{
    return values[i][j];
}

size_t vector_matrix::ncolumns() const
{
    return values.size() > 0 ? values[0].size() : 0;
}

size_t vector_matrix::nrows() const
{
    return values.size();
}

vector_matrix::vector_matrix(matrix_t const& other)
    : values(other.nrows(), std::vector<matrix_t::real_t>(other.ncolumns()))
{
    for (size_t i = 0; i < other.nrows(); ++i)
    {
        for (size_t j = 0; j < other.ncolumns(); ++j)
        {
            values[i][j] = other.get(i , j);
        }
    }
}

vector_matrix::vector_matrix(std::vector<std::vector<matrix_t::real_t>> const& values)
    : values(values)
{}
