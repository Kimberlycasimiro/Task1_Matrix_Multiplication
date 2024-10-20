package org.example;

import org.openjdk.jmh.annotations.*;
import java.util.Random;
import java.util.concurrent.TimeUnit;

@BenchmarkMode(Mode.AverageTime)
@OutputTimeUnit(TimeUnit.MILLISECONDS)
@State(Scope.Benchmark)
public class MatrixMultiplicationTest {

    @Param({"64", "128", "256", "512", "1024"})
    private int n;

    private double[][] A;
    private double[][] B;

    @Setup(Level.Trial)
    public void setup() {
        Random random = new Random();
        A = new double[n][n];
        B = new double[n][n];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                A[i][j] = random.nextDouble();
                B[i][j] = random.nextDouble();
            }
        }
    }

    @Warmup(iterations = 5, time = 1000, timeUnit = TimeUnit.MILLISECONDS)
    @Measurement(iterations = 10, time = 1000, timeUnit = TimeUnit.MILLISECONDS)
    @Fork(1)
    @Benchmark
    public void multiplicationBenchmark() {
        double memoryBefore = MatrixMultiplication.memoryUsage();
        long cpuBefore = MatrixMultiplication.cpuTime();
        long startTime = System.nanoTime();

        double[][] result = MatrixMultiplication.multiplyMatrices(A, B);

        double memoryAfter = MatrixMultiplication.memoryUsage();
        long cpuAfter = MatrixMultiplication.cpuTime();
        long endTime = System.nanoTime();

        double executionTime = (endTime - startTime) / 1_000_000.0;
        double cpuTimeDiff = (cpuAfter - cpuBefore) / 1_000_000_000.0;
        double wallClockTime = (endTime - startTime) / 1_000_000_000.0;
        double cpuUsagePercent = (cpuTimeDiff / wallClockTime) * 100;

        System.out.println("Matrix size: " + n + "x" + n);
        System.out.printf("Average Execution Time: %.3f ms%n", executionTime);
        System.out.printf("Average Memory Usage: %.3f MB%n", memoryAfter - memoryBefore);
        System.out.printf("Average CPU Usage: %.2f %% %n", cpuUsagePercent);
        System.out.println("------------------------------------");
    }
}
