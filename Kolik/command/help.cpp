#include "help.h"

namespace command
{

void help()
{
    // clang-format off
    std::cout << "usage: kolik command\n"
                 "\n"
                 "Commands:\n"
                 "  test <PATH> [options]\n"
                 "  help\n"
                 "\n"
                 "Options:\n"
                 "  --compare-file <PATH>      A file which contains inputs and outputs for program\n"
                 "  --save <PATH>              Save results to file\n"
                 "  --test-separator <SEP>     How to separate the indiviual inputs with outputs in --compare_file(default '---')\n"
                 "  --separator <SEP>          How to separate the input and output in --compare_file(default is '---')\n"
                 "  --timeout <N>              timeout for single run(default is 10000)\n"
                 "  --lang <LANG>              Specify file type\n"
                 "  --quiet                    Don't print to a termnali\n"
                 "\n"
                 "Supported file types:\n"
                 "  python                     Python file\n"
                 "  exe                        Executable file\n";
    // clang-format on
}

}