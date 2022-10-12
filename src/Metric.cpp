#include "Metric.hpp"

#include <iostream>
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
    for (auto &[host, reqs] : requestsPerHostname)
    {
        std::cout << host << ": " << reqs << std::endl;
    }
    // TODO: Implement
}