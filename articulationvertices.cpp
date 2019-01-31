#include <cstdlib>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <numeric>
#include <limits>
#include <iostream>

using namespace std;

struct Vertex{
    Vertex(const int val) : val_(val), adj_() {}
    int val_;
    unordered_set<Vertex*> adj_;
};

unordered_set<Vertex*> makeGraph()
{
    Vertex* a = new Vertex(1); Vertex* b = new Vertex(2); Vertex* c = new Vertex(3);
    Vertex* d = new Vertex(4); Vertex* e = new Vertex(5); Vertex* f = new Vertex(6);
    Vertex* g = new Vertex(7); Vertex* h = new Vertex(8);

    a->adj_ = { b, c }; b->adj_ = { a, c }; c->adj_ = { a, b };
    d->adj_ = { c, e }; e->adj_ = { g, f }; f->adj_ = { e, g, h };
    g->adj_ = { e, f }; h->adj_ = { f };

    return { a, b, c, d, e, f };
}

void processVertex(
    Vertex* u,
    unordered_map<Vertex*, Vertex*>& parentMap,
    unordered_map<Vertex*, int>& entryMap,
    unordered_map<Vertex*, int>& lowMap,
    unordered_set<Vertex*>& articulations
) {
    if (!parentMap[u] && u->adj_.size() > 1)
        articulations.insert(u);
    else
    {
        int minTime = accumulate(u->adj_.begin(), u->adj_.end(), numeric_limits<int>::infinity(),
        [&lowMap](int memo, Vertex* v)
        {
            return min(memo, lowMap[v]);
        });
        if (entryMap[u] < minTime)
            articulations.insert(u);
        lowMap[u] = minTime;
    }
}

void depthFirstSearch(
    Vertex* u,
    int& time,
    unordered_set<Vertex*>& visited,
    unordered_map<Vertex*, Vertex*>& parentMap,
    unordered_map<Vertex*, int>& entryMap,
    unordered_map<Vertex*, int>& lowMap,
    unordered_set<Vertex*>& articulations
) {
    if (visited.find(u) != visited.end())
        return;
    visited.insert(u);
    entryMap.insert(make_pair(u, time));
    lowMap.insert(make_pair(u, time));
    time++;
    for_each(u->adj_.begin(), u->adj_.end(),
        [&u, &time, &visited, &parentMap, &entryMap, &lowMap, &articulations](Vertex* v)
    {
        if (visited.find(v) != visited.end())
            return;
        parentMap[v] = u;
        depthFirstSearch(v, time, visited, parentMap, entryMap, lowMap, articulations);
    });
    processVertex(u, parentMap, entryMap, lowMap, articulations);
}

unordered_set<Vertex*> articulationVertices(
    unordered_set<Vertex*>& vertices
) {
    int time = 0;
    Vertex* root = *vertices.begin();
    unordered_set<Vertex*> visited;
    unordered_map<Vertex*, Vertex*> parentMap;
    unordered_map<Vertex*, int> entryMap;
    unordered_map<Vertex*, int> lowMap;
    parentMap.insert(make_pair(root, nullptr));
    unordered_set<Vertex*> articulations;
    depthFirstSearch(root, time, visited, parentMap, entryMap, lowMap, articulations);
    return articulations;
}

void printVertices(
    unordered_set<Vertex*>& vertices
) {
    cout << "Articulation Vertices: [";
    for (const Vertex* u : vertices)
        cout << u->val_ << " ";
    cout << "]" << endl;
}

int main(int argc, char* argv[])
{
    auto vertices = makeGraph();
    auto av = articulationVertices(vertices);
    printVertices(av);
    return EXIT_SUCCESS;
}
