#include "htf_cmdline.h"


int main(int argc, char* argv[])
try {
    htf::cmdline::htf_cmdline_parse(argc, argv);
    return 0;
}
catch (const std::exception& e) {
    htf::cout_error(e.what());
    return 1;
}
catch (...) {
    htf::cout_error("unknown error!!!");
    return 2;
}