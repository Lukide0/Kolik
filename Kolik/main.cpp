#include "main.h"

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        command::help();
        return STATUS_OK;
    }

    std::string_view cmd = argv[1];

    if (cmd == "test" && argc > 2)
        return command::test(argc, argv);
    else if (cmd == "help")
    {
        command::help();
        return STATUS_OK;
    }
    else
    {
        command::help();
        return STATUS_ERR;
    }
}