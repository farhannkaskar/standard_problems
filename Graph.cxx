#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

class Edge {
public:
  const int from;
  const int to;
  const int cost;

  Edge(int from, int to, int cost): from(from), to(to), cost(cost) {}
};

class Node {
public:
     int node;
     int cost;

    Node(const int node, const int cost): node(node), cost(cost) {}
    Node(const  Node& other): node(other.node), cost(other.cost) {}
    Node& operator=(Node&& other) {
      this->node = other.node;
      this->cost= other.cost;
    }
};

class Graph {
public:
  const int V; // Total vertices
  vector<vector<Edge>> all_edges;

  Graph(const int V): V(V) {
    all_edges = vector<vector<Edge>>(V);
  }

  void addEdge(Edge edge) {
    const bool isValidEdge = edge.from >= 0 && edge.from < V &&
                             edge.to >=0 && edge.to < V;

    if (isValidEdge) {
      all_edges[edge.from].push_back(edge);
    }
  }

  void printBFS(const int source) {
    const bool isValidSource = source >=0 && source < V;

    if (!isValidSource) {
      return;
    }

    std::queue<int> queue;
    std::vector<bool> visited(V, false);
    queue.push(source);

    while(!queue.empty()) {
      const int currentNode = queue.front();
      queue.pop();

      if (visited[currentNode]) {
        continue;
      }
      cout << currentNode << ", ";
      visited[currentNode] = true;

      for(Edge eachEdge: all_edges[currentNode]) {
        if (!visited[eachEdge.to])
          queue.push(eachEdge.to);
      }
    }

    cout << endl;
  }

  bool isValidNode(const int node) {
    return node >= 0 && node < V;
  }

  vector<int>* findShortestPathUsingDijkstra(const int source) {
    if (!isValidNode(source)) {
      return NULL;
    }

    vector<int>* nodeToCost = new vector<int>(V, INT_MAX);
    (*nodeToCost)[source] = 0;

    auto nodeCompare = [](Node first, Node second) {
      return first.cost > second.cost;
    };

    std::priority_queue<Node, vector<Node>, decltype(nodeCompare)> minHeap(nodeCompare);
    vector<bool> visited(V, false);
    Node sourceNode(source, 0);
    minHeap.push(sourceNode);

    while(!minHeap.empty()) {
      Node currentNode = minHeap.top();
      minHeap.pop();
      if (visited[currentNode.node]) {
        continue;
      }

      visited[currentNode.node] = true;;

      for (Edge eachEdge: all_edges[currentNode.node]) {
        const int connection = eachEdge.to;
        const int connectionCost = eachEdge.cost + currentNode.cost;

        if (visited[connection]) {
          continue;
        }

        (*nodeToCost)[connection] = (*nodeToCost)[connection] > connectionCost
                                  ? connectionCost
                                  : (*nodeToCost)[connection];

        Node connectedNode(connection, (*nodeToCost)[connection]);

        minHeap.push(connectedNode);
      }
    }

    return nodeToCost;
  }
};

int main() {
  Graph graph(6);
  graph.addEdge(Edge(0, 1, 2));
  graph.addEdge(Edge(0, 2, 4));
  graph.addEdge(Edge(1, 2, 1));
  graph.addEdge(Edge(1, 3, 7));
  graph.addEdge(Edge(2, 4, 3));
  graph.addEdge(Edge(4, 3, 2));
  graph.addEdge(Edge(3, 5, 1));
  graph.addEdge(Edge(4, 5, 5));

  cout << "Printing BFS" << endl;
  graph.printBFS(0);
  vector<int>* shortestPath = graph.findShortestPathUsingDijkstra(0);

  if (shortestPath != NULL) {
    for(int index = 0; index < shortestPath->size(); ++index) {
      cout << "[" << index << "] ==> " << (*shortestPath)[index] << endl;
    }
  }
  return 0;
}
