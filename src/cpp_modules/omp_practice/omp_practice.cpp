#include <iostream>
#include <omp.h>



void inner_funct() {

    omp_set_num_threads(8);

    #pragma omp parallel 
    {
        auto inner_int = omp_get_num_threads();
        std::cout << "inner thread num: " << inner_int << std::endl;
    }
}


void outer_funct()
{
    omp_set_num_threads(4);

#pragma omp parallel
    {
        auto my_int = omp_get_thread_num();
        std::cout << "outer thread num: " << my_int << std::endl;
        if (my_int == 0) {
            inner_funct();
        }
    }
}


int main()
{
    outer_funct();

    return 0;
}