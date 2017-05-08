#include <fstream>
#include <vector>
#include <queue>
#include <climits>

#define INF INT_MAX / 2

using namespace std;

struct Graph
{
    int n;
    vector< vector<int> > neighbours;

    explicit Graph(int count) : n(count)
    {
        neighbours = vector< vector<int> >(n, vector<int>());
    }
};

vector< pair <long long, int> > countPaths(const Graph &g, int start);

int main()
{
    ifstream fin("patrula.in");
    ofstream fout("patrula.out");

    int edge_count;
    int node_count;
    fin >> node_count >> edge_count;
    Graph g(node_count);

    /* Citeste muchiile */
    for (int i = 0; i < edge_count; i++)
    {
        int u, v;
        fin >> u >> v;
        g.neighbours[u - 1].push_back(v - 1);
        g.neighbours[v - 1].push_back(u - 1);
    }

    /* Caile minime de la N la 1 */
    vector< pair<long long, int> > paths = countPaths(g, g.n - 1);

    /* Caile minime de la 1 la N */
    vector< pair<long long, int> > inv_paths = countPaths(g, 0);

    /* Calculeaza media fiecarui nod aflat pe o cale minima */
    double maxScore = 0;
    for (int i = 1; i < g.n - 1; i++)
        if (paths[0].second == paths[i].second + inv_paths[i].second)
        {
            double score = (2.0 * paths[i].first * inv_paths[i].first) /
                (double)paths[0].first;
            maxScore = max(maxScore, score);
        }

    /* Scrie rezultatele in fisier */
    fout << paths[0].first << endl;
    fout.setf(iostream::fixed, iostream::floatfield);
    fout.precision(3);
    fout << max(maxScore, 1.0) << endl;

    fin.close();
    fout.close();
}

/**
 * Parcurgere BFS a grafului pornind din nodul specificat, retinandu-se
 * distantele pana la fiecare nod, precum si numarul de cai de lugime minima.
 *
 * @method countPaths
 *
 * @param g     Graful.
 * @param start Nodul de start pentru BFS.
 *
 * @return  Un vector de perechi ce contine distantele si numarul de cai de
 *          lugime minima de la nodul de start pana la fiecare nod.
 */
vector<pair<long long, int> > countPaths(const Graph &g, int start)
{
    vector< pair<long long, int> > paths(g.n, pair<long long, int>(0, INF));
    vector<bool> visited(g.n, false);
    queue<int> q;

    /* Start BFS */
    paths[start].second = 0;
    q.push(start);
    visited[start] = true;
    paths[start].first = 1;

    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (auto v : g.neighbours[u])
        {
            /* Daca nodul nu a mai fost gasit */
            if (!visited[v])
            {
                paths[v].second = paths[u].second + 1;
                paths[v].first = paths[u].first;
                visited[v] = true;
                q.push(v);
            }
            else
            {
                /* Daca mai pot ajunge din alt nod, cu aceeasi distanta */
                if (paths[v].second == paths[u].second + 1)
                    paths[v].first += paths[u].first;
            }
        }
    }

    return paths;
}
