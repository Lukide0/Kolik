#include "save.h"

namespace save
{

bool save_results(const process::ProcessResults& results, const Options& opts)
{
    using nlohmann::json;

    json j;

    j["version"] = "1.0";
    j["info"]    = {
        {  "compare_file",   opts.compareFile},
        {     "separator",     opts.separator},
        {"test_separator", opts.testSeparator},
        {       "timeout",       opts.timeout},
        {          "lang",          opts.lang}
    };

    json array = json::array();

    for (const auto& result : results)
    {
        json res;
        res["time"]            = result.time;
        res["status"]          = result.status;
        res["input"]           = result.input;
        res["output"]          = result.output;
        res["expected_output"] = result.expectedOutput;

        array.push_back(std::move(res));
    }

    j["tests"] = std::move(array);

    std::ofstream file(opts.saveFilePath.data(), std::ios::out);

    file << std::setw(4) << j << std::endl;
    file.close();

    return true;
}

}