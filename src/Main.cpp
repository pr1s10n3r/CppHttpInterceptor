#include <cstdlib>
#include <string>
#include <memory>

#include "spdlog/spdlog.h"
#include "HttpRecorder.hpp"

int main(int argc, char *argv[])
{
    // TODO: Parse this value from command arguments
    const std::string networkAdapter = "en0";
    // TODO: Parse this value from command arguments
    const uint32_t lapse = 10;
    // TODO: Parse this valeu from command arguments
    const std::string outputFilePath = "output.log";

    std::unique_ptr<HttpRecorder> httpRecorder;
    try
    {
        httpRecorder = std::make_unique<HttpRecorder>(networkAdapter, lapse);
    }
    catch (std::exception &e)
    {
        spdlog::error("unable to initialize http recorder: {}", e.what());
        return EXIT_FAILURE;
    }

    httpRecorder->start();
    httpRecorder->writeToFile(outputFilePath);

    return EXIT_SUCCESS;
}
