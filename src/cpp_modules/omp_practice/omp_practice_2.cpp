#include <iostream>
#include <omp.h>


void inner_funct() {
    int num_inner_threads = std::rand() % 8;
    #pragma omp parallel num_threads(5)
    {
        #pragma omp critical
        {
            std::cout 
                << "Hello from inner thread "
                << omp_get_ancestor_thread_num(1)
                << "-"
                << omp_get_thread_num()
                << std::endl;
        }
    }
}


void outer_funct() {

    omp_set_num_threads(4);

      
    int num_outer_loops = 2;

    #pragma omp parallel for
    for (auto idx = 0; idx < num_outer_loops; idx++)
    {
        #pragma omp critical
        {
            std::cout
                << "Hello from outer thread "
                << omp_get_thread_num()
                << std::endl;
        }

        if (omp_get_thread_num() == 0) {
            inner_funct();
        }
    }
}


int main()
{
    omp_set_nested(1);
    outer_funct();
    return 0;

}
