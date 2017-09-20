#include <iostream>
#include <typeinfo>
#include <vector>
#include <chrono>

using namespace std::chrono;

// Testing in terminal:
// g++ -o main -std=c++11 -Wall -Wextra -pedantic main.cpp && ./main
// g++ -v | Apple LLVM version 8.1.0 (clang-802.0.42)
using matrix = std::vector<std::vector<int>>;

class Result {
public:
    Result (int vertex_index, int matrix_accesses) : 
            vertex_index(vertex_index), 
            matrix_accesses(matrix_accesses) {}

    int getVertexIndex() const { return vertex_index; }
    int getMatrixAccesses() const { return matrix_accesses; }

private:
    int vertex_index;
    int matrix_accesses;
};

void print_matrix(matrix &m) {
    std::cout << "[";

    for (auto i = m.begin(); i != m.end(); i++) {
        std::cout << "[";

        for (auto j = (*i).begin(); j != (*i).end(); j++) {
            std::cout << *j;

            if ((j + 1) != (*i).end()) { std::cout << ", "; }
        }

        std::cout << "]";
        if ((i + 1) != m.end()) { std::cout << std::endl; }
    }
    std::cout << "]" << std::endl;
}

void print_pair(std::pair<int, int> &p) {
    std::cout << "{" << p.first << ", " << p.second << "}" << std::endl;
}

Result find_universelle_senke(matrix &m) {
    int requests = 0;
    int n = m.size();

    // outer loop = cols
    // inner loop = rows
    for (int j = 0; j < n; j++) {
        int deg_in = 0;

        for (int i = 0; i < n; i++) {
            deg_in += m[i][j];
            requests++;
        }

        if (deg_in == n - 1) {
            int deg_out = 0;

            // only cols
            for (int i = 0; i < n; i++) {
                deg_out += m[j][i];
                requests++;
            }

            if (deg_out == 0) {
                return Result(j, requests);
            }
        }
    }

    return Result(-1, requests);
}

// http://www.inf.fu-berlin.de/lehre/SS09/infb/muster03.pdf
Result find_universelle_senke_efficient(matrix &m) {
    int row = 0;
    int n = m.size();

    int requests = 0;
    
    // iterate over deg outs / represents column
    for (int i = 0; i < n; i++) {
        int deg_in = m[row][i];
        requests++;

        // In diesem Fall kann der erste Knoten, und alle < k keine totalen Senken sein. 
        // Man kann also direkt in die k-te Zeile gehen und dabei k + 1 weitermachen (weil man die Diagonale nicht beachtet).
        // Man geht nach jeder Anfrage also weiter nach rechts (es war eine 0) oder 
        // nach unten und nach rechts (es war eine 1).
        if (deg_in == 1) {
            row = i;

            // eigentlich i = row + 1 (unten und nach rechts), aber i wird dann durch i++ nochmals erhöht.
            // i = row; 
            // i = row + 1;
        }
    }

    // check row (deg out) and check col
    int deg_in = 0;
    for (int i = 0; i < n; i++) {
        int deg_out = m[row][i];
        requests++;

        if (deg_out != 0) {
            return Result(-1, requests);
        }

        deg_in += m[i][row];
        requests++;
    }

    if (deg_in != n - 1) {
        return Result(-1, requests);
    }

    return Result(row, requests);
}

void test_find_universelle_senke(matrix &adjacent_matrix) {
    print_matrix(adjacent_matrix); std::cout << std::endl;
    
    // [NORMAL] ===================
    high_resolution_clock::time_point senke_from = high_resolution_clock::now();
    Result senke = find_universelle_senke(adjacent_matrix);
    high_resolution_clock::time_point senke_until = high_resolution_clock::now();

    duration<double, std::nano> senke_dauer = senke_until - senke_from;
    // ===================

    // [EFFICIENT] ===================
    high_resolution_clock::time_point senke_efficient_from = high_resolution_clock::now();
    Result senke_efficient = find_universelle_senke_efficient(adjacent_matrix);
    high_resolution_clock::time_point senke_efficient_until = high_resolution_clock::now();

    duration<double, std::nano> senke_efficient_dauer = senke_efficient_until - senke_efficient_from;
    // ===================

    std::cout << "[normal][" << senke_dauer.count() << "ns] universelle Senke " << 
                        "bei Index: [" << senke.getVertexIndex() << "] " 
                        "mit |Matrixzugriffen| = " << senke.getMatrixAccesses()
                        << std::endl;

    std::cout << "[efficient][" << senke_efficient_dauer.count() << "ns] universelle Senke " << 
                        "bei Index: [" << senke_efficient.getVertexIndex() << "] " 
                        "mit |Matrixzugriffen| = " << senke_efficient.getMatrixAccesses() 
                        << std::endl;

    std::cout << "======================================================================" << std::endl;
}

int main() {
    matrix adjacent_matrix = {
        {0, 1, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0},
        {1, 0, 1, 0, 1},
        {0, 1, 1, 0, 0}
    };

    matrix adjacent_matrix_hard = {
        {0, 1, 0, 0, 1},
        {0, 0, 0, 1, 1},
        {0, 1, 0, 0, 1},
        {1, 0, 1, 0, 1},
        {0, 0, 0, 0, 0}
    };

    matrix adjacent_matrix_best = {
        {0, 0, 0, 0, 0},
        {1, 0, 1, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 0, 1, 0, 1},
        {1, 1, 1, 0, 0}
    };

    matrix adjacent_matrix_worst = {
        {0, 1, 0, 0, 1},
        {0, 0, 0, 1, 1},
        {0, 1, 0, 0, 1},
        {1, 0, 1, 0, 1},
        {0, 0, 0, 1, 1}
    };

    test_find_universelle_senke(adjacent_matrix);
    test_find_universelle_senke(adjacent_matrix_hard);
    test_find_universelle_senke(adjacent_matrix_best);
    test_find_universelle_senke(adjacent_matrix_worst);
}