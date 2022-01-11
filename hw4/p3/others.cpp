#include "ada-hw4-p3.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
using namespace std;

// feel free to use global variables
std::vector<int> contest;
unordered_set<int> concealed;
unordered_map<int, int> pairid;
vector<int> posToId;
int sz;

std::vector<int> init(int N)
{
    sz = N;
    for (int i = 0; i < N; ++i)
    {
        posToId.push_back(i);
    }

    if (sz == 1)
        return contest;

    int l = 0, r = 1;
    // contest id: [0, 1, 2, 3, 4]
    // score: [1, 5, 8, 3, 6]
    // l = 2 r = 3
    // contest: 2 3
    // concealed: 2 3
    // l = 1 r = 4
    while (r < N)
    {
        int a = posToId[l], b = posToId[r];
        //cout << l<<','<<r<<endl;
        if (!compare(a, b)) // a >= b
        {
            contest.push_back(a);
            concealed.insert(a);
            contest.push_back(b);
            concealed.insert(b);
            pairid[a] = b, pairid[b] = a;
            ++r;
            while (l >= 0 && concealed.find(posToId[l]) != concealed.end())
            {
                --l;
            }
            if (l < 0)
            {
                ++r;
                l = r - 1;
            }
        }
        else
        {
            l = r;
            ++r;
        }
    }
    return contest;
}

std::vector<int> insert(int p, int id)
{
    posToId.insert(posToId.begin() + p, id);
    int sz = posToId.size();
    //int sz = contest.size();

    int l = p - 1;
    //int found = 0;
    while (l >= 0 && concealed.find(posToId[l]) != concealed.end())
    {
        --l;
    }
    fprintf(stderr, "l: %d\n", l);
    if (l >= 0 && !compare(posToId[l], id))
    {
        contest.push_back(posToId[l]);
        concealed.insert(posToId[l]);
        contest.push_back(id);
        concealed.insert(id);
        //found = 1;
        pairid[posToId[l]] = id, pairid[id] = posToId[l];
    }
    else
    {
        //find right
        int r = p + 1;
        while (r < sz && concealed.find(posToId[r]) != concealed.end())
        {
            ++r;
        }
        fprintf(stderr, "r: %d\n", r);
        if (r < sz && !compare(id, posToId[r]))
        {
            contest.push_back(posToId[r]);
            concealed.insert(posToId[r]);
            contest.push_back(id);
            concealed.insert(id);
            //found = 1;
            pairid[posToId[r]] = id, pairid[id] = posToId[r];
        }
    }
    return contest;
}

std::vector<int> remove(int p)
{

    //posToId.erase(posToId.begin() + p);
    int sz = posToId.size();
    int id = posToId[p];

    if (concealed.find(id) == concealed.end())
    {
        posToId.erase(posToId.begin() + p);
        return contest;
    }

    int paired = pairid[id];

    concealed.erase(paired);
    contest.erase(std::remove(contest.begin(), contest.end(), paired), contest.end());
    contest.erase(std::remove(contest.begin(), contest.end(), id), contest.end());

    int cur = 0;
    for (int i = 0; i < sz; ++i)
    {
        if (posToId[i] == paired)
        {
            cur = i;
            break;
        }
    }

    int l = cur - 1;

    while (l >= 0 && concealed.find(posToId[l]) != concealed.end())
    {
        --l;
    }
    if (l >= 0 && !compare(posToId[l], paired))
    {
        contest.push_back(posToId[l]);
        concealed.insert(posToId[l]);
        contest.push_back(paired);
        concealed.insert(paired);
        pairid[posToId[l]] = paired, pairid[paired] = posToId[l];
    }
    else
    {
        //find right
        int r = cur + 1;
        while (r < sz && concealed.find(posToId[r]) != concealed.end())
        {
            ++r;
        }
        if (r < sz && !compare(paired, posToId[r]))
        {
            contest.push_back(posToId[r]);
            concealed.insert(posToId[r]);
            contest.push_back(paired);
            concealed.insert(paired);
            pairid[posToId[r]] = paired, pairid[paired] = posToId[r];
        }
    }
    posToId.erase(posToId.begin() + p);
    return contest;
}
