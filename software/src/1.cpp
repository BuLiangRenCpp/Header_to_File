#include "htf.h"



int main()
try{
    prompt::prompt();
    htf::header_to_file(R"(D:\5.temp_txt\dir\code1.h)");
    //htf::header_to_files(R"(D:\5.temp_txt\dir)");
    return 0;
}catch(string& e)
{
    prompt::print_error(e);
    return 1;
}