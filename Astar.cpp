#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <stack>

using namespace std;

// 坐标
struct Node {
    int x;
    int y;
    double g;
    double h;
    Node* parent;

    Node(int x_, int y_, double g_ = 0, double h_ = 0, Node* parent_ = nullptr) :
        x{ x_ }, y{ y_ }, g{ g_ }, h{ h_ }, parent(parent_) {}

    double f() const {
        return g + h;
    }
};

// mycmp
struct myCmp {
    bool operator()(const Node& n1, const Node& n2) {
        return n1.f() > n2.f();
    }
};

// A*算法
vector<pair<int, int>> astar(vector<vector<int>>& grid, pair<int, int> start, pair<int, int> end) {
    int m = grid.size();
    int n = grid[0].size();

    auto heuristic = [&](int x1, int y1, int x2, int y2) {
        return static_cast<double>(abs(x1 - x2) + abs(y1 - y2));
        };

    int dir[4][2] = {1, 0, -1, 0, 0, 1, 0, -1};

    priority_queue<Node, vector<Node>, myCmp> openlist;
    vector<vector<int>> closedlist(m, vector<int>(n, 0));

    openlist.push(Node{ start.first, start.second,
        0, heuristic(start.first, start.second, end.first, end.second) });

    while (!openlist.empty()) {
        Node nd = openlist.top();
        openlist.pop();

        if (nd.x == end.first && nd.y == end.second) {
            // end
            stack<pair<int, int>> path;
            Node* node = &nd;
            while (node) {
                path.push({ node->x, node->y });
                node = node->parent;
            }

            vector<pair<int, int>> result;
            while (!path.empty()) {
                result.push_back(path.top());
                path.pop();
            }
            return result;
        }
        
        // visited
        closedlist[nd.x][nd.y] = 1;

        // update
        for (int i = 0; i < 4; ++i) {
            int xx = nd.x + dir[i][0];
            int yy = nd.y + dir[i][1];
            if (xx < 0 || yy < 0 || xx >= m || yy >= n || closedlist[xx][yy] == 1) {
                continue;
            }
            if (grid[xx][yy] == 0) {
                double g_new = nd.g + 1;
                double h_new = heuristic(xx, yy, end.first, end.second);

                openlist.push(Node{ xx, yy, g_new, h_new, new Node{nd} });
            }
        }
        
    }
    
}

// 可视化网格
void visualizeGrid(const vector<vector<int>>& grid, const vector<pair<int, int>>& path, pair<int, int> start, pair<int, int> end) {
    vector<vector<char>> visual(grid.size(), vector<char>(grid[0].size(), ' '));

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            if (grid[i][j] == 1) {
                visual[i][j] = '#';
            }
        }
    }

    for (auto& p : path) {
        if (p != start && p != end) {
            visual[p.first][p.second] = '*';
        }
    }

    visual[start.first][start.second] = 'S';
    visual[end.first][end.second] = 'E';

    for (auto& row : visual) {
        for (auto& cell : row) {
            cout << cell << ' ';
        }
        cout << endl;
    }
}

int main() {
    // 0: 可通行, 1: 障碍物
    vector<vector<int>> grid = {
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
        {0, 1, 1, 1, 0, 0, 1, 0, 1, 0},
        {0, 1, 0, 0, 0, 1, 0, 0, 1, 0},
        {0, 1, 0, 1, 0, 0, 0, 1, 1, 0},
        {0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 1, 0, 0},
        {1, 1, 0, 1, 0, 0, 0, 1, 1, 1},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 1, 1, 1, 0, 0, 0, 1, 0, 0},
    };

    pair<int, int> start = { 0, 0 };
    pair<int, int> end = { 9, 9 };

    vector<pair<int, int>> path = astar(grid, start, end);

    if (!path.empty()) {
        cout << "Found path:" << endl;
        visualizeGrid(grid, path, start, end);
    }
    else {
        cout << "No path found!" << endl;
    }

    return 0;
}
