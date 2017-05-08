#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <list>

#define WHITE 0
#define GRAY 1
#define BLACK 2
#define ALPHABET_SIZE 26

using namespace std;

struct Node
{
    int index;
    char data;
    int colour;
    vector<int> neighbours;

    Node(int i, char ch) : index(i), data(ch), colour(WHITE) {}
};

struct Graph
{
    int n;
    vector<Node> nodes;

    explicit Graph(int count) : n(count) {}
};

void explore(Graph &g, int u, list<Node> &topSort);

int main()
{
    ifstream fin("permutari.in");
    ofstream fout("permutari.out");

    /* Initializeaza graful de litere */
    Graph g(26);
    for (int i = 0; i < ALPHABET_SIZE; i++)
        g.nodes.push_back(Node(i, i + 'a'));

    /* Citeste cuvintele si adauga muchiile in graf */
    int word_count;
    fin >> word_count;

    string prev;
    fin >> prev;
    for (int i = 1; i < word_count; i++)
    {
        string temp;
        fin >> temp;

        /* Gaseste primul caracter diferit */
        unsigned int j = 0;
        while (j < temp.length() && j < prev.length() && temp[j] == prev[j])
            j++;

        /* Daca nu exista diferente, dar cuvantul este mai
            scurt ca precedentul => IMPOSIBIL */
        if (j >= temp.length() && j < prev.length())
        {
            fout << "Imposibil" << endl;
            fout.close();
            return 0;
        }

        /* Adauga restrictia impusa de diferenta gasita in graf */
        if (j < temp.length() && j < prev.length())
        {
            int aux1 = prev[j] - 'a';
            int aux2 = temp[j] - 'a';
            g.nodes[aux1].neighbours.push_back(aux2);
        }
        prev = temp;
    }

    /* Sorteaza topologic nodurile grafului */
    list<Node> topSort;
    for (auto node : g.nodes)
        if (node.colour == WHITE)
        {
            /* Daca graful contine ciclu => IMPOSIBIL */
            try
            {
                explore(g, node.index, topSort);
            }
            catch (string err)
            {
                fout << err << endl;
                fout.close();
                return 0;
            }
        }

    /* Scrie ordinea rezultata */
    for (auto node : topSort)
        fout << node.data;

    fin.close();
    fout.close();
    return 0;
}

/**
 * Realizeaza o sortare topologica a grafului, folosind o parcurgere DFS a
 * acestuia. Ordinea nodurilor este salvata in lista primita ca parametru.
 *
 * @method explore
 *
 * @param  g       Graful.
 * @param  u       Nodul ce se exploreaza.
 * @param  topSort Lista in care se salveaza nodurile in ordinea sortarii.
 */
void explore(Graph &g, int u, list<Node> &topSort)
{
    g.nodes[u].colour = GRAY;
    for (auto v : g.nodes[u].neighbours)
    {
        if (g.nodes[v].colour == WHITE)
        {
            try
            {
                explore(g, v, topSort);
            }
            catch (string err)
            {
                throw err;
            }
        }
        /* Intoarce eroare in cazul in care exista ciclu */
        if (g.nodes[v].colour == GRAY)
            throw string("Imposibil");
    }
    g.nodes[u].colour = BLACK;
    topSort.push_front(g.nodes[u]);
}
