#include <iostream>
#include <vector>

struct TEdge {
    long long from;
    long long to;
    long long weight;
};

using Graph = std::vector <TEdge>;

class TGraphSolver {
public:
    void Init(Graph& graph, int vertexes, int edges) {
        graph_ = std::move(graph);
        vertexes_ = vertexes;
        edges_ = edges;
    }

    void FindShortestPathByFordBellman(int from, int to) {
        long long ans = FindShortestPathByFordBellmanImpl(from, to);
        if (ans == inf_) {
            std::cout << "No solution" << std::endl;
        } else {
            std::cout << ans << std::endl;
        }
    }

private:
    int vertexes_ = 0;
    int edges_ = 0;
    const long long inf_ = 1e9 * 1000000 + 322;
    Graph graph_;

    long long FindShortestPathByFordBellmanImpl(int from, int to) {
        std::vector <long long> dist(vertexes_, inf_);
        bool updated = true;

        dist[from] = 0;
        while (updated) {
            updated = false;
            for (int i = 0; i < edges_; ++i) {
                if (graph_[i].from < inf_) {
                    if (dist[graph_[i].to] > dist[graph_[i].from] + graph_[i].weight) {
                        dist[graph_[i].to] = dist[graph_[i].from] + graph_[i].weight;
                        updated = true;
                    }
                }
            }
        }

        return dist[to];
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    int vertexes;
    int edges;
    int from;
    int to;

    std::cin >> vertexes >> edges >> from >> to;
    Graph graph(edges);

    for (int i = 0; i < edges; ++i) {
        long long u;
        long long v;
        long long weight;

        std::cin >> u >> v >> weight;
        graph[i] = {--u, --v, weight};
    }

    TGraphSolver solve;
    solve.Init(graph, vertexes, edges);
    solve.FindShortestPathByFordBellman(--from, --to);

    return 0;
}