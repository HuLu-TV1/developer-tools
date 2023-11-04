#include "parse_pcap.h"
#include <chrono>
#include <ctime>
#include <malloc.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct PcapHeader
{
  uint32_t magic_number;  // 魔数，标识文件格式
  uint16_t version_major; // 主版本号
  uint16_t version_minor; // 次版本号
  int32_t thiszone;       // 时区修正，一般为0
  uint32_t sigfigs;       // 时间戳精度，一般为0
  uint32_t snaplen;       // 最大抓取长度
  uint32_t network;       // 数据链路类型
};

struct PcapPacketHeader
{
  uint32_t timestamp_sec;  // 时间戳（秒）
  uint32_t timestamp_usec; // 时间戳（微秒）
  uint32_t incl_len;       // 数据包实际长度（抓取到的长度）
  uint32_t orig_len;       // 数据包原始长度
};

struct EthernetHeader
{
  uint8_t destination[6]; // 目标MAC地址
  uint8_t source[6];      // 源MAC地址
  uint16_t type;          // 上层协议类型
};

struct IPv4Header
{
  uint8_t version_ihl;     // 版本号和头部长度
  uint8_t dscp_ecn;        // 服务类型字段
  uint16_t total_length;   // 总长度
  uint16_t identification; // 标识
  uint16_t flags_fragment; // 标志和分段偏移
  uint8_t ttl;             // 存活时间
  uint8_t protocol;        // 上层协议
  uint16_t checksum;       // 校验和
  uint32_t source_ip;      // 源IP地址
  uint32_t dest_ip;        // 目标IP地址
};

struct IPv6Header
{
  uint32_t version_class_flow; // 版本、Traffic Class和Flow Label
  uint16_t payload_length;     // 载荷长度
  uint8_t next_header;         // 下一个首部
  uint8_t hop_limit;           // 跳数限制
  uint8_t source_ip[16];       // 源IPv6地址
  uint8_t dest_ip[16];         // 目标IPv6地址
};

struct UDPHeader
{
  uint16_t sourcePort;      // 源端口号
  uint16_t destinationPort; // 目标端口号
  uint16_t length;          // UDP数据包长度
  uint16_t checksum;        // 校验和
};

struct TCPHeader
{
  uint16_t sourcePort;      // 源端口号
  uint16_t destinationPort; // 目标端口号
  uint32_t sequenceNumber;  // 序列号
  uint32_t ackNumber;       // 确认号
  uint8_t dataOffset;       // 数据偏移
  uint8_t flags;            // 标志位
  uint16_t windowSize;      // 窗口大小
  uint16_t checksum;        // 校验和
  uint16_t urgentPointer;   // 紧急指针
                            // 可选字段和数据...
};

ParsePcap::ParsePcap()
    : filebuf_(nullptr), pcapname_(nullptr), logname_(nullptr), frame_num_(0) {}
ParsePcap::~ParsePcap()
{
  fclose(logname_);
  fclose(pcapname_);
  free(filebuf_);
  filebuf_ = nullptr;
}
void ParsePcap::PrintLogToFile(bool flag)
{
  logname_ = fopen("ts.log", "wb");
  if (logname_ == NULL)
  {
    perror("Error opening file");
  }
}
int ParsePcap::PcapDecode(const char *filename)
{
  pcapname_ = fopen(filename, "rb");

  if (pcapname_ == NULL)
  {
    perror("Error opening pcapname_");
    return -1;
  }

  // 获取文件大小
  fseek(pcapname_, 0, SEEK_END);
  long filesize = ftell(pcapname_);
  fseek(pcapname_, 0, SEEK_SET);

  // 动态分配堆空间
  filebuf_ = (unsigned char *)malloc(filesize);
  printf("file_size =%ld\n", filesize);
  if (filebuf_ == NULL)
  {
    perror("Error allocating memory");
    fclose(pcapname_);
    return -1;
  }

  // 读取文件内容到堆空间中
  size_t itemsread = fread(filebuf_, 1, filesize, pcapname_);
  // 解析 Global Head
  PcapHeader *pcaphead = (PcapHeader *)filebuf_;
  fprintf(logname_, "magic_number = %0x,version_major = %0x,version_minor=%0x\n",
          pcaphead->magic_number, pcaphead->version_major,
          pcaphead->version_minor);
  size_t offset = sizeof(PcapHeader);

  while (offset < filesize)
  {
    frame_num_++;
    fprintf(logname_, "########current frame num  = %ld########\n\n", frame_num_);
    fprintf(logname_, "########Packet Header########\n");
    // 解析Packet Header (16 Bytes)
    PcapPacketHeader *pphead = (PcapPacketHeader *)(filebuf_ + offset);
    time_t t = pphead->timestamp_sec;
    struct tm *tminfo;
    tminfo = localtime(&t);
    if (tminfo != NULL)
    {
      fprintf(logname_, "Readable time: %04d-%02d-%02d %02d:%02d:%02d\n",
              tminfo->tm_year + 1900, tminfo->tm_mon + 1, tminfo->tm_mday,
              tminfo->tm_hour, tminfo->tm_min, tminfo->tm_sec);
    }
    else
    {
      fprintf(logname_, "Error converting timestamp.\n");
    }

    int protoffset = offset + sizeof(PcapPacketHeader);
    // 解析Ethernet Header (14 Bytes)
    fprintf(logname_, "########Ethernet Header########\n");
    EthernetHeader *ethernethead = (EthernetHeader *)(filebuf_ + protoffset);

    fprintf(logname_, "source->destination:[%02x:%02x:%02x:%02x:%02x:%02x]->[%02x:%02x:%02x:%02x:%02x:%02x] \n ",
            ethernethead->source[0], ethernethead->source[1], ethernethead->source[2], ethernethead->source[3], ethernethead->source[4], ethernethead->source[5],
            ethernethead->destination[0], ethernethead->destination[1], ethernethead->destination[2], ethernethead->destination[3], ethernethead->destination[4], ethernethead->destination[5]);
    fprintf(logname_, "type = %0x\n", ethernethead->type);
    uint16_t protocol = ntohs(ethernethead->type);
    switch (protocol)
    {
    case 0x0800:
      fprintf(logname_, "IPv4 Protocol\n");
      break;
    case 0x0806:
      fprintf(logname_, "ARP Protocol\n");
      break;
    case 0x8035:
      fprintf(logname_, "RARP Protocol\n");
      break;
    case 0x86DD:
      fprintf(logname_, "IPv6 Protocol\n");
      break;
    case 0x8100:
      fprintf(logname_, "VLAN Tagged Frame (IEEE 802.1Q)\n");
      break;
    // 添加更多的case语句来处理其他类型值
    default:
      fprintf(logname_, "Unknown Protocol\n");
      break;
    }
    // ip协议解析(20 bytes)
    protoffset += sizeof(EthernetHeader);
    if (protocol == 0x0800)
    {
      IpV4Decode(filebuf_ + protoffset);
    }
    else if (protocol == 0x86DD)
    {
      IpV6Decode(filebuf_ + protoffset);
    }
    int pcaplength = pphead->incl_len;
    fprintf(logname_, "pcaplength =%d\n\n", pcaplength);
    offset += sizeof(PcapPacketHeader) + pcaplength;
  }
  return 0;
}

void ParsePcap::IpV4Decode(const unsigned char *buf)
{
  fprintf(logname_, "########IpV4 Header########\n");
  IPv4Header *ipv4head = (IPv4Header *)buf;
  fprintf(logname_, "IP version number = %0X\n", ipv4head->version_ihl >> 4);
  uint8_t headlength = (ipv4head->version_ihl & 0x0f) * 4;
  fprintf(logname_, "IP head length = %d bite\n", headlength);

  uint16_t datalength = ntohs(ipv4head->total_length);
  fprintf(logname_, "IP data length  = %d bite\n",
          datalength);
  uint16_t protocol = ipv4head->protocol;
  fprintf(logname_, "protocol = %0x\n", protocol);
  switch (protocol)
  {
  case 0x17:
    fprintf(logname_, "UDP Protocol\n");
    break;
  case 0x06:
    fprintf(logname_, "TCP Protocol\n");
    break;
  default:
    fprintf(logname_, "Unknown Protocol\n");
    break;
  }
  char sourceaddress[20] = {0};
  char desaddress[20] = {0};
  inet_ntop(AF_INET, &ipv4head->source_ip, sourceaddress,
            sizeof(sourceaddress));

  inet_ntop(AF_INET, &ipv4head->dest_ip, desaddress,
            sizeof(desaddress));
  fprintf(logname_, "source address =%s\n", sourceaddress);
  fprintf(logname_, "des address =%s\n", desaddress);
  if (protocol == 0x17)
  {
    // TcpDecode
  }
  else if (protocol == 0x06)
  {
    TcpDecode(buf + sizeof(IPv4Header), datalength - sizeof(IPv4Header));
  }
}

void ParsePcap::IpV6Decode(const unsigned char *buf)
{
  fprintf(logname_, "########IPV6 Header########\n");
  IPv6Header *ipv6head = (IPv6Header *)buf;

  uint16_t protocol = (ipv6head->version_class_flow >> 12) & 0xFF;
  switch (protocol)
  {
  case 0x17:
    fprintf(logname_, "UDP Protocol\n");
    break;
  case 0x06:
    fprintf(logname_, "TCP Protocol\n");
    break;
  default:
    fprintf(logname_, "Unknown Protocol\n");
    break;
  }
}

void ParsePcap::TcpDecode(const unsigned char *buf, uint16_t datalength)
{
  fprintf(logname_, "########TCP Header########\n");
  TCPHeader *tcpheader = (TCPHeader *)buf;
  size_t tcpheaddata = (tcpheader->dataOffset >> 4) * 4;
  fprintf(logname_, "source port =%d\n", ntohs(tcpheader->sourcePort));
  fprintf(logname_, "des port =%d\n", ntohs(tcpheader->destinationPort));
  fprintf(logname_, "Sequence Number =%u\n", ntohl(tcpheader->sequenceNumber));
  fprintf(logname_, "Acknowledgment Number =%u\n", ntohl(tcpheader->ackNumber));
  fprintf(logname_, "Tcp Head size =%ld\n", tcpheaddata);
  size_t datasize = datalength - tcpheaddata;
  fprintf(logname_, "data size  =%ld\n", datasize);
}

void ParsePcap::UdpDecode(const unsigned char *buf, uint16_t datalength)
{
  fprintf(logname_, "########UDP Header########\n");
  UDPHeader *udpheader = (UDPHeader *)buf;
  fprintf(logname_, "source port =%d\n", ntohs(udpheader->sourcePort));
  fprintf(logname_, "des port =%d\n", ntohs(udpheader->destinationPort));
  size_t udpheaddata = ntohs(udpheader->length);
  fprintf(logname_, "Tcp Head size =%ld\n", udpheaddata);
  size_t datasize = datalength - udpheaddata;
  fprintf(logname_, "data size  =%ld\n", datasize);
}
