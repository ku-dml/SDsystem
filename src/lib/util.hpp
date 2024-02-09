#ifndef __UTIL_HPP_
#define __UTIL_HPP_

#include <string>
#include <bitset>
#include "lib.hpp"

class Param {
    public:
        std::string input_file_name_;
        std::string outname_;
        std::string common_outname_;
        double tlime_;
        integer exp_type_;
    public:
        Param()
            : input_file_name_()
            , outname_(INI_outname)
            , common_outname_(INI_common_outname)
            , tlime_(INI_tlim)
        { }
};

void checkArgs(int argc, char *argv[]);
Param readArgs(int argc, char *argv[]);

void writeResults(Param& param, integer numCount, double duration, bool is_finished);


#endif