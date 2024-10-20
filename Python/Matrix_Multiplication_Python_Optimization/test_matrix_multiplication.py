import pytest
import numpy as np
import psutil
import time
from matrix_multiplication import multiply_matrices
from memory_profiler import memory_usage


@pytest.mark.parametrize("matrix_size", [128, 256, 512, 1024, 2048])
def test_matrix_multiplication(benchmark, matrix_size):
    a = np.random.rand(matrix_size, matrix_size)
    b = np.random.rand(matrix_size, matrix_size)

    def multiplication():
        return multiply_matrices(a, b)

    for _ in range(5):
        multiplication()

    memory_usage_before = memory_usage()[0]
    cpu_usage_before = psutil.cpu_percent(interval=None)
    start_time = time.time()

    benchmark.pedantic(multiplication, rounds=10, iterations=1)

    end_time = time.time()
    cpu_usage_after = psutil.cpu_percent(interval=None)
    memory_usage_after = memory_usage()[0]

    memory_used = max(0, int(memory_usage_after - memory_usage_before))
    execution_time = (end_time - start_time) * 1000
    cpu_usage = max(0, int(cpu_usage_after - cpu_usage_before))

    print(f"Matrix size: {matrix_size}x{matrix_size}")
    print(f"Average Execution Time: {execution_time} ms")
    print(f"Average Memory Usage: {memory_used} MB")
    print(f"Average CPU Usage: {cpu_usage} %")
    print("------------------------------------")


if __name__ == "__main__":
    pytest.main()
