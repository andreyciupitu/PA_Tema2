#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

using namespace std;

struct Node
{
    vector< pair<int, int> > neighbours;
};

struct Graph
{
    int n;
    vector<Node> neighbours;
};

int pokemon_costs[20][100][100];
int total_costs[21][100][100];

int main()
{
    int edge_count, pokemons, switches;
    ifstream fin("pokemoni.in");

    Graph g;
    fin >> g.n >> edge_count >> pokemons >> switches;

    for (int i = 0; i < pokemons; i++)
        for (int j = 0; j < g.n; j++)
            for (int k = 0; k < g.n; k++)
                if (j != k)
                    pokemon_costs[i][j][k] = 100001;

    for (int i = 0; i <= switches; i++)
        for (int j = 0; j < g.n; j++)
            for (int k = 0; k < g.n; k++)
                if (j != k)
                    total_costs[i][j][k] = 100001;
    for (int i = 0; i < edge_count; i++)
    {
        int u, v, cost;
        fin >> u >> v;
        for (int j = 0; j < pokemons; j++)
        {
            fin >> cost;
            if (pokemon_costs[j][u-1][v-1] > cost)
            {
                pokemon_costs[j][u-1][v-1] = cost;
                pokemon_costs[j][v-1][u-1] = cost;
            }
        }
    }

    for (int p = 0; p < pokemons; p++)
        for (int k = 0; k < g.n; k++)
            for (int i = 0; i < g.n; i++)
                for (int j = 0; j < g.n; j++)
                    pokemon_costs[p][i][j] = min(pokemon_costs[p][i][j], pokemon_costs[p][i][k] + pokemon_costs[p][k][j]);

    for (int i = 0; i < g.n; i++)
        for (int j = 0; j < g.n; j++)
            for (int p = 0; p < pokemons; p++)
                total_costs[0][i][j] = min(total_costs[0][i][j], pokemon_costs[p][i][j]);
    for (int s = 1; s <= switches; s++)
        for (int k = 0; k < g.n; k++)
            for (int i = 0; i < g.n; i++)
                for (int j = 0; j < g.n; j++)
                {
                    total_costs[s][i][j] = min(total_costs[s-1][i][j], min(total_costs[s][i][j], total_costs[s-1][i][k] + total_costs[0][k][j]));
                }
    fin.close();
    ofstream fout("pokemoni.out");
    fout << total_costs[switches][0][g.n - 1]<< endl;
    fout.close();
    return 0;
}
