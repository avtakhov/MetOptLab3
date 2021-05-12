#include "test_generator.h"
#include "matrix.h"
#include "vector_matrix.h"
#include "profile.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <random>
#include <iterator>

int main()
{
    auto gen = std::mt19937(std::random_device()());
    uint TEST_CNT = 4;
    namespace fs = std::filesystem;
    for (uint i = 1; i <= TEST_CNT; ++i)
    {
        fs::path path = fs::current_path().append("Test" + std::to_string(i));
        fs::path input_file = path/"input.txt";
        fs::create_directory(path);
        {
            std::ofstream os(input_file);
            generate_one(os, gen, 20);
        }

        std::ifstream in(input_file);
        profile_matrix pm;
        in >> pm;
        profile_matrix pm_copy(pm);
        auto[l, u] = LU(pm);
        std::ofstream out(path/"output.txt");
        out << pm << std::endl;
        std::cout << *l * *u << std::endl;
        std::cout << vector_matrix(pm_copy) << std::endl << std::endl;
    }
}

/*

1 2 0 0
0 3 0 1
0 0 18 0
0 0 1 1

4 1 3
1 3 18 1
1
0 0 0 1
2 1 0
0 1 1 3


1 2 3
4 5 6
7 8 9

3 3 3
1 5 9
4 7 8
0 1 3
2 3 6
0 1 3

*/
