#include <pcap.h>
#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <cstring>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

// Helper function to print MAC address
void printMAC(const u_char* mac) {
    for (int i = 0; i < 6; i++) {
        printf("%02X", mac[i]);
        if (i < 5) printf(":");
    }
}

// Helper function to extract and display HTTP data
void parseHTTP(const u_char* payload, int payloadLen) {
    std::string httpData(reinterpret_cast<const char*>(payload), payloadLen);

    // Display the full HTTP request for analysis
    std::cout << "----- HTTP Request -----" << std::endl;
    std::cout << httpData << std::endl;
    std::cout << "-------------------------" << std::endl;

    // Check for sensitive fields (basic example)
    size_t pos = httpData.find("password=");
    if (pos != std::string::npos) {
        std::cout << "Sensitive Data Found: ";
        std::cout << httpData.substr(pos, httpData.find('&', pos) - pos) << std::endl;
    }
}

// Helper function to extract and display DNS queries
void parseDNS(const u_char* payload, int payloadLen) {
    if (payloadLen < 12) return; // Minimum DNS header size
    std::cout << "----- DNS Request -----" << std::endl;

    const u_char* dnsQuery = payload + 12; // Skip the DNS header
    while (*dnsQuery != 0 && dnsQuery < payload + payloadLen) {
        std::cout << (isprint(*dnsQuery) ? *dnsQuery : '.');
        dnsQuery++;
    }
    std::cout << std::endl;
    std::cout << "-------------------------" << std::endl;
}

// Capture and process packets
void capturePackets(pcap_t* handle) {
    struct pcap_pkthdr* header;
    const u_char* packet;
    int res;
    int stationNum = 0;

    std::map<std::string, std::string> stations; // MAC -> ESSID

    std::cout << std::setw(10) << "STATION" << std::setw(20) << "MAC ADDRESS" 
              << std::setw(30) << "ESSID" << std::endl;

    while ((res = pcap_next_ex(handle, &header, &packet)) >= 0) {
        if (res == 0) continue; // Timeout
        if (header->len < 14) continue; // Ignore packets too small for Ethernet header

        const u_char* srcMAC = packet + 6; // Source MAC
        const u_char* ethType = packet + 12; // Ethernet Type (bytes 12-13)

        // Print unique MAC addresses as stations
        std::string macStr = std::string((char*)srcMAC, 6);
        if (stations.find(macStr) == stations.end()) {
            stationNum++;
            stations[macStr] = "Unknown"; // Placeholder ESSID

            std::cout << std::setw(10) << stationNum;
            std::cout << std::setw(20);
            printMAC(srcMAC);
            std::cout << std::setw(30) << stations[macStr] << std::endl;
        }

        // Check for IPv4 packets
        if (ntohs(*(uint16_t*)ethType) == 0x0800) {
            const u_char* ipHeader = packet + 14; // IP header starts after Ethernet header
            int ipHeaderLen = (ipHeader[0] & 0x0F) * 4;
            u_char protocol = ipHeader[9];       // Protocol field in IP header

            if (protocol == 6) { // TCP Protocol
                const u_char* tcpHeader = ipHeader + ipHeaderLen;
                int tcpHeaderLen = ((tcpHeader[12] & 0xF0) >> 4) * 4;
                uint16_t srcPort = ntohs(*(uint16_t*)tcpHeader);
                uint16_t destPort = ntohs(*(uint16_t*)(tcpHeader + 2));

                const u_char* payload = tcpHeader + tcpHeaderLen;
                int payloadLen = header->len - (14 + ipHeaderLen + tcpHeaderLen);

                if (payloadLen > 0) {
                    // HTTP Parsing: Check for HTTP traffic on port 80
                    if (srcPort == 80 || destPort == 80) {
                        parseHTTP(payload, payloadLen);
                    }
                }
            } else if (protocol == 17) { // UDP Protocol
                const u_char* udpHeader = ipHeader + ipHeaderLen;
                uint16_t srcPort = ntohs(*(uint16_t*)udpHeader);
                uint16_t destPort = ntohs(*(uint16_t*)(udpHeader + 2));

                const u_char* payload = udpHeader + 8;
                int payloadLen = header->len - (14 + ipHeaderLen + 8);

                if (payloadLen > 0) {
                    // DNS Parsing: Check for DNS traffic on port 53
                    if (srcPort == 53 || destPort == 53) {
                        parseDNS(payload, payloadLen);
                    }
                }
            }
        }
    }

    if (res == -1) {
        std::cerr << "Error reading packets: " << pcap_geterr(handle) << std::endl;
    }
}

int main() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t* alldevs, *d;

    // Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        std::cerr << "Error finding devices: " << errbuf << std::endl;
        return 1;
    }

    // List all devices
    std::cout << "Available Network Adapters:" << std::endl;
    int i = 0;
    for (d = alldevs; d != nullptr; d = d->next) {
        std::cout << ++i << ": " << (d->description ? d->description : "No description available") << std::endl;
    }

    // Select device
    int choice;
    std::cout << "Select an Adapter by Number: ";
    std::cin >> choice;

    pcap_if_t* selectedDevice = nullptr;
    i = 0;
    for (d = alldevs; d != nullptr; d = d->next) {
        if (++i == choice) {
            selectedDevice = d;
            break;
        }
    }

    if (!selectedDevice) {
        std::cerr << "Invalid selection." << std::endl;
        pcap_freealldevs(alldevs);
        return 1;
    }

    // Open the device
    pcap_t* handle = pcap_open_live(selectedDevice->name, 65536, 1, 1000, errbuf);
    if (!handle) {
        std::cerr << "Error opening device: " << errbuf << std::endl;
        pcap_freealldevs(alldevs);
        return 1;
    }

    pcap_freealldevs(alldevs);
    capturePackets(handle);
    pcap_close(handle);

    return 0;
}
