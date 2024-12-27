#include <iostream>
#include <pcap.h>
#include <winsock2.h>
#include <string>
#include <map>
#include <vector>
#include <iomanip>

#define ETHERTYPE_IP 0x0800
#define ETHERTYPE_ARP 0x0806
#define ETHERTYPE_IPV6 0x86DD
#define DNS_PORT 53
#define HTTP_PORT 80

using namespace std;

// Function Prototypes
void listInterfaces();
void capturePackets(const string& interfaceName);
void packetHandler(u_char* userData, const pcap_pkthdr* pkthdr, const u_char* packet);
void parseHttpRequest(const u_char* packet, int size);
void parseDnsRequest(const u_char* packet, int size);
void printMacAddress(const u_char* mac);

int main() {
    cout << "SELECT DRIVER:\n";
    listInterfaces();

    cout << "\nEnter interface name to capture on: ";
    string interfaceName;
    getline(cin, interfaceName);

    capturePackets(interfaceName);

    return 0;
}

// Function to list available network interfaces
void listInterfaces() {
    pcap_if_t* alldevs;
    pcap_if_t* d;
    char errbuf[PCAP_ERRBUF_SIZE];

    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        cerr << "Error finding devices: " << errbuf << endl;
        return;
    }

    int i = 0;
    for (d = alldevs; d; d = d->next) {
        cout << ++i << ": " << (d->description ? d->description : "No description") << endl;
    }

    pcap_freealldevs(alldevs);
}

// Function to start packet capture
void capturePackets(const string& interfaceName) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_live(interfaceName.c_str(), BUFSIZ, 1, 1000, errbuf);

    if (!handle) {
        cerr << "Error opening device: " << errbuf << endl;
        return;
    }

    cout << "Identifying MAC ADDRESSES...\n";
    pcap_loop(handle, 0, packetHandler, NULL);
    pcap_close(handle);
}

// Callback function to handle each packet
void packetHandler(u_char* userData, const pcap_pkthdr* pkthdr, const u_char* packet) {
    // Parse the Ethernet header
    struct ether_header* ethernetHeader = (struct ether_header*)packet;

    // Print the source and destination MAC addresses
    cout << "\nSource MAC: ";
    printMacAddress(ethernetHeader->ether_shost);

    cout << "Destination MAC: ";
    printMacAddress(ethernetHeader->ether_dhost);

    // Check the EtherType for IP or ARP packets
    uint16_t etherType = ntohs(ethernetHeader->ether_type);
    
    if (etherType == ETHERTYPE_IP) {
        // IP packet
        parseIpPacket(packet + sizeof(struct ether_header), pkthdr->len - sizeof(struct ether_header));
    }
    else if (etherType == ETHERTYPE_ARP) {
        // ARP packet
        cout << "ARP packet detected." << endl;
    }
    else {
        // Other types of packets
        cout << "Non-IP Packet (EtherType: " << hex << etherType << dec << ")" << endl;
    }
}

// Parse IP packet to detect HTTP and DNS
void parseIpPacket(const u_char* packet, int size) {
    struct ip* ipHeader = (struct ip*)packet;

    // Check for DNS or HTTP
    uint16_t protocol = ipHeader->ip_p;
    if (protocol == IPPROTO_TCP) {
        // HTTP over TCP
        uint16_t destPort = ntohs(((struct tcphdr*)(packet + sizeof(struct ip)))->th_dport);
        if (destPort == HTTP_PORT) {
            parseHttpRequest(packet, size);
        }
    }
    else if (protocol == IPPROTO_UDP) {
        // DNS over UDP
        uint16_t destPort = ntohs(((struct udphdr*)(packet + sizeof(struct ip)))->uh_dport);
        if (destPort == DNS_PORT) {
            parseDnsRequest(packet, size);
        }
    }
}

// Function to parse HTTP requests and print potential passwords
void parseHttpRequest(const u_char* packet, int size) {
    cout << "HTTP Request Detected!" << endl;
    // Look for potential HTTP request payload (basic parsing)
    for (int i = 0; i < size; ++i) {
        if (packet[i] == 0x50 && packet[i+1] == 0x4f && packet[i+2] == 0x53 && packet[i+3] == 0x54) {
            // Found "POST" in the HTTP request (very basic check)
            cout << "Potential HTTP POST request found. Further parsing needed.\n";
            break;
        }
    }
}

// Function to parse DNS requests
void parseDnsRequest(const u_char* packet, int size) {
    cout << "DNS Request Detected!" << endl;
    // Example: Extracting DNS query details
    struct udphdr* udpHeader = (struct udphdr*)(packet + sizeof(struct ip));
    u_char* dnsPayload = (u_char*)(udpHeader + 1);

    // DNS query starts after the header
    cout << "DNS Query: ";
    for (int i = 12; i < size; ++i) {
        if (dnsPayload[i] == 0) break;
        cout << (char)dnsPayload[i];
    }
    cout << endl;
}

// Helper function to print MAC addresses in readable format
void printMacAddress(const u_char* mac) {
    for (int i = 0; i < 6; ++i) {
        printf("%02x", mac[i]);
        if (i < 5) printf(":");
    }
    cout << endl;
}
