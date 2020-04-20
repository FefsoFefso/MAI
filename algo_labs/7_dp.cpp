#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

int main() {
	std::ios::sync_with_stdio(false);

    int n;
    int m;
    std::cin >> n >> m;
    
    std::vector <std::vector <int>> matr((n + 1), std::vector <int> (m + 1, 1));
    for (int i = 1; i <= n; ++i) {
        std::string input;
        std::cin >> input;
        for (int j = 1; j <= m; ++j) {
            matr[i][j] = (input[j - 1] - '0');
        }
    }

    int ans = 0;
    std::vector <int> firts_up_1(m + 1, 0);
    std::vector <int> first_left_1(m + 1);
	std::vector <int> first_right_1(m + 1);
	for (int i = 1; i <= n; ++i) {
		for (int j = 1; j <= m; ++j) {
			if (matr[i][j] == 1) {
				firts_up_1[j] = i;
			}
		}

		std::stack <int> rows_dp;
		for (int j = 1; j <= m; ++j) {
			while (!rows_dp.empty() && firts_up_1[rows_dp.top()] <= firts_up_1[j]) {
				rows_dp.pop();
			}
			first_left_1[j] = rows_dp.empty() ? 0 : rows_dp.top();
			rows_dp.push(j);
		}

		while (!rows_dp.empty()) {
			rows_dp.pop();
		}

		for (int j = m; j > 0; --j) {
			while (!rows_dp.empty() && firts_up_1[rows_dp.top()] <= firts_up_1[j]) {
				rows_dp.pop();
			}
			first_right_1[j] = rows_dp.empty() ? m + 1 : rows_dp.top();
			rows_dp.push(j);
		}

		for (int j = 1; j <= m; ++j) {
			ans = std::max(ans, (i - firts_up_1[j]) * (first_right_1[j] - first_left_1[j] - 1));
		}

	}   
   
	std::cout << ans << std::endl;
    return 0;
}