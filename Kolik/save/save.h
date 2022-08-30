#ifndef KOLIK_SAVE_SAVE_H
#define KOLIK_SAVE_SAVE_H

#include "Options.h"
#include "process/process.h"
#include <nlohmann/json.hpp>

namespace save
{

bool save_results(const process::ProcessResults& results, const Options& opts);

}

#endif // KOLIK_SAVE_SAVE_H