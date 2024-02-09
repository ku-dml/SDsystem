#include "util.hpp"
#include "graph.hpp"
#include "lib.hpp"

#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>

using namespace std;

void checkArgs(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr,
            "usage: %s "
            "(graph_file)"
            "[(param1)(value1)(param2)...]\n\n",
            argv[0]);
            fprintf(stderr, "list of params:\n");
        fprintf(stderr,
            "  -tlim <DOUBLE> ... time limit in sec; if <0, it is inf. (%g)\n",
            INI_tlim);
        fprintf(
            stderr,
            "  -outname <STR> ... filename of output from vtable & itable (%s)\n",
            INI_outname);
        fprintf(
            stderr,
            "  -common-outname <STR> ... filename of common output from vtable & itable (%s)\n",
            INI_common_outname);
        fprintf(
            stderr,
            "  -type <INT> ... exp type (%d)\n",
            0);
        exit(EXIT_FAILURE);
    }
}

Param readArgs(int argc, char *argv[]) {
    checkArgs(argc, argv);

    auto param = Param();
    param.input_file_name_ = argv[1];

    char *arg;
    for (int k = 2; k < argc; k += 2) {
        if (k == argc - 1 || argv[k][0] != '-') {
          fprintf(stderr, "error: arguments are not set appropriately.\n");
          exit(EXIT_FAILURE);
        }
        arg = argv[k] + 1;
        if (strcmp(arg, "tlim") == 0)
          param.tlime_ = atof(argv[k + 1]);
        else if (strcmp(arg, "outname") == 0)
          param.outname_ =  string(argv[k + 1]);
        else if (strcmp(arg, "common-outname") == 0)
          param.common_outname_ = string(argv[k + 1]);
        else if (strcmp(arg, "type") == 0)
            param.exp_type_ = atof(argv[k + 1]);
        else {
          fprintf(stderr, "error: <%s> is illegal parameter.\n", arg);
          exit(EXIT_FAILURE);
        }
    }

    return param;
}

void writeResults(Param& param, integer numCount, double duration, bool is_finished) {
    ofstream file;
    file.open(param.common_outname_, ios::app);

    file << param.exp_type_ << "," << param.input_file_name_ << ",";
    file << numCount << "," << duration << "," << is_finished << endl;

    file.close();
}