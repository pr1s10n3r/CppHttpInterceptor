#include "Metric.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <functional>

#include "spdlog/spdlog.h"
#include "HttpLayer.h"
#include "tabulate/table.hpp"

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

template <typename K, typename V>
std::multimap<V, K, std::greater<V>> toMultiMap(const std::map<K, V> &map)
{
    std::multimap<V, K, std::greater<V>> result;

    for (auto const &[host, reqCount] : map)
        result.insert(std::make_pair(reqCount, host));

    return result;
}

std::string Metric::getHistogramBar(uint32_t reqCount) const
{
    // 20 because at max we want to print 20 asterisks
    const uint32_t charsToPrint = (20 * reqCount) / popular.second;

    std::stringstream ss;
    for (uint32_t i = 0; i < charsToPrint; i++)
        ss << "*";

    return ss.str();
}

/**
 * Complexity Analysis
 * O(n log n)
 *
 * This is by far the most complex function in this program. This is because in order to generate a histogram,
 * some work has to be done:
 *     1. Sort the `requestsPerHostname` map.
 *        This is done using the std::multimap built-in structure. In this step, we need to sort the whole map in order
 *        to detect most consumed host. The complexity of this algorithm is O(n log n).
 *     2. The second step is build the histogram, for this the Tabulate library is used; behind the scenes this library
 *        makes a lot of work, so we will asume the complexity of table building is O(n).
 */
std::string Metric::getHistogram() const
{
    const std::multimap<uint32_t, std::string, std::greater<uint32_t>> sorted = toMultiMap(requestsPerHostname);

    tabulate::Table table;
    table.add_row({"Host", "Requests", "Histogram"});

    uint8_t hosts = 0;
    for (auto const &[reqCount, host] : sorted)
    {
        table.add_row({host, std::to_string(reqCount), getHistogramBar(reqCount)});
        if (++hosts > 10)
            break;
    }

    return table.str();
}