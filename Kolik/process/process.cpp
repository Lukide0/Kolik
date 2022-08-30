#include "process.h"

namespace process
{

void run_program(const char** program, const reproc::options& opts, std::string_view input, std::string_view output,
                 ProcessInfo* info)
{
    reproc::sink::string sink(info->output);

    info->input = input;

    // start process
    reproc::process process;
    std::error_code ec = process.start(program, opts);

    if (ec)
    {
        info->status = ProcessInfo::ERR;
        return;
    }
    // prepare input
    const uint8_t* buff = reinterpret_cast<const uint8_t*>(input.data());

    if (process.write(buff, input.size()).second)
    {
        info->status = ProcessInfo::ERR_CANNOT_WRITE;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();

    // get process output
    ec = reproc::drain(process, sink, reproc::sink::null);
    if (ec)
    {
        info->status = ProcessInfo::ERR_EXECUTION_TIME_EXCEEDED;
        return;
    }

    int status           = 0;
    std::tie(status, ec) = process.wait(reproc::infinite);

    if (ec)
    {
        info->status = ProcessInfo::ERR_EXECUTION_TIME_EXCEEDED;
        return;
    }

    auto end = std::chrono::high_resolution_clock::now();

    // remove \r from output
    info->output.erase(std::remove(info->output.begin(), info->output.end(), '\r'), info->output.end());

    info->time           = ((end - start) / std::chrono::milliseconds(1));
    info->expectedOutput = output;

    if (info->output != output)
        info->status = ProcessInfo::WRONG_OUTPUT;
    else
        info->status = ProcessInfo::CORRECT_OUTPUT;
}

bool test_program(const char* file, Options opts, ProcessResults& result)
{
    reproc::options options;
    reproc::stop_actions stop = {
        {reproc::stop::terminate, reproc::milliseconds(2000)}
    };

    options.deadline = reproc::milliseconds(opts.timeout);
    options.stop     = stop;

    // inputs with their outputs
    std::vector<std::pair<std::string, std::string>> inputsOutputs;

    if (opts.compareFile != "")
    {
        if (!std::filesystem::exists(opts.compareFile))
        {
            std::cout << "ERROR: Invalid --compare-file value";
            return false;
        }
        else if (opts.separator == "")
        {
            std::cout << "ERROR: Empty --separator value";
            return false;
        }

        // read whole file
        std::ifstream file(opts.compareFile.data());
        std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        // input/output separator size
        int separatorSize = opts.separator.size();
        std::size_t pos   = 0;
        std::size_t prev  = 0;

        // file contains multiple inputs
        if (opts.testSeparator != "")
        {
            int sepSize = opts.testSeparator.size();
            std::string inputOutput;

            while ((pos = str.find(opts.testSeparator, prev)) != std::string::npos)
            {
                // get input with output
                inputOutput = str.substr(prev, pos - prev);

                // find separator
                prev = inputOutput.find(opts.separator);
                if (prev == std::string::npos || prev + sepSize > inputOutput.size())
                {
                    inputsOutputs.emplace_back(inputOutput, "");
                    break;
                }
                else
                    inputsOutputs.emplace_back(inputOutput.substr(0, prev), inputOutput.substr(prev + sepSize + 1));

                prev = pos + sepSize + 1;
            }
        }
        // find end of the first input
        pos = str.find(opts.separator, prev);

        // the input has no output
        if (pos == std::string::npos || pos + separatorSize > str.size())
        {
            if (prev > str.size())
            {
                std::cout << "ERROR: Compare file is not valid";
                return false;
            }
            inputsOutputs.emplace_back(str.substr(prev), "");
        }
        // input with output
        else
            inputsOutputs.emplace_back(str.substr(prev, pos - prev), str.substr(pos + separatorSize + 1));
    }

    // add \n character
    for (auto& [input, output] : inputsOutputs)
    {
        if (input.back() != '\n')
            input.push_back('\n');

        if (output.back() != '\n')
            output.push_back('\n');
    }

    const char* argv[3];

    // Language
    switch (opts.lang)
    {
    case Options::PYTHON:
        argv[0] = (char*)"python";
        argv[1] = file;
        argv[2] = nullptr;
        break;
    case Options::EXE:
        argv[0] = file;
        argv[1] = nullptr;
        break;
    }

    std::vector<std::thread> threads;
    result.reserve(inputsOutputs.size());

    for (auto& [in, out] : inputsOutputs)
    {
        result.push_back({});
        ProcessInfo* info = &result.back();

        threads.push_back(std::move((std::thread(run_program, argv, std::cref(options), in, out, info))));
    }

    for (auto& thread : threads)
        thread.join();

    return true;
}
}