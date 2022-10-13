#pragma once

#include "Packet.h"
#include <map>

class Metric
{
private:
    uint32_t totalHttpRequests;
    std::map<std::string, uint32_t> requestsPerHostname;

    std::string getHistogram() const;

public:
    Metric();
    virtual ~Metric();

    void consume(pcpp::Packet &pkt);
    void writeToFile(const std::string &path) const;
};