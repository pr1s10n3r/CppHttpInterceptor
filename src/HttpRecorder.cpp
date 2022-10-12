#include "HttpRecorder.hpp"

#include <iostream>
#include <fstream>
#include <exception>
#include "spdlog/spdlog.h" // for logging
#include "SystemUtils.h"   // for pcpp::multiPlatformSleep

static void onPacketArrives(pcpp::RawPacket *packet, pcpp::PcapLiveDevice *dev, void *cookie);

HttpRecorder::HttpRecorder(const std::string &networkInterfaceName, const uint32_t lapseInSeconds) : m_lapseInSeconds(lapseInSeconds)
{
    device = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName(networkInterfaceName);
    if (device == NULL)
    {
        spdlog::error("unable to find network interface \"{}\"", networkInterfaceName);
        throw std::runtime_error("unable to find network interface");
    }

    if (!device->open())
    {
        spdlog::error("unable to open network device. You may run this program as root");
        throw std::runtime_error("unable to open network device");
    }
}

HttpRecorder::~HttpRecorder()
{
    device->stopCapture();
    device->close();
}

void HttpRecorder::start()
{
    pcpp::ProtoFilter httpProtoFilter(pcpp::HTTP);
    device->setFilter(httpProtoFilter);

    device->startCapture(onPacketArrives, &metric);
    pcpp::multiPlatformSleep(m_lapseInSeconds);
}

static void onPacketArrives(pcpp::RawPacket *packet, pcpp::PcapLiveDevice *dev, void *cookie)
{
    pcpp::Packet parsedPacket(packet);
    Metric *metric = (Metric *)cookie;
    metric->consume(parsedPacket);
}