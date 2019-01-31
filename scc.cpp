#include <iostream>
#include <vector>
#include <cstdlib>
#include <stack>
#include <unordered_set>
#include <algorithm>

using namespace std;

struct Vertex
{
    Vertex(const int val) : val_(val), adj_() {}
    int val_;
    vector<Vertex*> adj_;
};

void printComponents(
    const vector<unordered_set<Vertex*>>& components
) {
    cout << "Components: " << endl;
    for (const unordered_set<Vertex*>& component : components)
    {
        cout << "[ ";
        for (const Vertex* v : component)
        {
            cout << v->val_ << " ";
        }
        cout << "]" << endl;
    }
}

vector<Vertex*> makeGraph()
{
    Vertex* v1 = new Vertex(1); Vertex* v2 = new Vertex(2); Vertex* v3 = new Vertex(3);
    Vertex* v4 = new Vertex(4); Vertex* v5 = new Vertex(5); Vertex* v6 = new Vertex(6);
    Vertex* v7 = new Vertex(7);

    v1->adj_.emplace_back(v2); v2->adj_.emplace_back(v3); v3->adj_.emplace_back(v1);
    v3->adj_.emplace_back(v4); v4->adj_.emplace_back(v5); v5->adj_.emplace_back(v6);
    v6->adj_.emplace_back(v4); v6->adj_.emplace_back(v7);

    return { v1, v2, v3, v4, v5, v6, v7 };
}

void depthFirstSearch(
    Vertex* u,
    stack<Vertex*>& crumbs,
    unordered_set<Vertex*>& visited
) {
    visited.insert(u);
    for_each(u->adj_.begin(), u->adj_.end(), [&crumbs, &visited](Vertex* v)
    {
        if (visited.find(v) != visited.end())
            return;
        else
            depthFirstSearch(v, crumbs, visited);
    });
    crumbs.push(u);
}

void reverseGraph(
    vector<Vertex*>& vertices
) {
    vector<vector<Vertex*>> adjList;
    for_each(vertices.begin(), vertices.end(), [&adjList](Vertex* u)
    {
        adjList.emplace_back(u->adj_);
        u->adj_.clear();
    });
    for (int i=0; i < vertices.size(); i++)
    {
        Vertex* u = vertices[i];
        vector<Vertex*>& adj = adjList[i];
        for (const Vertex* v : adj)
        {
            auto iter = find(vertices.begin(), vertices.end(), v);
            if (iter != vertices.end())
            {
                (*iter)->adj_.emplace_back(u);
            }
            else
            {
                cout << "Did not find vertex: " << v->val_ << " in reverse graph" << endl;
            }
        }
    };
}

void depthFirstComponent(
    Vertex* u,
    unordered_set<Vertex*>& component,
    unordered_set<Vertex*>& visited
) {
    visited.insert(u);
    component.insert(u);
    for_each(u->adj_.begin(), u->adj_.end(), [&component, &visited](Vertex* v)
    {
        if (visited.find(v) != visited.end())
            return;
        depthFirstComponent(v, component, visited);
    });
}

vector<unordered_set<Vertex*>> getComponents(
    const vector<Vertex*>& vertices,
    stack<Vertex*>& crumbs,
    unordered_set<Vertex*>& visited
) {
    vector<unordered_set<Vertex*>> components;
    while(!crumbs.empty())
    {
        Vertex* u = crumbs.top();
        crumbs.pop();
        if (visited.find(u) != visited.end())
            continue;
        unordered_set<Vertex*> component;
        depthFirstComponent(u, component, visited);
        components.emplace_back(component);
    }
    return components;
}

vector<unordered_set<Vertex*>> connectedComponents(
    vector<Vertex*>& vertices
) {
    stack<Vertex*> crumbs;
    unordered_set<Vertex*> visited;
    for_each(vertices.begin(), vertices.end(), [&crumbs, &visited](Vertex* u)
    {
        if (visited.find(u) != visited.end())
            return;
        else
            depthFirstSearch(u, crumbs, visited);
    });
    reverseGraph(vertices);
    visited.clear();
    return getComponents(vertices, crumbs, visited);
}

int main(int argc, char* argv[])
{
    auto vertices = makeGraph();
    auto components = connectedComponents(vertices);
    printComponents(components);
    return EXIT_SUCCESS;
}
