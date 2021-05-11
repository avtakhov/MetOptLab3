#ifndef PROFILE_H
#define PROFILE_H

#include "matrix.h"
#include <vector>
#include <istream>
#include <ostream>
#include <functional>
#include <memory>

template <typename Profile>
struct profile_block;

template <typename T>
struct profile
{
    using value_type = T;

    profile(std::vector<T> values, std::vector<size_t> separators)
        : values(std::move(values))
        , separators(std::move(separators))
    {}

    profile() = default;

    profile_block<profile> operator[](size_t index)
    {
        return profile_block<profile>(index, *this);
    }

    profile_block<profile const> operator[](size_t index) const
    {
        return profile_block<profile const>(index, *this);
    }
private:
    std::vector<T> values;

    std::vector<size_t> separators;

    friend std::istream& operator>>(std::istream&, profile&);

    friend std::ostream& operator<<(std::ostream&, profile const&);

    template <typename Profile>
    friend struct profile_block;

};

template <typename Profile>
struct profile_block
{
    using value_type = typename Profile::value_type;

    using iterator = typename std::vector<value_type>::iterator;

    using const_iterator = typename std::vector<value_type>::const_iterator;

    profile_block(size_t index, Profile& profile_ref)
        : index(index)
        , profile_ref(profile_ref)
    {}

    iterator begin()
    {
        Profile& p = profile_ref.get();
        return std::next(p.values.begin(), index == 0 ? 0 : p.separators[index - 1]);
    }

    iterator end()
    {
        Profile& p = profile_ref.get();
        return index + 1 == p.values.size() ? p.values.end() : p[index + 1].begin();
    }

    const_iterator begin() const
    {
        return const_cast<profile<Profile>>(this)->begin();
    }

    const_iterator end() const
    {
        return const_cast<profile<Profile>>(this)->end();
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

    matrix_t::real_t get(size_t, size_t) const override;

    size_t nrows() const override;

    size_t ncolumns() const override;

    ~profile_matrix() = default;

    friend std::pair<std::unique_ptr<matrix_t>, std::unique_ptr<matrix_t>> LU(profile_matrix&&);

private:
    std::optional<std::reference_wrapper<matrix_t::real_t>> get_reference(size_t, size_t);

    std::vector<real_t> diag;

    profile<matrix_t::real_t> rows;

    profile<matrix_t::real_t> columns;

    friend std::istream& operator>>(std::istream&, profile_matrix&);

    friend std::ostream& operator<<(std::ostream&, profile_matrix const&);

};

#endif // PROFILE_H
