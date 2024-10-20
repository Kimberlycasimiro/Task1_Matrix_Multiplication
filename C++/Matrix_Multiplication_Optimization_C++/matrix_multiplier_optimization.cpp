#include <vector>
#include <algorithm>

const int UNROLL_FACTOR = 4;

std::vector<std::vector<double>> multiplyMatrices(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B
) {
    const int n = A.size();
    std::vector<std::vector<double>> result(n, std::vector<double>(n, 0.0));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            double sum0 = 0.0, sum1 = 0.0, sum2 = 0.0, sum3 = 0.0;
            int k = 0;

            for (; k <= n - UNROLL_FACTOR; k += UNROLL_FACTOR) {
                sum0 += A[i][k] * B[k][j];
                sum1 += A[i][k + 1] * B[k + 1][j];
                sum2 += A[i][k + 2] * B[k + 2][j];
                sum3 += A[i][k + 3] * B[k + 3][j];
            }

            for (; k < n; ++k) {
                sum0 += A[i][k] * B[k][j];
            }

            result[i][j] = sum0 + sum1 + sum2 + sum3;
        }
    }

    return result;
}

