#include "Metric.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include "spdlog/spdlog.h"
#include "HttpLayer.h"

Metric::Metric() : totalHttpRequests(0)
{
    popular = std::make_pair("", 0);
}

Metric::~Metric()
{
}

void Metric::consume(pcpp::Packet &pkt)
{
    pcpp::HttpRequestLayer *httpReqLayer = pkt.getLayerOfType<pcpp::HttpRequestLayer>();
    if (httpReqLayer != NULL)
    {
        totalHttpRequests++;

        const std::string host = httpReqLayer->getFieldByName(PCPP_HTTP_HOST_FIELD)->getFieldValue();
        if (++requestsPerHostname[host] > popular.second)
            popular = std::make_pair(host, requestsPerHostname[host]);
    }
}

void Metric::writeToFile(const std::string &path) const
{
    std::ofstream file(path);

    file << "Total HTTP intercepted requests: " << totalHttpRequests << std::endl;
    if (totalHttpRequests > 0)
        file << getHistogram() << std::endl;

    file.close();
}

std::string Metric::getHistogram() const
{
    std::stringstream ss;

    uint8_t calculatedHosts = 0;
    for (auto const& [host, reqCount] : requestsPerHostname)
    {
        spdlog::info("{} {}", host, reqCount);
    }

    return ss.str();
}