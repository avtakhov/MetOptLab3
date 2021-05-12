#include "matrix.h"

bool operator==(matrix_t const& a, matrix_t const& b)
{
    if (a.nrows() != b.nrows() || a.ncolumns() != b.ncolumns())
    {
        return false;
    }
    for (size_t i = 0; i < a.nrows(); ++i)
    {
        for (size_t j = 0; j < a.ncolumns(); ++j)
        {
            if (a.get(i, j) != b.get(i, j))
            {
                return false;
            }
        }
    }
    return true;
}

bool operator!=(matrix_t const& a, matrix_t const& b)
{
    return !(a == b);
}


std::ostream& operator<<(std::ostream& os, matrix_t const& m)
{
    for (size_t i = 0; i < m.nrows(); ++i)
    {
        for (size_t j = 0; j < m.ncolumns(); ++j)
        {
            os << m.get(i, j) << " ";
        }
        os << std::endl;
    }
    return os;
}
