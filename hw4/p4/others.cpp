#include <bits/stdc++.h>
// #include <iostream>
// #include <vector>
// #include <algorithm>
// #include <unordered_set>
//reference: b08902149 徐晨祐 , b08501098 柯晨緯, b06303131 沈家睿
using namespace std;
#define ll long long
#define MX 1e18
ll solve(vector<ll> ring, ll m)
{
    srand(316);
    ll n = ring.size();
    ll k = 5 * n / m;
    //cout << k<<endl;
    ll ans = MX;
    vector<ll> rod;
    unordered_set<ll> tried;
    while (k--)
    {
        //random cut
        ll cut = rand() % n;
        if (tried.find(cut) == tried.end())
            tried.insert(cut);
        else
            continue;
        rod.clear();
        //flattern ring to rod
        rod.insert(rod.end(), ring.begin() + cut, ring.end());
        rod.insert(rod.end(), ring.begin(), ring.begin() + cut);

        vector<vector<ll> > dp(n, vector<ll>(m, MX));
        dp[0][0] = rod[0];
        dp[1][0] = rod[0] + rod[1] - abs(rod[1] - rod[0]);
        for (int i = 2; i < n; ++i)
        {
            dp[i][0] = dp[i - 1][0] + rod[i] - abs(rod[i] - rod[i - 1]) + rod[i - 1] * ((rod[i - 2] & rod[i - 1]) ^ (rod[i - 1] | rod[i]) ^ (rod[i - 2] + rod[i]));
        }

        for (int i = 1; i < n; ++i)
        {
            for (int j = 1; j < m; ++j)
            {
                if (i < j)
                {
                    continue;
                }
                //case 1
                ll add1 = 0;
                if (i == 1)
                {
                    add1 = rod[i] - abs(rod[i] - rod[i - 1]);
                }
                else
                {
                    add1 = rod[i] - abs(rod[i] - rod[i - 1]) + rod[i - 1] * ((rod[i - 2] & rod[i - 1]) ^ (rod[i - 1] | rod[i]) ^ (rod[i - 2] + rod[i]));
                }
                //case2
                ll add2 = rod[i];
                //case 3
                ll add3 = rod[i] + rod[i - 1] - abs(rod[i] - rod[i - 1]);

                if (i >= 2)
                    dp[i][j] = min({dp[i - 1][j] + add1, dp[i - 1][j - 1] + add2, dp[i - 2][j - 1] + add3});
                else
                    dp[i][j] = min(dp[i - 1][j] + add1, dp[i - 1][j - 1] + add2);
            }
        }

        ans = min(ans, dp[n - 1][m - 1]);
    }

    return ans;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    ll n, m;
    cin >> n >> m;
    vector<ll> ring;
    while (n--)
    {
        ll input;
        cin >> input;
        ring.push_back(input);
    }
    cout << solve(ring, m);
    return 0;
}
