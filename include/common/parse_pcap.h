#ifndef __PARSE_PACAP__
#define __PARSE_PACAP__
#include <stdio.h>

class ParsePcap{
    public:
    int PcapDecode( const char * filename);
    void PrintLogToFile(bool flag);
    ParsePcap();
     ~ParsePcap();
    private:

    void IpV4Decode(const unsigned char*buf) ;
    void IpV6Decode(const unsigned char *buf) ;
    void TcpDecode(const unsigned char*buf) ;
    void UdpDecode(const unsigned char*buf) ;
    unsigned char  *filebuf_;
    FILE *pcapname_;
    FILE *logname_;
    size_t frame_num_;
    
};
#endif 
