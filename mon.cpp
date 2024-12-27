#include <pcap.h>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstring>

void listNetworkAdapters() {
    pcap_if_t* alldevs;
    pcap_if_t* dev;
    char errbuf[PCAP_ERRBUF_SIZE];

    // Get the list of available adapters
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        std::cerr << "Error finding devices: " << errbuf << std::endl;
        return;
    }

    std::cout << "Available network adapters:" << std::endl;
    int index = 0;
    for (dev = alldevs; dev != nullptr; dev = dev->next) {
        std::cout << "[" << index++ << "] " << dev->name;
        if (dev->description) {
            std::cout << " - " << dev->description;
        }
        std::cout << std::endl;
    }

    pcap_freealldevs(alldevs);
}

pcap_t* selectAndOpenAdapter() {
    pcap_if_t* alldevs;
    pcap_if_t* dev;
    char errbuf[PCAP_ERRBUF_SIZE];

    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        std::cerr << "Error finding devices: " << errbuf << std::endl;
        return nullptr;
    }

    listNetworkAdapters();
    std::cout << "Select an adapter by number: ";
    int choice;
    std::cin >> choice;

    int index = 0;
    for (dev = alldevs; dev != nullptr; dev = dev->next) {
        if (index == choice) {
            pcap_t* handle = pcap_open_live(dev->name, 65536, PCAP_OPENFLAG_PROMISCUOUS, 1000, errbuf);
            if (!handle) {
                std::cerr << "Error opening adapter: " << errbuf << std::endl;
                return nullptr;
            }
            std::cout << "Opened adapter: " << dev->name << std::endl;
            return handle;
        }
        ++index;
    }

    std::cerr << "Invalid selection." << std::endl;
    return nullptr;
}

void capturePackets(pcap_t* handle) {
    struct pcap_pkthdr* header;
    const u_char* pkt_data;
    int res;

    std::cout << "Capturing packets... Press Ctrl+C to stop." << std::endl;
    while ((res = pcap_next_ex(handle, &header, &pkt_data)) >= 0) {
        if (res == 0) {
            continue; // Timeout
        }

        // Example: Extract source and destination MAC addresses
        std::cout << "Packet captured:" << std::endl;
        std::cout << "Src MAC: ";
        for (int i = 0; i < 6; ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)pkt_data[i];
            if (i < 5) std::cout << ":";
        }
        std::cout << std::endl;

        std::cout << "Dst MAC: ";
        for (int i = 6; i < 12; ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)pkt_data[i];
            if (i < 11) std::cout << ":";
        }
        std::cout << std::endl;

        // Optional: Parse HTTP or DNS packets (requires deeper packet parsing)
        // For DNS: Check for UDP port 53
        // For HTTP: Check for TCP port 80
    }

    if (res == -1) {
        std::cerr << "Error reading the packets: " << pcap_geterr(handle) << std::endl;
    }
}

int main() {
    pcap_t* handle = selectAndOpenAdapter();
    if (!handle) {
        return 1;
    }

    capturePackets(handle);

    pcap_close(handle);
    return 0;
}
