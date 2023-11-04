#include "parse_pcap.h"
#define _WIN32_WINNT_WIN10                  0x0A00 // Windows 10
int main(){
    ParsePcap parsepcap;
    parsepcap.PrintLogToFile(true);
    const char *file_name = "toutiao.pcap";
    parsepcap.PcapDecode(file_name);
    return 0;
}