/**
 * ============================================================================
 * Competitive Programming Topic: Divisors & Multiples Guide
 * ============================================================================
 * Concepts Covered:
 * 1. Divisor Definition & Basic Properties
 * 2. Algorithms: O(N) Brute Force vs O(sqrt(N)) Optimized
 * 3. Count of Divisors: tau(n) via Prime Factorization
 * 4. Sum of Divisors: sigma(n) via Prime Factorization
 * 5. Product of Divisors: P(n) Formula
 * 6. Precomputing Divisors Sieve-style: O(N log N)
 * 7. CSES Problem Solution: "Counting Divisors"
 * ============================================================================
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>

using namespace std;

// Maximum limit for precomputation (e.g., CSES constraints)
const int MAXN = 1000000;
int divisor_count_sieve[MAXN + 1];
int spf[MAXN + 1]; // Smallest Prime Factor array

// ============================================================================
// 1 & 2. FINDING DIVISORS
// ============================================================================

// Brute Force: O(N)
vector<int> getDivisorsBrute(int n) {
    vector<int> divisors;
    for (int i = 1; i <= n; i++) {
        if (n % i == 0) {
            divisors.push_back(i);
        }
    }
    return divisors;
}

// Optimized: O(sqrt(N))
vector<int> getDivisorsOptimized(int n) {
    vector<int> divisors;
    for (int i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            divisors.push_back(i);
            if (i != n / i) { // Avoid inserting duplicate square root factor
                divisors.push_back(n / i);
            }
        }
    }
    return divisors;
}

// ============================================================================
// 3, 4 & 5. FORMULAS VIA PRIME FACTORIZATION
// ============================================================================

// Returns prime factors as pairs of {prime, exponent} in O(sqrt(N))
vector<pair<int, int>> getPrimeFactorization(int n) {
    vector<pair<int, int>> factors;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            int count = 0;
            while (n % i == 0) {
                count++;
                n /= i;
            }
            factors.push_back({i, count});
        }
    }
    if (n > 1) {
        factors.push_back({n, 1});
    }
    return factors;
}

// 3. Count of Divisors tau(n) = (e1 + 1) * (e2 + 1) * ...
long long countDivisors(int n) {
    auto factors = getPrimeFactorization(n);
    long long count = 1;
    for (auto& [p, e] : factors) {
        count *= (e + 1);
    }
    return count;
}
// Or if you directly have to count total divisors of n
long long countDivisors(long long n)
{
  long long ans = 1;
  for(long long i=2; i*1<=n; ++i)
    {
      if(n%i==0)
      {
        int count = 0;
        while(n%i==0)
          {
            count++;
              n /= i;
          }
        ans*= (count+1);
      }
    }
    return ans;
  
}

// 4. Sum of Divisors sigma(n) = PRODUCT( (p^(e+1) - 1) / (p - 1) )
long long sumOfDivisors(int n) {
    auto factors = getPrimeFactorization(n);
    long long total_sum = 1;
    for (auto& [p, e] : factors) {
        long long current_term = 0;
        long long p_power = 1;
        for (int i = 0; i <= e; i++) {
            current_term += p_power;
            p_power *= p;
        }
        total_sum *= current_term;
    }
    return total_sum;
}

// 5. Product of Divisors P(n) = n^(tau(n) / 2)
double productOfDivisors(int n) {
    long long tau = countDivisors(n);
    return pow(n, tau / 2.0);
}

// ============================================================================
// 6. SIEVE-STYLE PRECOMPUTATION
// ============================================================================

// O(N log N) precomputation for query processing
void precomputeDivisorCount() {
    for (int i = 1; i <= MAXN; i++) {
        for (int j = i; j <= MAXN; j += i) {
            divisor_count_sieve[j]++;
        }
    }
}

// Smallest Prime Factor (SPF) approach for quick factorization queries
void precomputeSPF() {
    for (int i = 1; i <= MAXN; i++) spf[i] = i;
    for (int i = 2; i * i <= MAXN; i++) {
        if (spf[i] == i) {
            for (int j = i * i; j <= MAXN; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }
}

// ============================================================================
// 7. CSES: COUNTING DIVISORS (SOLVER)
// ============================================================================

void solveCSESCountingDivisors() {
    // Usage: Input number of queries Q, then Q integers.
    // Precomputation ensures O(1) response per query.
    int queries[] = {16, 17, 18};
    cout << "\n--- CSES Counting Divisors Demo ---" << endl;
    for (int x : queries) {
        cout << "Number: " << x << " -> Count of Divisors: " << divisor_count_sieve[x] << "\n";
    }
}

// ============================================================================
// MAIN DEMONSTRATION
// ============================================================================

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int test_number = 360;

    cout << "==========================================" << endl;
    cout << " Divisor Concepts Demo for n = " << test_number << endl;
    cout << "==========================================" << endl;

    // 1 & 2. Divisor List
    vector<int> divs = getDivisorsOptimized(test_number);
    cout << "1. Divisors count [O(sqrt N)]: " << divs.size() << endl;

    // 3. Count Formula tau(n)
    cout << "3. Count of Divisors tau(" << test_number << "): " << countDivisors(test_number) << endl;

    // 4. Sum Formula sigma(n)
    cout << "4. Sum of Divisors sigma(" << test_number << "): " << sumOfDivisors(test_number) << endl;

    // 5. Product Formula P(n)
    cout << "5. Product of Divisors P(" << test_number << "): " << productOfDivisors(test_number) << endl;

    // 6. Precomputation
    precomputeDivisorCount();
    precomputeSPF();
    cout << "6. Precomputed tau(" << test_number << ") via Sieve: " << divisor_count_sieve[test_number] << endl;

    // 7. CSES Demo
    solveCSESCountingDivisors();

    return 0;
}
