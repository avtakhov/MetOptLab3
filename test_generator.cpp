#include "matrix.h"
#include "test_generator.h"
#include <iterator>
#include <functional>

void generate_one(std::ostream& os, std::mt19937& gen, size_t max_size)
{
    auto value_dis = std::uniform_real_distribution<matrix_t::value_type>(0, 10);

    auto size_dis = std::uniform_int_distribution<size_t>(2, max_size);

    size_t size = size_dis(gen);
    auto dis = std::uniform_int_distribution<size_t>(0, static_cast<size_t>(std::min(1ull * (size - 1) * size / 2, 100ull)));
    size_t r = dis(gen);
    size_t c = dis(gen);
    os << size << " " << r << " " << c << std::endl;
    auto out_random_values = [&](size_t size)
    {
        std::generate_n(std::ostream_iterator<matrix_t::value_type>(os, " "), size, std::bind(value_dis, gen));
        os << std::endl;
    };
    out_random_values(size);
    auto generate_profile = [&](size_t r)
    {
        out_random_values(r);
        std::vector<size_t> seps(size, 0);
        auto dis_seps = std::uniform_int_distribution<size_t>(1, size - 1);
        for (size_t k = 0; k < r; ++k)
        {
            seps[dis_seps(gen)]++;
        }
        for (size_t j = 1; j < seps.size(); ++j)
        {
            seps[j] += seps[j - 1];
        }
        std::copy(seps.begin(), seps.end(), std::ostream_iterator<matrix_t::value_type>(os, " "));
        os << std::endl;

    };
    generate_profile(r);
    generate_profile(c);
}

