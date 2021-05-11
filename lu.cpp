#include "lu.h"

size_t LU::nrows() const
{
    return matrix->nrows();
}

size_t LU::ncolumns() const
{
    return matrix->ncolumns();
}

matrix_t::real_t L::get(size_t r, size_t c) const
{
    if (r < c)
    {
        return 0;
    }
    else
    {
        return matrix->get(r, c);
    }
}

matrix_t::real_t U::get(size_t r, size_t c) const
{
    if (r > c)
    {
        return 0;
    }
    else if (r == c)
    {
        return 1;
    }
    else
    {
        return matrix->get(r, c);
    }
}
