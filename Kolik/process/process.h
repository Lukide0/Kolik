#ifndef KOLIK_PROCESS_PROCESS_H
#define KOLIK_PROCESS_PROCESS_H

#include "Options.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <reproc++/drain.hpp>
#include <reproc++/reproc.hpp>
#include <string>
#include <thread>

namespace process
{

struct ProcessInfo
{
    long long time;
    std::string input;
    std::string output;
    std::string expectedOutput;

    enum ProcessStatus
    {
        CORRECT_OUTPUT              = 1 << 0,
        WRONG_OUTPUT                = 1 << 1,
        ERR                         = 1 << 2,
        ERR_CANNOT_WRITE            = 1 << 3,
        ERR_EXECUTION_TIME_EXCEEDED = 1 << 4,

    } status;
};
// clang-format off
NLOHMANN_JSON_SERIALIZE_ENUM(ProcessInfo::ProcessStatus, {
    { ProcessInfo::CORRECT_OUTPUT, "Correct" },
    { ProcessInfo::WRONG_OUTPUT, "Fail" },
    { ProcessInfo::ERR, "Unknown" },
    { ProcessInfo::ERR_CANNOT_WRITE, "Error" },
    { ProcessInfo::ERR_EXECUTION_TIME_EXCEEDED, "timeout" },
})
// clang-format on

using ProcessResults = std::vector<ProcessInfo>;

bool test_program(const char* program, Options opts, ProcessResults& result);

}

#endif // KOLIK_PROCESS_PROCESS_H