#include "lu.h"
#include "profile.h"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <cassert>

profile_matrix::profile_matrix() = default;

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

profile_matrix::value_type profile_matrix::get(size_t r, size_t c) const
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
    size_t size;
    size_t r;
    size_t c;
    is >> size >> r >> c;
    copy_from(is, size, matrix.diag);
    copy_from(is, r, matrix.rows.values);
    copy_from(is, size, matrix.rows.separators);
    copy_from(is, c, matrix.columns.values);
    copy_from(is, size, matrix.columns.separators);
    return is;
}

std::ostream& operator<<(std::ostream& os, profile_matrix const& matrix)
{
    os << matrix.size() << " " << matrix.rows.values.size() << " " << matrix.columns.values.size() << std::endl;
    copy_to(os, matrix.diag);
    copy_to(os, matrix.rows.values);
    copy_to(os, matrix.rows.separators);
    copy_to(os, matrix.columns.values);
    copy_to(os, matrix.columns.separators);
    return os;
}


std::optional<std::reference_wrapper<profile_matrix::value_type>> profile_matrix::get_reference(size_t row, size_t column)
{
    if (column == row)
    {
        return diag[row];
    }
    else if (column < row)
    {
        size_t size = std::distance(rows[row].begin(), rows[row].end());
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
        size_t size = std::distance(columns[column].begin(), columns[column].end());
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

profile_matrix::profile_matrix(matrix_t const& matrix)
    : diag(matrix.nrows())
{
    assert(matrix.nrows() == matrix.ncolumns());
    std::vector<matrix_t::value_type> row_values;
    std::vector<size_t> row_separators;
    std::vector<matrix_t::value_type> column_values;
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

profile_block<profile> profile::operator[](size_t index)
{
    return profile_block<profile>(index, *this);
}

profile_block<profile const> profile::operator[](size_t index) const
{
    return profile_block<profile const>(index, *this);
}
