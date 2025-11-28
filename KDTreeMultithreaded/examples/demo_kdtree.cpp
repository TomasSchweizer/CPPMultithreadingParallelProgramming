#include <iostream>
#include <omp.h>

int main() {

    const int N = 8;
    int results[N] = {0};

    std::cout << "Starting parallel loop with " << omp_get_max_threads() << " threads." << std::endl;

    #pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        int thread_id = omp_get_thread_num();
        results[i] = thread_id;

    }
    for (int i = 0; i < N; ++i) {
        std::cout << "results[" << i << "] = " << results[i] << std::endl;
    }
    std::cout << "Final results: \n";
    for (int i = 0; i < N; ++i) {
        std::cout << results[i] << " ";
    }
    std::cout << std::endl;
    return 0;
}