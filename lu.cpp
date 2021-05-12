#include "lu.h"

size_t LU::nrows() const
{
    return matrix->nrows();
}

size_t LU::ncolumns() const
{
    return matrix->ncolumns();
}

matrix_t::value_type L::get(size_t r, size_t c) const
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

matrix_t::value_type U::get(size_t r, size_t c) const
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

std::pair<std::unique_ptr<matrix_t>, std::unique_ptr<matrix_t>> LU(profile_matrix& matrix)
{
    for (size_t i = 0; i < matrix.size(); ++i)
    {

        auto start = [&](size_t index, profile const& p)
        {
            return index - std::distance(p[index].begin(), p[index].end());
        };

        auto max_start = [&](size_t i, size_t j)
        {
            return std::max(start(i, matrix.rows), start(j, matrix.columns));
        };

        for (size_t j = start(i, matrix.rows); j < i; ++j)
        {
            auto lij = matrix.get_reference(i, j);
            for (size_t k = max_start(i, j); k < j; ++k)
            {
                lij->get() -= matrix.get(i, k) * matrix.get(k, j);
            }
        }

        for (size_t j = start(i, matrix.columns); j < i; ++j)
        {
            auto uji = matrix.get_reference(j, i);
            for (size_t k = max_start(j, i); k < j; ++k)
            {
                uji->get() -= matrix.get(j, k) * matrix.get(k, i);
            }
            uji->get() /= matrix.get(j, j);
        }

        for (size_t k = max_start(i, i); k < i; ++k)
        {
            matrix.get_reference(i, i)->get() -= matrix.get(i, k) * matrix.get(k, i);
        }
    }

    auto profile_ptr = std::make_shared<profile_matrix>(std::move(matrix));
    return { std::make_unique<L>(profile_ptr), std::make_unique<U>(profile_ptr) };
}
