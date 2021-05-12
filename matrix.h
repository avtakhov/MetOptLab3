#ifndef MATRIX_H
#define MATRIX_H

#include <cstdio>
#include <ostream>

struct matrix_t
{
    using value_type = double;

    virtual value_type get(size_t, size_t) const = 0;

    virtual size_t nrows() const = 0;

    virtual size_t ncolumns() const = 0;

    virtual ~matrix_t() = default;
};

bool operator==(matrix_t const&, matrix_t const&);

bool operator!=(matrix_t const&, matrix_t const&);

std::ostream& operator<<(std::ostream&, matrix_t const&);
#endif // MATRIX_H
