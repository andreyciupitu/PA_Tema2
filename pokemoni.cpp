#include <fstream>
#include <vector>
#include <climits>

#define INF INT_MAX / 2

using namespace std;

struct Graph
{
    int node_count;
    int pokemons;
    int ***pokemon_costs;

    Graph(int n, int p);
    ~Graph();
    void computeDistances();
};

int findShortestPath(Graph &g, int switches);

int main()
{
    int n, edge_count, pokemons, switches;
    ifstream fin("pokemoni.in");
    ofstream fout("pokemoni.out");

    fin >> n >> edge_count >> pokemons >> switches;
    Graph g(n, pokemons);

    /* Citeste muchiile din fisier */
    for (int i = 0; i < edge_count; i++)
    {
        int u, v, cost;
        fin >> u >> v;
        for (int j = 0; j < pokemons; j++)
        {
            fin >> cost;

            /* Retine costul mai mic in cazul muchiilor duplicat */
            if (g.pokemon_costs[j][u - 1][v - 1] > cost)
            {
                g.pokemon_costs[j][u - 1][v - 1] = cost;
                g.pokemon_costs[j][v - 1][u - 1] = cost;
            }
        }
    }

    /* Determina distantele intre noduri */
    g.computeDistances();

    /* Scrie in fisier rezultatul final, obtinut prin PD */
    fout << findShortestPath(g, switches) << endl;

    fout.close();
    fin.close();
    return 0;
}

Graph::Graph(int n, int p) : node_count(n), pokemons(p)
{
    /* Aloca memorie pentru matricile de distante */
    pokemon_costs = new int**[pokemons];
    for (int i = 0; i < pokemons; i++)
    {
        pokemon_costs[i] = new int*[node_count];
        for (int j = 0; j < node_count; j++)
            pokemon_costs[i][j] = new int[node_count];
    }

    /* Initializeaza matricile de distante */
    for (int i = 0; i < pokemons; i++)
        for (int j = 0; j < node_count; j++)
            for (int k = 0; k < node_count; k++)
                if (j != k)
                    pokemon_costs[i][j][k] = INF;
                else
                    pokemon_costs[i][j][k] = 0;
}

Graph::~Graph()
{
    /* Elibereaza memoria ocupata de matricile de distante */
    for (int i = 0; i < pokemons; i++)
    {
        for (int j = 0; j < node_count; j++)
            delete[] pokemon_costs[i][j];
        delete[] pokemon_costs[i];
    }
    delete[] pokemon_costs;
}

/**
 * Aplica Floyd-Warshall pentru a determina distantele intre oricare 2 noduri
 * pentru fiecare pokemon in parte.
 *
 * @method Graph::computeDistances
 */
void Graph::computeDistances()
{
    for (int p = 0; p < pokemons; p++)
        for (int k = 0; k < node_count; k++)
            for (int i = 0; i < node_count; i++)
                for (int j = 0; j < node_count; j++)
                    pokemon_costs[p][i][j] = min(pokemon_costs[p][i][j],
                        pokemon_costs[p][i][k] + pokemon_costs[p][k][j]);
}


/**
 * Functia determina drumul minim de la nodul 1 la nodul N prin graful
 * primit ca parametru folosind cel mult switches schimbari.
 *
 * @method findShortestPath
 *
 * @param  g                Graful.
 * @param  switches         Numarul maxim de schimbari.
 *
 * @return Distanta minima de la 1 la N pentru graful dat.
 */
int findShortestPath(Graph &g, int switches)
{
    /* Aloca memorie pentru PD */
    int ***total_costs = new int**[switches + 1];
    for (int i = 0; i <= switches; i++)
    {
        total_costs[i] = new int*[g.node_count];
        for (int j = 0; j < g.node_count; j++)
            total_costs[i][j] = new int[g.node_count];
    }

    /* Initializeaza matricile pentru PD */
    for (int i = 0; i <= switches; i++)
        for (int j = 0; j < g.node_count; j++)
            for (int k = 0; k < g.node_count; k++)
                if (j != k)
                    total_costs[i][j][k] = INF;
                else
                    total_costs[i][j][j] = 0;

    /* Primul pas din PD */
    for (int i = 0; i < g.node_count; i++)
        for (int j = 0; j < g.node_count; j++)
            for (int p = 0; p < g.pokemons; p++)
                total_costs[0][i][j] = min(total_costs[0][i][j],
                    g.pokemon_costs[p][i][j]);

    /* Aplica PD, crescand mereu numarul de schimburi facute,
        incercand schimburi in fiecare nod */
    for (int s = 1; s <= switches; s++)
        for (int k = 0; k < g.node_count; k++)
            for (int i = 0; i < g.node_count; i++)
                for (int j = 0; j < g.node_count; j++)
                {
                    int cost = min(total_costs[s][i][j], total_costs[s-1][i][k]
                        + total_costs[0][k][j]);
                    total_costs[s][i][j] = min(total_costs[s-1][i][j], cost);
                }
    int result = total_costs[switches][0][g.node_count - 1];

    /* Elibereaza memoria ocupata de PD */
    for (int i = 0; i <= switches; i++)
    {
        for (int j = 0; j < g.node_count; j++)
            delete[] total_costs[i][j];
        delete[] total_costs[i];
    }
    delete[] total_costs;

    return result;
}
