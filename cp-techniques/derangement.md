# Derangements & Related Permutation Constructions — CP Guide (C++)

A **derangement** is a permutation `p` of `0, 1, ..., n-1` in which no index stays in its original position:

Example: `p = {1, 2, 0}` is a derangement; `p = {1, 0, 2}` is not because index `2` is a fixed point.

> In CP, problems may ask for a derangement directly or hide the idea inside assignment, rearrangement, or permutation constraints. The patterns below are related, but not all are strict mathematical derangements.

---

## 1. Complete Derangement (Cyclic Shift)

**Use when:** every index must move, and there are no extra restrictions.

**Idea:** send each index to the next one; the last wraps to the first. This works for `n >= 2`.

```cpp
vector<int> p(n);
for (int i = 0; i < n; i++)
    p[i] = (i + 1) % n;
```

Example: `0 1 2 3 4` → `1 2 3 4 0`.

**Complexity:** `O(n)` time, `O(n)` space for `p`.

**Edge case:** for `n = 1`, no derangement exists.

---
## 2. Derange Only a Subarray `[l, r]`

**Use when:** only a contiguous range should move; indices outside it must remain unchanged.

**Idea:** start with the identity mapping, then cyclically shift the selected interval.

```cpp
vector<int> p(n);
iota(p.begin(), p.end(), 0); // 0, 1, ..., n-1

for (int i = l; i <= r; i++)
    p[i] = (i == r ? l : i + 1);
```

Example: `0 1 2 3 4 5 6`, range `[2,5]` → `0 1 3 4 5 2 6`.

**Complexity:** `O(n + (r-l+1))` time including initialization; `O(n)` space.

**Edge case:** range length `1` cannot be deranged.

---
## 3. Derange a Selected Subsequence of Indices

**Use when:** selected indices are non-contiguous, and all other indices must remain mapped to themselves.

`idx` contains the selected original indices. Initialize the answer to identity, then cycle only the selected indices.

```cpp
vector<int> p(n);
iota(p.begin(), p.end(), 0);

// idx = selected indices, for example {1, 3, 5}
int k = idx.size();
for (int i = 0; i < k; i++)
    p[idx[i]] = idx[(i + 1) % k];
```

Example: `idx = {1,3,5}` → full mapping `p = {0,3,2,5,4,1,6}` (for `n = 7`).

**Complexity:** `O(n + k)` time, `O(n)` space.

**Edge cases:** `k = 0` changes nothing; `k = 1` is impossible to derange (guard it before the loop); `k >= 2` works. The selected indices must be distinct and valid.

---
## 4. Maximum Number of Moved Indices

This is a **maximum-movement construction**, not necessarily a full derangement when `n` is odd.

**Use when:** maximize positions `i` for which `p[i] != i`, without additional constraints.

**Idea:** swap adjacent indices. For odd `n`, one index must remain fixed in this simple construction.

```cpp
vector<int> p(n);
iota(p.begin(), p.end(), 0);

for (int i = 0; i + 1 < n; i += 2) {
    swap(p[i], p[i + 1]);
}
```

For even `n`, all indices move. For odd `n`, this leaves the final index fixed, achieving `n-1` moved positions (the maximum possible for odd `n).

**Complexity:** `O(n)` time, `O(n)` space.

---
## 5. Derangement with Forbidden Assignments (General Restrictions)

**Use when:** each item has allowed/forbidden destinations beyond just “not its own index.”

This is no longer a simple cyclic-shift problem in general. Model it as bipartite matching:
- Left side: items.
- Right side: destination positions.
- Edge `i -> j`: assigning item `i` to position `j` is allowed.

Use a matching algorithm (for example, Kuhn's DFS augmenting-path algorithm) to find a valid assignment. If no perfect matching exists, no valid permutation exists.

### Kuhn's algorithm (basic adjacency-list version)

```cpp
int n;
vector<vector<int>> g; // g[u] = allowed destination positions for item u
vector<int> mt;        // mt[v] = item currently matched to destination v
vector<int> vis;

bool dfs(int u) {
    for (int v : g[u]) {
        if (vis[v]) continue;
        vis[v] = 1;
        if (mt[v] == -1 || dfs(mt[v])) {
            mt[v] = u;
            return true;
        }
    }
    return false;
}

// To find a matching:
mt.assign(n, -1);
int matched = 0;
for (int u = 0; u < n; u++) {
    vis.assign(n, 0);
    if (dfs(u)) matched++;
}
// A perfect assignment exists iff matched == n.
// Recover item -> destination by reversing mt:
vector<int> p(n, -1);
for (int v = 0; v < n; v++)
    if (mt[v] != -1) p[mt[v]] = v;
```

**Complexity:** Kuhn's algorithm is `O(VE)` in the usual bound for this implementation. Use Hopcroft–Karp for larger bipartite graphs.

---
## 6. Count Derangements (DP)

**Use when:** count how many derangements exist, rather than construct one.

Let `D[n]` be the number of derangements of `n` elements. Recurrence:

\[
D[n] = (n-1)(D[n-1] + D[n-2])
\]
with `D[0] = 1`, `D[1] = 0`.

```cpp
const long long MOD = 1'000'000'007;
vector<long long> dp(n + 1);
dp[0] = 1;
if (n >= 1) dp[1] = 0;

for (int i = 2; i <= n; i++)
    dp[i] = (1LL * (i - 1) * (dp[i - 1] + dp[i - 2])) % MOD;
```

**Complexity:** `O(n)` time, `O(n)` space.

**Note:** If the problem asks for an exact count (not modulo), use a big-integer type/library; `long long` overflows quickly.

---
## 7. Lexicographically Smallest Derangement

**Use when:** construct the smallest derangement in lexicographic order.

**Idea:** greedily try the smallest unused value that is not the current index. Accept it only if the remaining positions can still be deranged. With only the “not fixed” restriction, any remainder of size at least 2 can be completed; a remainder of size 1 is valid only if its remaining value differs from its remaining position.

The following straightforward feasibility-checking code is `O(n^2)` and is useful when constraints are modest:

```cpp
vector<int> p(n, -1);
vector<int> vals(n);
iota(vals.begin(), vals.end(), 0);
vector<int> used(n, 0);

for (int i = 0; i < n; i++) {
    for (int x = 0; x < n; x++) {
        if (used[x] || x == i) continue;

        // Try x at position i. Check the final leftover singleton,
        // if only one position/value would remain.
        int lastPos = n - 1;
        int leftover = -1;
        if (i == n - 2) {
            for (int y = 0; y < n; y++)
                if (!used[y] && y != x) leftover = y;
            if (leftover == lastPos) continue;
        }

        p[i] = x;
        used[x] = 1;
        break;
    }
}
```

**Important:** this greedy is intended for the ordinary derangement constraint and modest `n`; validate the result in code if adapting it, and prefer a known contest-specific construction when constraints are large. `n = 1` has no solution.

---
## 8. Rearrange Values So Every Position Changes (Value Derangement)

This differs from index derangement. Given values (duplicates allowed), rearrange them so `b[i] != a[i]` at every position.

**Feasibility:** a full value derangement exists iff the maximum frequency of any value is at most `n/2`.

**Construction:** sort the values, then rotate the sorted sequence by the maximum frequency. This standard construction works when the frequency condition is satisfied.

```cpp
vector<int> b = a;
sort(b.begin(), b.end());

int mx = 0;
for (int i = 0; i < n; ) {
    int j = i;
    while (j < n && b[j] == b[i]) j++;
    mx = max(mx, j - i);
    i = j;
}

if (mx > n / 2) {
    // Impossible to make every position's value different.
} else {
    vector<int> c(n);
    for (int i = 0; i < n; i++)
        c[i] = b[(i + mx) % n];
    // Check c[i] != a[i] for all i if adapting this construction
    // to the original order. For a general original a, use a
    // frequency-aware matching/assignment construction instead.
}
```

**Important distinction:** the sorted-rotation construction directly avoids equal values in the *sorted order*. If the requirement is `b[i] != a[i]` in the original unsorted positions, you must build assignments against the original positions (or use matching); don't blindly use this rotation.

---
## 9. Exactly `k` Fixed Points / Exactly `k` Positions Stay

A fixed point is a position `i` where `p[i] == i`. This is the opposite of a full derangement, which has zero fixed points.

**Use when:** the task asks for exactly `k` unchanged positions and all remaining positions must move.

**Construction:** keep the first `k` positions fixed and derange the rest by cyclic shift. This requires the remaining count to be `0` or at least `2`.

```cpp
vector<int> p(n);
iota(p.begin(), p.end(), 0);

int rem = n - k;
if (rem == 1) {
    // Impossible: one remaining position cannot be deranged by itself.
} else if (rem >= 2) {
    for (int i = k; i < n; i++)
        p[i] = (i + 1 == n ? k : i + 1);
}
// Positions [0, k-1] are fixed; the rest are deranged.
```

**Complexity:** `O(n)` time, `O(n)` space.

---
## 10. Quick Recognition Checklist

| Problem wording | Likely pattern |
|---|---|
| `p[i] != i` for every index | Complete derangement / cyclic shift |
| Only `[l, r]` can change | Subarray cyclic shift |
| Only listed/non-contiguous indices can change | Subsequence index cycle |
| Maximize positions that change | Swaps/cycles; handle odd length |
| Assignments have allowed destinations | Bipartite matching |
| Count valid permutations with no fixed point | Derangement DP / inclusion-exclusion |
| Smallest valid permutation | Greedy with feasibility |
| Every new value differs from the original value | Value derangement / frequency construction or matching |
| Exactly `k` positions remain unchanged | Fix `k`, derange the remainder |


## Common Mistakes

- Confusing **indices** with **values**. `p[i] != i` is an index derangement; `b[i] != a[i]` is a value-rearrangement condition.
- Forgetting that a derangement of one element is impossible.
- Using a cyclic shift on a selected subset containing duplicate indices (the subset should be distinct).
- Assuming any random permutation is a derangement.
- Ignoring extra constraints: a cyclic shift solves only the unconstrained case.
- Treating a construction as valid without checking the exact condition required by the problem.

---

## Final Takeaway

For simple derangement construction, **cycle the selected indices**. When restrictions, value conflicts, counting, or lexicographic optimization appear, the problem may require matching, DP, greedy feasibility, or a different construction.
