#include <fstream>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <random>

int main(int argc, char *argv[]) {
    using namespace std;

    int n = atoi(argv[1]);
    float p = atof(argv[2]);
    int seed = atoi(argv[3]);
    string output = string(argv[4]);

    srand(seed);
    ofstream file;
    file.open(output, ios::out);

    for (int u = 0; u < n; ++u) {
        for (int v = u + 1; v < n; ++ v) {
            if (rand() / static_cast<float>(RAND_MAX) < p) {
                file << u << "\t1\t" << v << endl;
            }
        }
    }

    file.close();
}