#ifndef LU_H
#define LU_H

#include "matrix.h"
#include "profile.h"
#include <memory>

struct LU : public matrix_t
{
    explicit LU(std::shared_ptr<profile_matrix> matrix)
        : matrix(std::move(matrix))
    {}

    size_t nrows() const override;

    size_t ncolumns() const override;

    matrix_t::value_type get(size_t, size_t) const override = 0;
protected:
    std::shared_ptr<profile_matrix> matrix;
};

struct L : public LU
{
    explicit L(std::shared_ptr<profile_matrix> matrix)
        : LU(matrix)
    {}

    matrix_t::value_type get(size_t i, size_t j) const override;
};

struct U : public LU
{
    explicit U(std::shared_ptr<profile_matrix> matrix)
        : LU(matrix)
    {}

    matrix_t::value_type get(size_t i, size_t j) const override;
};

std::pair<std::unique_ptr<matrix_t>, std::unique_ptr<matrix_t>> LU(profile_matrix&);

#endif // LU_H
