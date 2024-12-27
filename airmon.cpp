#include <pcap.h>
#include <iostream>
#include <vector>
#include <string>

void listInterfaces() {
    pcap_if_t *interfaces, *temp;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Find all available interfaces
    if (pcap_findalldevs(&interfaces, errbuf) == -1) {
        std::cerr << "Error finding devices: " << errbuf << std::endl;
        return;
    }

    int index = 0;
    temp = interfaces;
    std::cout << "SELECT DRIVER: \n";

    // Print all available interfaces
    while (temp) {
        std::cout << index++ << ": " << temp->name << std::endl;
        temp = temp->next;
    }
}

bool setMonitorMode(pcap_t *handle) {
    // Try to set monitor mode (specific to the driver/hardware)
    // This may not work on all interfaces due to lack of support.
    if (pcap_set_promisc(handle, 1) == -1) {
        std::cerr << "Error setting monitor mode: " << pcap_geterr(handle) << std::endl;
        return false;
    }
    std::cout << "Successfully set to monitor mode (or promiscuous mode)." << std::endl;
    return true;
}

int main() {
    pcap_if_t *interfaces, *temp;
    char errbuf[PCAP_ERRBUF_SIZE];
    
    // Find all available interfaces
    if (pcap_findalldevs(&interfaces, errbuf) == -1) {
        std::cerr << "Error finding devices: " << errbuf << std::endl;
        return 1;
    }

    listInterfaces();

    int selectedInterfaceIndex;
    std::cout << "Enter the interface index to select: ";
    std::cin >> selectedInterfaceIndex;

    temp = interfaces;
    for (int i = 0; i < selectedInterfaceIndex; ++i) {
        temp = temp->next;
    }

    if (!temp) {
        std::cerr << "Invalid interface selection." << std::endl;
        return 1;
    }

    // Open the selected interface
    pcap_t *handle = pcap_open_live(temp->name, BUFSIZ, 1, 1000, errbuf);
    if (!handle) {
        std::cerr << "Error opening device: " << errbuf << std::endl;
        return 1;
    }

    // Attempt to set the interface into monitor mode
    if (!setMonitorMode(handle)) {
        std::cout << "Attempting to set promiscuous mode instead..." << std::endl;
        pcap_set_promisc(handle, 1);
    }

    // Cleanup
    pcap_close(handle);
    pcap_freealldevs(interfaces);

    return 0;
}
