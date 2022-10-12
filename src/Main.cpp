#include <cstdlib>
#include <string>
#include <memory>

#include "tclap/CmdLine.h"
#include "spdlog/spdlog.h"
#include "HttpRecorder.hpp"

int main(int argc, char *argv[])
{
    TCLAP::CmdLine cmd("small C++ command line tool for HTTP interception", ' ', "0.0.1");
    TCLAP::ValueArg<std::string> inetArg("i", "interface", "network interface to intercept", true, "", "string");
    TCLAP::ValueArg<uint32_t> lapseArg("l", "lapse", "duration in seconds interception will last", true, 0, "non-negative integer");
    TCLAP::ValueArg<std::string> outFileArg("o", "output", "output filepath where results will be written", true, "", "string");

    cmd.add(inetArg);
    cmd.add(lapseArg);
    cmd.add(outFileArg);
    cmd.parse(argc, argv);

    const std::string networkAdapter = inetArg.getValue();
    const uint32_t lapse = lapseArg.getValue();
    const std::string outputFilePath = outFileArg.getValue();

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
