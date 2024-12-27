#include <iostream>
#include <pcap.h>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

// Function to list available interfaces
void list_interfaces() {
    pcap_if_t *alldevs;
    pcap_if_t *device;
    char errbuf[PCAP_ERRBUF_SIZE];

    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        std::cerr << "Error finding devices: " << errbuf << std::endl;
        return;
    }

    std::cout << "Available Interfaces:" << std::endl;
    int count = 1;
    for (device = alldevs; device != NULL; device = device->next) {
        std::cout << count++ << ". " << device->name << std::endl;
    }

    pcap_freealldevs(alldevs);
}

// Function to change interface to monitor mode
bool set_monitor_mode(pcap_t *handle) {
    // For Npcap, this is generally done via device-specific mechanisms
    // Changing the mode to monitor might require additional commands, here is a simple approach:
    
    // Monitor mode change (platform-dependent, needs to be implemented or configured)
    // This functionality depends on the device and adapter, not all adapters support it programmatically in C++
    return true;
}

// Packet capture callback
void packet_handler(u_char *user_data, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    // Print packet details (beacon count, BSSID, etc.)
    // Parse the raw packet data and extract required information
    std::cout << "Packet captured: " << pkthdr->len << " bytes" << std::endl;
    // Implement packet parsing logic here
}

int main(int argc, char *argv[]) {
    std::cout << "Select Driver (Interface):" << std::endl;
    list_interfaces();

    int choice;
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    pcap_if_t *alldevs;
    pcap_if_t *device;
    char errbuf[PCAP_ERRBUF_SIZE];

    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        std::cerr << "Error finding devices: " << errbuf << std::endl;
        return 1;
    }

    int count = 1;
    for (device = alldevs; device != NULL; device = device->next) {
        if (count == choice) {
            std::cout << "Selected Interface: " << device->name << std::endl;

            pcap_t *handle = pcap_open_live(device->name, BUFSIZ, 1, 1000, errbuf);
            if (handle == NULL) {
                std::cerr << "Error opening device: " << errbuf << std::endl;
                return 1;
            }

            if (!set_monitor_mode(handle)) {
                std::cout << "Failed to set monitor mode, switching to promiscuous mode..." << std::endl;
                pcap_set_promisc(handle, 1); // Set promiscuous mode if monitor mode fails
            }

            // Start capturing packets
            if (pcap_loop(handle, 0, packet_handler, NULL) < 0) {
                std::cerr << "Error capturing packets: " << pcap_geterr(handle) << std::endl;
                return 1;
            }

            pcap_close(handle);
            break;
        }
        count++;
    }

    pcap_freealldevs(alldevs);
    return 0;
}
