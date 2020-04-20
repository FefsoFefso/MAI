#include <iostream>
#include <vector>
#include <algorithm>

typedef struct TSPolynom{
    std::vector <double> coefs;
    double cost;
    int id;
}TPolynom;

int main() {
    std::ios::sync_with_stdio(false);

    int n;
    int m;
    std::cin >> m >> n;

    std::vector <TPolynom> system(m);
    for (int i = 0; i < m; ++i) {
        system[i].coefs.resize(n, 0.0);
        for (int j = 0; j < n; ++j) {
            std::cin >> system[i].coefs[j];
        }
        std::cin >> system[i].cost;
        system[i].id = i + 1;
    }

    std::vector <int> ans;
    for (int i = 0; i < n; ++i) {
        int min_id = -1;
        double min_cost = 1234567.0;
        for (int j = i; j < m; ++j) {
            if (system[j].cost < min_cost && system[j].coefs[i] != 0.0) {
                min_cost = system[j].cost;
                min_id = j;
            }
        }

        if (min_id == -1) {
            std::cout << -1 << std::endl;
            return 0;
        }
        
        ans.push_back(system[min_id].id);
        std::swap(system[i], system[min_id]);
        
        for (int j = i + 1; j < m; ++j) {
            double attitude = system[j].coefs[i] / system[i].coefs[i];
            for (int q = i; q < n; ++q) {
                system[j].coefs[q] -= (attitude * system[i].coefs[q]);
            }
        }
    }

    std::sort(ans.begin(), ans.end());
    int ans_size = ans.size();
    for (int i = 0; i < ans_size; ++i) {
        if (i == ans_size - 1) {
            std::cout << ans[i] << std::endl;
        } else {
            std::cout << ans[i] << " ";
        }
    }
    
    return 0;
}