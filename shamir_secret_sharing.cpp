#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <utility>

using namespace std;

// Function to evaluate the polynomial at a given x
int evaluatePolynomial(const vector<int>& coefficients, int x) {
    int result = 0;
    int power = 1; // x^0
    for (int coeff : coefficients) {
        result += coeff * power;
        power *= x; // Increase the power of x
    }
    return result;
}

// Function to generate shares
vector<pair<int, int>> generateShares(int secret, int n, int k) {
    // Generate random coefficients for the polynomial
    vector<int> coefficients(k - 1);
    for (int i = 0; i < k - 1; ++i) {
        coefficients[i] = rand() % 100; // Random coefficients
    }
    coefficients.insert(coefficients.begin(), secret); // Insert secret as the constant term

    vector<pair<int, int>> shares;
    for (int i = 1; i <= n; ++i) {
        int shareValue = evaluatePolynomial(coefficients, i);
        shares.push_back(make_pair(i, shareValue)); // (x, f(x))
    }
    return shares;
}

// Function to reconstruct the secret using Lagrange interpolation
int reconstructSecret(const vector<pair<int, int>>& shares, int k) {
    int secret = 0;
    for (int i = 0; i < k; ++i) {
        int x_i = shares[i].first;
        int y_i = shares[i].second;
        int L_i = 1; // Lagrange basis polynomial

        for (int j = 0; j < k; ++j) {
            if (j != i) {
                int x_j = shares[j].first;
                L_i *= (0 - x_j) / (x_i - x_j); // Lagrange basis polynomial
            }
        }
        secret += y_i * L_i; // Add to the secret
    }
    return secret;
}

int main() {
    srand(time(0)); // Seed for random number generation

    int secret = 123; // The secret to be shared
    int n = 5; // Total number of shares
    int k = 3; // Minimum number of shares required to reconstruct the secret

    // Generate shares
    vector<pair<int, int>> shares = generateShares(secret, n, k);

    // Display the shares
    cout << "Generated Shares:" << endl;
    for (const auto& share : shares) {
        cout << "Share " << share.first << ": " << share.second << endl;
    }

    // Reconstruct the secret using the first k shares
    int reconstructedSecret = reconstructSecret(vector<pair<int, int>>(shares.begin(), shares.begin() + k), k);
    cout << "Reconstructed Secret: " << reconstructedSecret << endl;

    return 0;
}