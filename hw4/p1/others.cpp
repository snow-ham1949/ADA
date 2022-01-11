#include "ypglpk.hpp"
#include <bits/stdc++.h>


using namespace std;
//Reference: many thx to b06303131 沈家睿
void dfs(bitset<80> r, bitset<80> p, bitset<80> x, vector<bitset<80> > &mxC, vector<vector<int> > &adj)
{
    //if P and X are both empty:
    bitset<80> porx = p | x;
    if (!porx.any())
    {
        //report R as a maximal clique
        mxC.push_back(r);
    }
    else
    {
        //choose a pivot vertex u in P ⋃ X
        //bitset<80> pivot = 0;
        int mx = 0, u = 0;
        while (porx.any())
        {
            int a = porx._Find_first();
            porx.set(a, 0);
            int sz = adj[a].size();
            if (sz > mx)
            {
                u = a;
                mx = sz;
            }
        }

        //pivot.set(u, 1);

        //for each vertex v in P \ N(u):
        bitset<80> nu = 0; //N(u)
        bitset<80> temp = 0;
        temp.set(0);
        for (int i = 0; i < int(adj[u].size()); i++)
            nu |= (temp << adj[u][i]);

        bitset<80> pdelNu(p); // P\ N(u)
        nu &= p;
        pdelNu ^= nu;
        while (pdelNu.any())
        {
            bitset<80> v = 0; //{v}
            int vidx = pdelNu._Find_first();
            v.set(vidx);
            //BronKerbosch1(R ⋃ {v}, P ⋂ N(v), X ⋂ N(v))
            bitset<80> nbofv = 0; //N(v)
            bitset<80> temp2 = 0;
            temp2.set(0);
            for (int i = 0; i < int(adj[vidx].size()); i++)
                nbofv |= (temp2 << adj[vidx][i]);
            dfs(r | v, p & nbofv, x & nbofv, mxC, adj);
            p.set(vidx, 0);
            pdelNu.set(vidx, 0);
            x |= v;
        }
    }
}

void solve(vector<vector<int> > &adj)
{
    int n = adj.size();
    bitset<80> r = 0, p = 0, x = 0;
    for (auto i = 0; i < n; ++i)
    {
        p.set(i);
    }
    vector<bitset<80> > mxC;
    dfs(r, p, x, mxC, adj);
    int sz = mxC.size();
    vector<vector<double> > A(2 * sz + n, vector<double>(sz, 0));
    vector<double> B(2 * sz + n, 0);
    vector<double> C(sz, -1);
    vector<bool> isint(sz, true);
    for (int i = 0; i < sz; ++i)
    {
        A[i][i] = 1;
        A[sz + i][i] = -1;
        B[i] = 1;
    }

    for (int i = 2 * sz; i < 2 * sz + n; ++i)
    {
        B[i] = -1;
    }

    vector<vector<int> > clique(sz, vector<int>());
    for (int i = 0; i < sz; ++i)
    {
        while (mxC[i].any())
        {
            int a = mxC[i]._Find_first();
            clique[i].push_back(a);
            mxC[i].set(a, 0);
            A[2 * sz + a][i] = -1;
        }
    }

    vector<double> y = ypglpk::mixed_integer_linear_programming(A, B, C, isint).second;
    vector<int> visited(n, 0);
    vector<vector<int> > output;
    for (int i = 0; i < y.size(); ++i)
    {
        if (!y[i])
            continue;
        vector<int> temp;
        for (int j = 0; j < (int)clique[i].size(); ++j)
        {
            if (!visited[clique[i][j]])
            {
                temp.push_back(clique[i][j]);
                visited[clique[i][j]] = 1;
            }
        }
        if (!temp.empty())
            output.push_back(temp);
    }

    cout << output.size() << '\n';
    for (auto &it : output)
    {
        cout << it.size() << ' ';
        for (auto &it2 : it)
        {
            cout << it2 << ' ';
        }
        cout << '\n';
    }
    // cout << "mxC size: " <<mxC.size()<<'\n';
    // for (auto &it:mxC){
    //     cout << it.to_string()<<'\n';
    // }
}

int main()
{
    int t, n, m;
    cin >> t;
    while (t--)
    {
        cin >> n >> m;
        vector<vector<int> > adj(n, vector<int>());
        while (m--)
        {
            int a, b;
            cin >> a >> b;
            adj[a].push_back(b), adj[b].push_back(a);
        }
        solve(adj);
    }
    return 0;
}
