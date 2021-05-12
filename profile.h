#ifndef PROFILE_H
#define PROFILE_H

#include "matrix.h"
#include <vector>
#include <istream>
#include <ostream>
#include <functional>
#include <memory>

struct profile_matrix;

template <typename Profile>
struct profile_block;

struct profile
{
    using value_type = matrix_t::value_type;

    profile(std::vector<value_type> values, std::vector<size_t> separators)
        : values(std::move(values))
        , separators(std::move(separators))
    {}

    profile() = default;

    profile_block<profile> operator[](size_t index);

    profile_block<profile const> operator[](size_t index) const;

private:
    std::vector<value_type> values;

    std::vector<size_t> separators;

    friend std::istream& operator>>(std::istream&, profile&);

    friend std::ostream& operator<<(std::ostream&, profile const&);

    friend std::istream& operator>>(std::istream&, profile_matrix&);

    friend std::ostream& operator<<(std::ostream&, profile_matrix const&);

    template <typename Profile>
    friend struct profile_block;
};

template <typename Profile>
struct profile_block
{
    using value_type = typename Profile::value_type;

    profile_block(size_t index, Profile& profile_ref)
        : index(index)
        , profile_ref(profile_ref)
    {}

    auto begin()
    {
        Profile& p = profile_ref.get();
        return std::next(p.values.begin(), index == 0 ? 0 : p.separators[index - 1]);
    }

    auto end()
    {
        Profile& p = profile_ref.get();
        return index + 1 == p.separators.size() ? p.values.end() : p[index + 1].begin();
    }

    auto begin() const
    {
        return const_iterator(const_cast<profile_block*>(this)->begin());
    }

    auto end() const
    {
        return const_iterator(const_cast<profile_block*>(this)->end());
    }

private:
    size_t index;

    std::reference_wrapper<Profile> profile_ref;
};


struct profile_matrix : public matrix_t
{

    profile_matrix();

    explicit profile_matrix(matrix_t const&);

    size_t size() const;

    value_type get(size_t, size_t) const override;

    size_t nrows() const override;

    size_t ncolumns() const override;

    ~profile_matrix() = default;

    friend std::pair<std::unique_ptr<matrix_t>, std::unique_ptr<matrix_t>> LU(profile_matrix&);

private:
    std::optional<std::reference_wrapper<matrix_t::value_type>> get_reference(size_t, size_t);

    std::vector<value_type> diag;

    profile rows;

    profile columns;

    friend std::istream& operator>>(std::istream&, profile_matrix&);

    friend std::ostream& operator<<(std::ostream&, profile_matrix const&);

};

#endif // PROFILE_H
