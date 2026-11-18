# Derangement of a Subsequence (Index Mapping)

## Idea

Sometimes we need to derange only a selected subsequence of indices while keeping all other indices unchanged.

Instead of randomly rearranging elements, use a **cyclic shift** of the selected indices.

If the selected indices are:

`idx = {1, 3, 5}`

Cyclically shift them:

`1 → 3, 3 → 5, 5 → 1`

This guarantees:
- No selected index maps to itself.
- Every selected index is used exactly once.
- Unselected indices remain unchanged.
- The mapping is a permutation.

## C++ Code

```cpp
int n = a.size();
vector<int> ans(n);

iota(ans.begin(), ans.end(), 0);

// idx contains the indices of the selected subsequence
for(int i = 0; i < idx.size(); i++) {
    ans[idx[i]] = idx[(i + 1) % idx.size()];
}
```

## Example

```cpp
a = {4, 5, 20, 11, 23, 30, 40};
idx = {1, 3, 5};
```

### Initial mapping

`ans = {0, 1, 2, 3, 4, 5, 6}`

### After cyclic shift

`ans = {0, 3, 2, 5, 4, 1, 6}`

| Index | Mapped to |
|---|---|
| 0 | 0 |
| 1 | 3 |
| 2 | 2 |
| 3 | 5 |
| 4 | 4 |
| 5 | 1 |
| 6 | 6 |

Only indices `1, 3, 5` are deranged.

## Why `(i + 1) % k`?

Let `k = idx.size()`.

- `i + 1` moves to the next selected index.
- `% k` wraps the last index back to the first.

Thus, the selected indices form a cycle, ensuring no index remains in its original position, provided `k >= 2`.

## Complexity

- Time: `O(n + k)`
- Space: `O(n)`

Here, `n` is the total number of indices and `k` is the number of selected indices.

## When to use

Use this technique when:
- Only a selected subsequence needs derangement.
- The rest of the permutation must remain unchanged.
- You need a simple, guaranteed permutation without randomization.

**Edge case:** If `k = 1`, a derangement is impossible. If `k = 0`, nothing needs to be changed.
