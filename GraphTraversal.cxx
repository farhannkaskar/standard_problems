#include <iostream>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

class Edge {
public:
  const int to;
  const int from;
  Edge(const int from_, const int to_)
    :to(to_),
     from(from_){}
};

class Graph {
public:
  const int V;  // Total vertices
  vector<vector<Edge> > allEdges;

  Graph(const int V_)
  :V(V_) {
    if (V < 0) throw std::invalid_argument("V cannot be negative");
    allEdges.resize(V);
  }

  void addEdge(const Edge& edge) {
    const int from =edge.from;
    if (from >=0 && from < V) {
      allEdges[from].push_back(edge);
    }
  }

  void printBfs(const int from) {
    if (from < 0 || from > V) {
      return;
    }

    std::queue<int> lifoQueue;
    lifoQueue.push(from);
    vector<bool> visited(V, false);

    while(!lifoQueue.empty()) {
      const int vertex = lifoQueue.front();
      lifoQueue.pop();
      if (visited[vertex]) continue;

      cout << vertex << ", ";

      visited[vertex] = true;
      const vector<Edge>& connectedEdges = allEdges[vertex];
      for (const Edge& edge: connectedEdges) {
        if(!visited[edge.to])
          lifoQueue.push(edge.to);
      }
    }
  }

  void printDfsUtils(const int from, vector<bool>* visitedPtr) {
    if (from < 0 || from >= V || visitedPtr == NULL && visitedPtr->size() >= V) return;

    cout << from << ", ";
    (*visitedPtr)[from] = true;
    const vector<Edge>& connectedEdges = allEdges[from];

    for (const Edge& edge : connectedEdges) {
      if(!(*visitedPtr)[edge.to]) {
        printDfsUtils(edge.to, visitedPtr);
      }
    }
  }

  void printDfs(const int from) {
    if (from < 0 || from >= V) return;

    vector<bool> visited(V, false);
    printDfsUtils(from, &visited);
  }

  void topologicalSortUtils(const int rootVertex, vector<int>* visitedPtr, stack<int>* topologicalSortedListPtr) {
      if (rootVertex < 0 || rootVertex >= V || topologicalSortedListPtr == NULL) return;

      if ((*visitedPtr)[rootVertex]) return;

      (*visitedPtr)[rootVertex] = true;

      const vector<Edge>& connectedEdges = allEdges[rootVertex];

      for (const Edge& edge: connectedEdges) {
        if ((*visitedPtr)[edge.to]) continue;

        topologicalSortUtils(edge.to, visitedPtr, topologicalSortedListPtr);
      }

      topologicalSortedListPtr->push(rootVertex);
  }

  void topologicalSort(vector<int>* topologicalSortedListPtr) {
    if (topologicalSortedListPtr == NULL) return;

    vector<int> visited(V, false);
    std::stack<int> topologicalSortedStack;
    for (int vertex = 0; vertex < V; ++vertex) {
      topologicalSortUtils(vertex, &visited, &topologicalSortedStack);
    }

    topologicalSortedListPtr->clear();
    topologicalSortedListPtr->resize(V);
    int index = 0;
    while(!topologicalSortedStack.empty()) {
      (*topologicalSortedListPtr)[index++] = topologicalSortedStack.top();
      topologicalSortedStack.pop();
    }
  }
};

int main(int argc, char** argv) {
  Graph graph(7);
  graph.addEdge(Edge(0, 1));
  graph.addEdge(Edge(0, 2));
  graph.addEdge(Edge(1, 2));
  graph.addEdge(Edge(2, 4));
  graph.addEdge(Edge(1, 3));
  graph.addEdge(Edge(4, 3));
  graph.addEdge(Edge(3, 5));
  graph.addEdge(Edge(4, 5));
  graph.addEdge(Edge(6, 0));

  cout << "BFS Traversal : ";
  graph.printBfs(0);
  cout << endl;
  cout << "DFS Traversal : ";
  graph.printDfs(0);
  cout << endl;
  vector<int> topologicallySortedList;
  graph.topologicalSort(&topologicallySortedList);

  cout << "Topologically Sorted List : ";
  for (int vertex : topologicallySortedList) {
    cout << vertex << ", ";
  }
  return 0;
}
