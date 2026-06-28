#include <iostream>
#include <queue>
#include <climits>

using namespace std;

// ---------- DOUBLY LINKED LIST NODE ----------
struct Node {
    int v, w;
    Node* next;
    Node* prev;

    Node(int vertex, int weight) {
        v = vertex;
        w = weight;
        next = prev = nullptr;
    }
};

class Graph {
    int V;
    bool directed;
    Node** adj;

public:
    Graph(int v, bool dir) {
        V = v;
        directed = dir;
        adj = new Node * [V];
        for (int i = 0; i < V; i++)
            adj[i] = nullptr;
    }

    // ---------- DESTRUCTOR (MEMORY CLEANUP) ----------
    ~Graph() {
        for (int i = 0; i < V; i++) {
            Node* curr = adj[i];
            while (curr) {
                Node* temp = curr;
                curr = curr->next;
                delete temp;
            }
        }
        delete[] adj;
    }

    // ---------- SORTED INSERT ----------
    void insertSorted(Node*& head, int v, int w) {
        Node* newNode = new Node(v, w);

        if (!head || v < head->v) {
            newNode->next = head;
            if (head) head->prev = newNode;
            head = newNode;
            return;
        }

        Node* curr = head;
        while (curr->next && curr->next->v < v)
            curr = curr->next;

        newNode->next = curr->next;
        if (curr->next)
            curr->next->prev = newNode;

        curr->next = newNode;
        newNode->prev = curr;
    }

    void addEdge(int u, int v, int w) {
        if (u < 0 || v < 0 || u >= V || v >= V) {
            cout << "Invalid edge!\n";
            return;
        }
        insertSorted(adj[u], v, w);
        if (!directed)
            insertSorted(adj[v], u, w);
    }

    // ---------- VISUAL GRAPH ----------
    void visualGraph() {
        cout << "\n===== VISUAL GRAPH =====\n";
        bool printed[50][50] = { false };

        for (int i = 0; i < V; i++)
            cout << "[" << i << "] ";
        cout << "\n\nEdges:\n";

        for (int u = 0; u < V; u++) {
            Node* curr = adj[u];
            while (curr) {
                int v = curr->v;
                int w = curr->w;
                if (!printed[u][v]) {
                    cout << "[" << u << "] --(" << w << ")--> [" << v << "]\n";
                    printed[u][v] = true;
                    if (!directed) printed[v][u] = true;
                }
                curr = curr->next;
            }
        }
    }

    // ---------- ADJ LIST ----------
    void displayAdjList() {
        cout << "\nAdjacency List:\n";
        for (int i = 0; i < V; i++) {
            cout << i << " -> ";
            Node* curr = adj[i];
            while (curr) {
                cout << "(" << curr->v << "," << curr->w << ") ";
                curr = curr->next;
            }
            cout << endl;
        }
    }

    // ---------- BFS ----------
    void BFS() {
        bool visited[50] = { false };
        queue<int> q;

        cout << "\nBFS Traversal: ";
        for (int i = 0; i < V; i++) {
            if (!visited[i]) {
                visited[i] = true;
                q.push(i);

                while (!q.empty()) {
                    int u = q.front(); q.pop();
                    cout << u << " ";

                    Node* curr = adj[u];
                    while (curr) {
                        int v = curr->v;
                        if (!visited[v]) {
                            visited[v] = true;
                            q.push(v);
                        }
                        curr = curr->next;
                    }
                }
            }
        }
        cout << endl;
    }

    // ---------- DFS ----------
    void DFSUtil(int u, bool visited[]) {
        visited[u] = true;
        cout << u << " ";

        Node* curr = adj[u];
        while (curr) {
            int v = curr->v;
            if (!visited[v])
                DFSUtil(v, visited);
            curr = curr->next;
        }
    }

    void DFS() {
        bool visited[50] = { false };
        cout << "\nDFS Traversal: ";
        for (int i = 0; i < V; i++)
            if (!visited[i])
                DFSUtil(i, visited);
        cout << endl;
    }

    // ---------- DIJKSTRA ----------
    void dijkstra(int src) {
        int dist[50], parent[50];
        bool visited[50] = { false };

        for (int i = 0; i < V; i++) {
            dist[i] = INT_MAX;
            parent[i] = -1;
        }

        dist[src] = 0;

        for (int c = 0; c < V - 1; c++) {
            int u = -1, minDist = INT_MAX;
            for (int i = 0; i < V; i++)
                if (!visited[i] && dist[i] < minDist) {
                    minDist = dist[i];
                    u = i;
                }

            if (u == -1) break;
            visited[u] = true;

            Node* curr = adj[u];
            while (curr) {
                int v = curr->v;
                int w = curr->w;
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                }
                curr = curr->next;
            }
        }

        cout << "\nShortest Paths from " << src << ":\n";
        for (int i = 0; i < V; i++) {
            cout << "Node " << i << " Distance: ";
            if (dist[i] == INT_MAX) cout << "INF\n";
            else {
                cout << dist[i] << " | Path: ";
                printPath(i, parent);
                cout << endl;
            }
        }
    }

    void printPath(int v, int parent[]) {
        if (v == -1) return;
        printPath(parent[v], parent);
        cout << v << " ";
    }

    // ---------- DEGREE INFO ----------
    void degreeInfo() {
        if (directed) {
            for (int i = 0; i < V; i++) {
                int out = 0, in = 0;
                Node* curr = adj[i];
                while (curr) { out++; curr = curr->next; }

                for (int j = 0; j < V; j++) {
                    Node* t = adj[j];
                    while (t) {
                        if (t->v == i) in++;
                        t = t->next;
                    }
                }

                cout << "Vertex " << i << " | In: " << in
                    << " | Out: " << out << endl;
            }
        }
        else {
            for (int i = 0; i < V; i++) {
                int degree = 0;
                Node* curr = adj[i];
                while (curr) { degree++; curr = curr->next; }
                cout << "Vertex " << i << " | Degree: " << degree << endl;
            }
        }
    }

    // ---------- GRAPH INFO ----------
    void graphInfo() {
        int edges = 0;
        for (int i = 0; i < V; i++) {
            Node* curr = adj[i];
            while (curr) { edges++; curr = curr->next; }
        }
        if (!directed) edges /= 2;

        bool visited[50] = { false };
        dfsConnectivity(0, visited);

        bool connected = true;
        for (int i = 0; i < V; i++)
            if (!visited[i])
                connected = false;

        cout << "\nVertices: " << V;
        cout << "\nEdges: " << edges;
        cout << "\nGraph Status: "
            << (connected ? "CONNECTED" : "DISCONNECTED") << endl;
    }

    void dfsConnectivity(int u, bool visited[]) {
        visited[u] = true;
        Node* curr = adj[u];
        while (curr) {
            if (!visited[curr->v])
                dfsConnectivity(curr->v, visited);
            curr = curr->next;
        }
    }

    // ---------- TIME COMPLEXITY ----------
    void showComplexity() {
        int ch;
        cout << "\n1. BFS\n2. DFS\n3. Dijkstra\nChoose function: ";
        cin >> ch;

        if (ch == 1 || ch == 2)
            cout << "Time Complexity: O(V + E)\n";
        else if (ch == 3)
            cout << "Time Complexity: O(V^2)\n";
        else
            cout << "Invalid choice\n";
    }
};

// ---------- MAIN ----------
int main() {
    cout << "                                    Ahmed and Afzal DS Algorithms\n\n";
    cout << "Enter Graph details:\n\n";

    int V, E, dir;
    cout << "Enter number of vertices: ";
    cin >> V;

    cout << "1. Directed\n2. Undirected\nChoose graph type: ";
    cin >> dir;

    Graph g(V, dir == 1);

    cout << "Enter number of edges: ";
    cin >> E;

    for (int i = 0; i < E; i++) {
        int u, v, w;
        cout << "Enter edge (u v weight): ";
        cin >> u >> v >> w;
        if (w < 0) { i--; continue; }
        g.addEdge(u, v, w);
    }

    int ch;
    do {
        cout << "\n===== GRAPH MENU =====";
        cout << "\n1. Visual Graph";
        cout << "\n2. Adjacency List";
        cout << "\n3. BFS";
        cout << "\n4. DFS";
        cout << "\n5. Dijkstra";
        cout << "\n6. Degree Info";
        cout << "\n7. Graph Info";
        cout << "\n8. Time Complexity";
        cout << "\n9. Exit";
        cout << "\nEnter choice: ";
        cin >> ch;

        if (ch == 1) g.visualGraph();
        else if (ch == 2) g.displayAdjList();
        else if (ch == 3) g.BFS();
        else if (ch == 4) g.DFS();
        else if (ch == 5) {
            int s; cout << "Enter source: "; cin >> s;
            g.dijkstra(s);
        }
        else if (ch == 6) g.degreeInfo();
        else if (ch == 7) g.graphInfo();
        else if (ch == 8) g.showComplexity();

    } while (ch != 9);

    cout << "\nProgram Ended Successfully.\n";
    return 0;
}