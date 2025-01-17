#include <iostream>
#include <vector>
using namespace std;

const int MOD = 1000003;

// Function to multiply two matrices
vector<vector<int>> multiply(const vector<vector<int>> &a, const vector<vector<int>> &b) {
    int size = a.size();
    vector<vector<int>> result(size, vector<int>(size, 0));
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int k = 0; k < size; ++k) {
                result[i][j] = (result[i][j] + 1LL * a[i][k] * b[k][j]) % MOD;
            }
        }
    }
    return result;
}

// Function to perform matrix exponentiation
vector<vector<int>> matrixExponentiation(vector<vector<int>> base, unsigned long long exp) {
    int size = base.size();
    vector<vector<int>> result(size, vector<int>(size, 0));

    // Initialize result as the identity matrix
    for (int i = 0; i < size; ++i) {
        result[i][i] = 1;
    }

    // Exponentiation by squaring
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = multiply(result, base);
        }
        base = multiply(base, base);
        exp /= 2;
    }

    return result;
}

int main() {
    unsigned long long n;
    cin >> n;

    if (n == 1) {
        cout << 1 << endl;
        return 0;
    }

    // Transition matrix
    vector<vector<int>> M = {
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0}
    };

    // Initial state vector
    vector<int> dp = {1, 0, 0, 0, 0};

    // Compute M^(n-1)
    vector<vector<int>> Mn_minus_1 = matrixExponentiation(M, n - 1);

    // Compute dp[n] using Mn_minus_1
    int result = 0;
    for (int i = 0; i < 5; ++i) {
        result = (result + 1LL * Mn_minus_1[0][i] * dp[i]) % MOD;
    }

    cout << result << endl;
    return 0;
}