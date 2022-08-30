#include "test.h"

namespace command
{

std::string pretty_time(long long ms)
{
    if (ms < 1000)
        return std::to_string(ms) + "ms";
    else
        return std::to_string(ms / 1000) + "s";
}

Options parser_args(int argc, char* argv[], bool& err)
{
    Options opts;

    for (int i = 3; i < argc; i++)
    {
        std::string_view tmp = argv[i];

        // Compare file
        if (tmp == "--compare-file")
        {
            if (i + 1 == argc)
            {
                std::cout << "ERROR: Missing --compare-file value\n";
                err = true;
                break;
            }

            opts.compareFile = argv[++i];
        }
        // Save file path
        else if (tmp == "--save")
        {
            if (i + 1 == argc)
            {
                std::cout << "ERROR: Missing --save value\n";
                err = true;
                break;
            }

            opts.saveFilePath = argv[++i];
        }
        // Tests separator
        else if (tmp == "--test-separator")
        {
            if (i + 1 == argc)
            {
                std::cout << "ERROR: Missing --test-separator value\n";
                err = true;
                break;
            }

            opts.testSeparator = argv[++i];
        }
        // input/output separator
        else if (tmp == "--separator")
        {
            if (i + 1 == argc)
            {
                std::cout << "ERROR: Missing --separator value\n";
                err = true;
                break;
            }

            opts.separator = argv[++i];
        }
        // timeout
        else if (tmp == "--timeout")
        {
            if (i + 1 == argc)
            {
                std::cout << "ERROR: Missing --timeout value\n";
                err = true;
                break;
            }

            try
            {
                opts.timeout = std::stoul(argv[++i]);
            }
            catch (const std::exception& e)
            {
                std::cout << "ERROR: Invalid --timeout value\n";
                err = true;
                break;
            }
        }
        // Language
        else if (tmp == "--lang")
        {
            if (i + 1 == argc)
            {
                std::cout << "ERROR: Missing --lang value\n";
                err = true;
                break;
            }
            std::string_view lang = argv[++i];

            if (lang == "python")
                opts.lang = Options::PYTHON;
            else if (lang == "exe")
                opts.lang = Options::EXE;
            else
            {
                std::cout << "ERROR: Invalid --lang value\n";
                err = true;
                break;
            }
        }
        // quiet
        else if (tmp == "--quiet")
        {
            opts.quiet = true;
        }
        else
        {
            std::cout << "ERROR: Unknown option '" << tmp << "'\n";
            err = true;
        }
    }

    return opts;
}

int test(int argc, char* argv[])
{
    bool err;
    Options opts = parser_args(argc, argv, err);
    process::ProcessResults results;

    if (err)
        return 1;

    bool saveResults = opts.saveFilePath != "";
    if (saveResults)
    {
        if (std::filesystem::exists(opts.saveFilePath))
        {
            std::cout << "ERROR: Save file already exists";
            return false;
        }
    }

    // run tests
    bool status = process::test_program(argv[2], opts, results);

    if (!status)
        return 1;

    if (saveResults)
    {
        if (!save::save_results(results, opts))
            return 1;
    }

    if (opts.quiet)
        return 0;

    std::cout << "RESULTS: \n";

    int i = 1;
    for (const auto& result : results)
    {
        std::cout << "TEST " << i << ":\n";

        std::cout << "  TIME: " << pretty_time(result.time) << '\n' << "  STATUS: ";
        switch (result.status)
        {
        case process::ProcessInfo::CORRECT_OUTPUT:
            std::cout << "OK\n";
            break;
        case process::ProcessInfo::WRONG_OUTPUT:
            std::cout << "FAIL\n";
            break;
        case process::ProcessInfo::ERR:
            std::cout << "ERROR\n";
            break;
        case process::ProcessInfo::ERR_CANNOT_WRITE:
            std::cout << "WRITE ERROR\n";
            break;
        case process::ProcessInfo::ERR_EXECUTION_TIME_EXCEEDED:
            std::cout << "EXECUTION TIME EXCEEDED\n";
            break;
        }
        i++;
    }

    return 0;
}

}
