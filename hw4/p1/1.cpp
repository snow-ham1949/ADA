#include "ypglpk.hpp"
#include <bits/stdc++.h>

using namespace std;

#define AC                       \
    ios::sync_with_stdio(false); \
    cin.tie(0);                  \
    cout.tie(0);
#define pb(a) push_back(a)
#define pf(a) push_front(a)
#define pob pop_back()
#define pof pop_front()
#define mp(a, b) make_pair(a, b)
#define F first
#define S second

typedef long long ll;
typedef long double ld;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;

const int maxn = 80;
const int mod = 1e9 + 7;

void solve(vector<vector<int> > &edge, int n)
{
    bitset<80> r, p, x;
}

int main()
{
    AC;
    int t;
    cin >> t;
    while (t--)
    {
        int n, m;
        cin >> n >> m;
        vector<vector<int> > edge(n, vector<int>());
        for (int i = 0; i < m; i++)
        {
            int u, v;
            cin >> u >> v;
            edge[u].pb(v), edge[v].pb(u);
        }

        solve(edge, n);
    }

    return 0;
}