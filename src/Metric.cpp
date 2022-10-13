#include "Metric.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include "HttpLayer.h"

Metric::Metric() : totalHttpRequests(0)
{
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
        requestsPerHostname[host]++;
    }
}

void Metric::writeToFile(const std::string &path) const
{
    std::ofstream file(path);

    file << "Total HTTP intercepted requests: " << totalHttpRequests << std::endl;
    file << getHistogram() << std::endl;

    file.close();
}

std::string Metric::getHistogram() const
{
    // Create and populate vector from map.
    std::vector<std::pair<std::string, uint32_t>> vectorizedMap;
    std::copy(
        requestsPerHostname.begin(),
        requestsPerHostname.end(),
        std::back_inserter<std::vector<std::pair<std::string, uint32_t>>>(vectorizedMap));

    // Sort the vector using the second value (amount of requests).
    auto sortFunc = [](const std::pair<std::string, uint32_t> &left, const std::pair<std::string, uint32_t> &right)
    {
        if (left.second != right.second)
            return left.second < right.second;
        return left.first < right.first;
    };
    std::sort(vectorizedMap.begin(), vectorizedMap.end(), sortFunc);

    std::stringstream ss;

    return ss.str();
}