#ifndef KOLIK_OPTIONS_H
#define KOLIK_OPTIONS_H

#include <nlohmann/json.hpp>
#include <string_view>

struct Options
{
    std::string_view compareFile;

    std::string_view saveFilePath;

    std::string_view testSeparator;

    std::string_view separator = "---";

    bool quiet = false;

    enum SupportedLangs
    {
        PYTHON,
        EXE,
    } lang
        = PYTHON;

    // NOTE: default is 10s
    unsigned int timeout = 10000;
};
// clang-format off
NLOHMANN_JSON_SERIALIZE_ENUM(Options::SupportedLangs, {
    { Options::PYTHON, "Python" },
    { Options::EXE, "Executable" }
})
// clang-format on

#endif // KOLIK_OPTIONS_H