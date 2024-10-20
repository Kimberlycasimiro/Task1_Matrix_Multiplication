package org.example;

public class MatrixMultiplication {

    public static double[][] multiplyMatrices(double[][] A, double[][] B) {
        int n = A.length;
        double[][] result = new double[n][n];
        double[][] B_transposed = transposeMatrix(B);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < n; k++) {
                    result[i][j] += A[i][k] * B_transposed[j][k];
                }
            }
        }
        return result;
    }

    private static double[][] transposeMatrix(double[][] matrix) {
        int n = matrix.length;
        double[][] transposed = new double[n][n];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                transposed[i][j] = matrix[j][i];
            }
        }
        return transposed;
    }

    public static long memoryUsage() {
        System.gc();
        Runtime runtime = Runtime.getRuntime();
        return (runtime.totalMemory() - runtime.freeMemory()) / (1024 * 1024);
    }

    public static long cpuTime() {
        return java.lang.management.ManagementFactory.getThreadMXBean().getCurrentThreadCpuTime();
    }
}