#include "profile.h"

#include <iostream>
#include <algorithm>
#include <vector>



int main()
{
    std::vector<std::vector<double>> a = {std::vector<double>({1, 2, 3}),std::vector<double>({4, 5, 6}), std::vector<double>({7, 8, 9})};
    vector_matrix vm(a);
    profile_matrix pm(vm);
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            std::cout << pm.get(i, j) << " ";
        }
        std::cout << std::endl;
    }


    std::cout << std::endl;

    auto[l, u] = LU(std::move(pm));

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            std::cout << l->get(i, j) << " ";
        }
        std::cout << std::endl;
    }


    std::cout << std::endl;

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            std::cout << u->get(i, j) << " ";
        }
        std::cout << std::endl;
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
