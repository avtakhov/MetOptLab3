#include "lu.h"
#include "profile.h"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <cassert>

profile_matrix::profile_matrix()
{
}

size_t profile_matrix::size() const
{
    return diag.size();
}

size_t profile_matrix::nrows() const
{
    return size();
}


size_t profile_matrix::ncolumns() const
{
    return size();
}

profile_matrix::real_t profile_matrix::get(size_t r, size_t c) const
{
    auto res = const_cast<profile_matrix*>(this)->get_reference(r, c);
    return res ? res->get() : 0;
}

template <typename Collection>
void copy_from(std::istream& is, size_t n, Collection& collection)
{
    std::copy_n(std::istream_iterator<typename Collection::value_type>(is), n, std::back_inserter(collection));
}

template <typename Collection>
void copy_to(std::ostream& os, Collection const& collecion)
{
    std::copy(collecion.begin(), collecion.end(), std::ostream_iterator<typename Collection::value_type>(os, " "));
    os << std::endl;
}

std::istream& operator>>(std::istream& is, profile_matrix& matrix)
{
    size_t size, nrows, ncolumns;
    is >> size >> nrows >> ncolumns;
    copy_from(is, size, matrix.diag);
    copy_from(is, size, matrix.column_separators);
    return is;
}

std::ostream& operator<<(std::ostream& os, profile_matrix const& matrix)
{
    copy_to(os, matrix.diag);
    copy_to(os, matrix.column_separators);
    return os;
}


std::optional<std::reference_wrapper<profile_matrix::real_t>> profile_matrix::get_reference(size_t row, size_t column)
{
    if (column == row)
    {
        return diag[row];
    }
    else if (column < row)
    {
        auto size = std::distance(rows[row].begin(), rows[row].end());
        if (column < row - size)
        {
            return std::nullopt;
        }
        else
        {
            return *std::next(rows[row].begin(), column - (row - size));
        }
    }
    else
    {
        auto size = std::distance(columns[column].begin(), columns[column].end());
        if (row < column - size)
        {
            return std::nullopt;
        }
        else
        {
            return *std::next(columns[column].begin(), row - (column - size));
        }
    }
}

std::pair<std::unique_ptr<matrix_t>, std::unique_ptr<matrix_t>> LU(profile_matrix&& matrix)
{

    for (size_t i = 0; i < matrix.size(); ++i)
    {
        for (size_t j = 0; j < i; ++j) {
            auto lij = matrix.get_reference(i, j);
            if (lij.has_value())
            {
                for (size_t k = 0; k < j; ++k)
                {
                    lij->get() -= matrix.get(i, k) * matrix.get(k, j);
                }
            }

            auto uji = matrix.get_reference(j, i);

            if (uji.has_value())
            {
                for (size_t k = 0; k < j; ++k)
                {
                    uji->get() -= matrix.get(j, k) * matrix.get(k, i);
                }
                uji->get() /= matrix.get(j, j);
            }
        }


        for (size_t k = 0; k < i; ++k)
        {
            matrix.get_reference(i, i)->get() -= matrix.get(i, k) * matrix.get(k, i);
        }
    }

    auto profile_ptr = std::make_shared<profile_matrix>(std::move(matrix));
    return { std::make_unique<L>(profile_ptr), std::make_unique<U>(profile_ptr) };
}

profile_matrix::profile_matrix(matrix_t const& matrix)
    : diag(matrix.nrows())
{
    assert(matrix.nrows() == matrix.ncolumns());
    std::vector<matrix_t::real_t> row_values;
    std::vector<size_t> row_separators;
    std::vector<matrix_t::real_t> column_values;
    std::vector<size_t> column_separators;

    for (size_t i = 0; i < matrix.nrows(); ++i)
    {
        diag[i] = matrix.get(i, i);

        for (size_t j = 0; j < i; ++j)
        {
            row_values.push_back(matrix.get(i, j));
            column_values.push_back(matrix.get(j, i));
        }
        row_separators.push_back(row_values.size());
        column_separators.push_back(column_values.size());
    }
    rows = profile(row_values, row_separators);
    columns = profile(column_values, column_separators);
}


