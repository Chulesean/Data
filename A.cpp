#include <iostream>
using namespace std;

const int MOD = 1000003;

void matrixMultiply(long long A[2][2], long long B[2][2]) {
    long long p = (A[0][0] * B[0][0] + A[0][1] * B[1][0]) % MOD;
    long long q = (A[0][0] * B[0][1] + A[0][1] * B[1][1]) % MOD;
    long long r = (A[1][0] * B[0][0] + A[1][1] * B[1][0]) % MOD;
    long long s = (A[1][0] * B[0][1] + A[1][1] * B[1][1]) % MOD;

    A[0][0] = p;
    A[0][1] = q;
    A[1][0] = r;
    A[1][1] = s;
}

void matrixPower(long long mat[2][2], long long exp) {
    if (exp == 0 || exp == 1) return;

    long long base[2][2] = {{1, 1}, {1, 0}};

    matrixPower(mat, exp / 2);
    matrixMultiply(mat, mat);

    if (exp % 2 != 0) {
        matrixMultiply(mat, base);
    }
}

int computeFibonacci(long long n) {
    if (n == 1) return 0; 
    if (n == 2) return 1;

    long long transformationMatrix[2][2] = {{1, 1}, {1, 0}};
    
    matrixPower(transformationMatrix, n - 2);

    return transformationMatrix[0][0];
}

int main() {
    long long n;
    cin >> n;
    cout << computeFibonacci(n) << "\n";
    return 0;
}