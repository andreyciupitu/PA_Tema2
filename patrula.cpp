#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

struct Graph
{
    int n;
    vector< vector<int> > neighbours;
};

vector<long long> bfs(const Graph &g, int start, vector<int> &distance);
int findPaths(Graph &g, const vector<int> d, int begin, int end);

int main()
{
    ifstream fin("0-patrula.in");

    Graph g;
    int edge_count;
    fin >> g.n >> edge_count;
    g.neighbours = vector< vector<int> >(g.n, vector<int>());
    for (int i = 0; i < edge_count; i++)
    {
        int u, v;
        fin >> u >> v;
        g.neighbours[u - 1].push_back(v - 1);
        g.neighbours[v - 1].push_back(u - 1);
    }
    vector<int> distance(g.n, 9999999);
    vector<int> distance2(g.n, 9999999);
    vector<long long> paths = bfs(g, g.n - 1, distance);
    vector<long long> paths2 = bfs(g, 0, distance2);

    for (auto i : distance)
        cout << i << " ";
    cout << endl;

    double maxScore = 0;
    for (int i = 1; i < g.n - 1; i++)
        if (distance[0] == distance[i] + distance2[i])
            maxScore = max(maxScore, (2.0 * paths[i] * paths2[i]) / (double)paths[0]);
    ofstream fout("patrula.out");
    fout << paths[0] << endl;

    /*int i = 0;
    while (i != g.n - 1)
    {

    }*/

    fout.setf(iostream::fixed, iostream::floatfield);
    fout.precision(3);
    fout << max(maxScore, 1.0) << endl;

    fin.close();
    fout.close();
}

vector<long long> bfs(const Graph &g, int start, vector<int> &distance)
{
    vector<long long> paths(g.n, 0);
    vector<bool> visited(g.n, false);

    queue<int> q;
    distance[start] = 0;
    q.push(start);
    visited[start] = true;
    paths[start] = 1;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (auto v : g.neighbours[u])
        {
            if (!visited[v])
            {
                distance[v] = distance[u] + 1;
                paths[v] = paths[u];
                visited[v] = true;
                q.push(v);
            }
            else
            {
                if (distance[v] == distance[u] + 1)
                    paths[v] += paths[u];
            }
        }
    }

    return paths;
}

int findPaths(Graph &g, const vector<int> d, int begin, int end)
{
    if (begin == end)
        return 1;
    int paths = 0;
    for (auto v : g.neighbours[begin])
        if (d[v] + 1 == d[begin])
            paths += findPaths(g, d, v, end);
    return paths;
}
