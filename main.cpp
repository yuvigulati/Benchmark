#include <iostream>
#include <vector>
#include <thread>
#include <benchmark/benchmark.h>
using namespace std;

void parallelMul(int row, int col, vector<vector<int>>& matrixC, const vector<vector<int>>& matrixA, const vector<vector<int>>& matrixB) {
    int sum = 0;
    int n = matrixA[0].size();

    for (int i = 0; i < n; i++) {
        sum += matrixA[row][i] * matrixB[i][col];
    }
    matrixC[row][col] = sum;
}

static void BM_ParallelMatrixMultiply(benchmark::State& state) {
    vector<vector<int>> matrixA = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    vector<vector<int>> matrixB = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int row = matrixA.size();
    int col = matrixB[0].size();
    vector<vector<int>> matrixC(row, vector<int>(col, 0));

    for (auto _ : state) {
        vector<thread> threads;

        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                threads.push_back(thread(parallelMul, i, j, ref(matrixC), cref(matrixA), cref(matrixB)));
            }
        }

        for (auto& x : threads) {
            x.join();
        }

        for (int i = 0; i < row; i++) {
            fill(matrixC[i].begin(), matrixC[i].end(), 0);
        }
    }
}

BENCHMARK(BM_ParallelMatrixMultiply);
BENCHMARK_MAIN();


