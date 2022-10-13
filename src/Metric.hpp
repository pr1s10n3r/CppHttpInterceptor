#pragma once

#include "Packet.h"
#include <map>

class Metric
{
private:
    typedef std::pair<std::string, uint32_t> HostRequests;

    uint32_t totalHttpRequests;
    std::map<std::string, uint32_t> requestsPerHostname;

    // This will help us to build the histogram
    HostRequests popular;

    std::string getHistogram() const;

public:
    Metric();
    virtual ~Metric();

    void consume(pcpp::Packet &pkt);
    void writeToFile(const std::string &path) const;
};