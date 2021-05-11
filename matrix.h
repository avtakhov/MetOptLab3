#ifndef MATRIX_H
#define MATRIX_H

#include <cstdio>
#include <vector>

struct matrix_t
{
    using real_t = double;

    virtual real_t get(size_t, size_t) const = 0;

    virtual size_t nrows() const = 0;

    virtual size_t ncolumns() const = 0;

    virtual ~matrix_t() = default;
};

struct vector_matrix : public matrix_t
{
    real_t get(size_t i, size_t j) const override;

    size_t nrows() const override;

    size_t ncolumns() const override;

    vector_matrix(matrix_t const& other);

    vector_matrix(std::vector<std::vector<real_t>> const& values);

    ~vector_matrix() override = default;

private:
    std::vector<std::vector<real_t>> values;
};

#endif // MATRIX_H
