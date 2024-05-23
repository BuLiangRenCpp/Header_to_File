#ifdef _WIN32
#define LIB_EXPORT
#endif

#include "Resolve.h" 
#include "args_const.h"
#include "Hpath.h"
#include "Hdir.h"
#include "preprocess.h"
#include "compile.h"

using namespace std;
using namespace output;

namespace htf {
    using namespace path_deal;

    namespace argument{
        Resolve::Resolve() 
        {

        }

        void Resolve::resolve(const Htf_args& args) 
        {
            _htf_ins(args);
        }

        // -------------------- private ----------------------

        static void ret_value(const Htf_args& htf_args, bool& is_force, vector<string>& sources, vector<string> & includes, string& target_dir)
        {
            vector<Arg> args = htf_args.args();
            if (args[0].key().ch() == args_const::FORCE_ARG) is_force = true;

            auto size = args.size();
            if (size == 1) sources = args[0].con().con();       // 1. -i
            else if (size == 2) {
                if (is_force) {      // 2.1 -f -i
                    sources = args[1].con().con();
                } else {        
                    sources = args[0].con().con(); 
                    if (args[1].key().ch() == args_const::OUTPUT_ARG)      // 2.2 -i -o
                        target_dir = args[1].con().con()[0];        // ! -o 只有一个参数
                    else includes = args[1].con().con();        // 2.3 -i -I
                }
            }
            else if (size == 3) {     
                if (is_force) {   
                    sources = args[1].con().con();    
                    if (args[2].key().ch() == args_const::OUTPUT_ARG)     // 3.1 -f -i -o 
                        target_dir = args[2].con().con()[0];
                    else includes = args[2].con().con();        // 3.2 -f -i -I
                } else {        // 3.3 -i -o -I
                    sources = args[0].con().con();
                    target_dir = args[1].con().con()[0];
                    includes = args[2].con().con();
                }
            } 
            else {      // 4. -f -i -o -I
                sources = args[1].con().con();
                target_dir =  args[2].con().con()[0];
                includes = args[3].con().con();
            }
        }

        void Resolve::_htf_ins(const Htf_args& htf_args) 
        {
            vector<Arg> args = htf_args.args();
            vector<string> sources, includes;
            string target_dir = current_dir();      // * 默认
            bool is_force = false;
            ret_value(htf_args, is_force, sources, includes, target_dir);
            // ***********************************************************
            vector<Hpath> hpaths;
            for (const auto& s : sources) 
                hpaths.emplace_back(Hpath{s});
            Hdir hdir(target_dir);
            vector<Hdir> include_hdirs;
            for (const auto& s : includes) {
                include_hdirs.emplace_back(Hdir{s});
            }
            // *********************** main ********************************
            output::print_result("dependency processing...");
            auto ts = htf::core::preprocess(hpaths, include_hdirs);
            output::print_result("file outputting...");
            htf::core::compile(ts, hdir, is_force);
            output::print_result("over");
        }
    }
}


