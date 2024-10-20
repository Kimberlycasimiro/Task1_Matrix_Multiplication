#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <sys/resource.h>
#include <chrono>
#include <fstream> 
#include "matrix_multiplier.cpp"

std::vector<std::vector<double>> generate_random_matrix(int size) {
    std::vector<std::vector<double>> matrix(size, std::vector<double>(size));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = static_cast<double>(rand()) / RAND_MAX;
        }
    }
    return matrix;
}

long get_memory_usage() {
    std::ifstream statm("/proc/self/statm");
    long size;
    statm >> size;
    return size * sysconf(_SC_PAGESIZE) / 1024;
}

double get_cpu_time() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return static_cast<double>(usage.ru_utime.tv_sec) + static_cast<double>(usage.ru_utime.tv_usec) / 1e6;
}

double get_cpu_usage_percent(double cpu_before, double cpu_after, double wall_clock_time) {
    return ((cpu_after - cpu_before) / wall_clock_time) * 100;
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    int matrix_sizes[] = {64, 128, 256, 512, 1024};
    int num_runs = 10;

    for (int size : matrix_sizes) {
        double avg_execution_time = 0.0;
        double avg_cpu_usage_percent = 0.0;
        long avg_memory_usage = 0;

        for (int run = 0; run < num_runs; run++) {
            std::vector<std::vector<double>> A = generate_random_matrix(size);
            std::vector<std::vector<double>> B = generate_random_matrix(size);

            long memory_before = get_memory_usage();
            double cpu_before = get_cpu_time();
            auto wall_clock_start = std::chrono::high_resolution_clock::now();

            std::vector<std::vector<double>> result = multiplyMatrices(A, B);
            auto wall_clock_end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> execution_time = wall_clock_end - wall_clock_start;
            avg_execution_time += execution_time.count();

            long memory_after = get_memory_usage();
            double cpu_after = get_cpu_time();

            double cpu_usage_percent = get_cpu_usage_percent(cpu_before, cpu_after, execution_time.count() / 1000.0);
            avg_memory_usage += (memory_after - memory_before);
            avg_cpu_usage_percent += cpu_usage_percent;
        }

        avg_execution_time /= num_runs;
        avg_cpu_usage_percent /= num_runs;
        avg_memory_usage /= num_runs;

        std::cout << "Matrix size: " << size << "x" << size << std::endl;
        std::cout << "Average Execution Time: " << avg_execution_time << " ms" << std::endl;
        std::cout << "Average Memory Usage: " << avg_memory_usage / 1024.0 << " MB" << std::endl;
        std::cout << "Average CPU Usage: " << avg_cpu_usage_percent << " %" << std::endl;
        std::cout << "------------------------------------" << std::endl;
    }

    return 0;
}

