#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <list>

using namespace std;

struct Graph
{
    vector< vector<int> > neighbours;
    int n;
};

bool selected[26];
bool finished[26];

void explore(Graph &g, int u, list<int> &topSort);

int main()
{
    ifstream fin("permutari.in");
    int word_count;
    fin >> word_count;
    Graph g;
    g.n = 26;
    g.neighbours = vector< vector<int> >(26, vector<int>());
    string last_word;
    fin >> last_word;
    ofstream fout("permutari.out");
    for (int i = 1; i < word_count; i++)
    {
        string temp;
        fin >> temp;
        int j = 0;
        while (j < temp.length() && j < last_word.length() && temp[j] == last_word[j])
            j++;
        if (j >= temp.length() && j < last_word.length())
        {
            fout << "Imposibil" << endl;
            fout.close();
            return 0;
        }
        if (j < temp.length() && j < last_word.length())
            g.neighbours[last_word[j] - 'a'].push_back(temp[j] - 'a');
        last_word = temp;
    }
    list<int> topSort;
    for (int i = 0; i < g.n; i++)
    {
        if (!finished[i])
            try
            {
                explore(g, i, topSort);
            }
            catch (string err)
            {
                fout << err << endl;
                fout.close();
                return 0;
            }
    }
    for (int c : topSort)
    {
        char aux = c + 'a';
        fout << aux;
    }
    fin.close();
    fout.close();
    return 0;
}

void explore(Graph &g, int u, list<int> &topSort)
{
    selected[u] = true;
    for (auto v : g.neighbours[u])
    {
        if (!selected[v] && !finished[v])
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
        if (selected[v])
            throw string("Imposibil");
    }
    finished[u] = true;
    selected[u] = false;
    topSort.push_front(u);
}
