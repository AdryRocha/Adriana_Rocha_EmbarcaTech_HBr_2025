#ifndef LWIPOPTS_H
#define LWIPOPTS_H

// Platform specific locking
#define NO_SYS                          0
#define SYS_LIGHTWEIGHT_PROT            1
#define LWIP_SOCKET                     1
#define LWIP_NETCONN                    1
#define LWIP_NETIF_API                  0

#define LWIP_DNS                        1
#define DNS_TABLE_SIZE                  4
#define DNS_MAX_NAME_LENGTH             256

// Memory options
#define MEM_LIBC_MALLOC                 0
#define MEMP_MEM_MALLOC                 0
#define MEM_ALIGNMENT                   4
#define MEM_SIZE                        (16 * 1024)
#define MEMP_NUM_PBUF                   24
#define MEMP_NUM_UDP_PCB                4
#define MEMP_NUM_TCP_PCB                8
#define MEMP_NUM_TCP_PCB_LISTEN         2
#define MEMP_NUM_TCP_SEG                16
#define MEMP_NUM_REASSDATA              8
#define MEMP_NUM_ARP_QUEUE              4
#define MEMP_NUM_IGMP_GROUP             8
#define MEMP_NUM_SYS_TIMEOUT            8
#define MEMP_NUM_NETBUF                 4
#define MEMP_NUM_NETCONN                4
#define MEMP_NUM_TCPIP_MSG_API          4
#define MEMP_NUM_TCPIP_MSG_INPKT        4
#define PBUF_POOL_SIZE                  16
#define PBUF_POOL_BUFSIZE               1584

// ARP options
#define LWIP_ARP                        1
#define ARP_TABLE_SIZE                  4
#define ARP_QUEUEING                    1

#define LWIP_ETHERNET                   1
#define LWIP_AUTOIP                     0

// IP options
#define IP_FORWARD                      0
#define IP_OPTIONS_ALLOWED              1
#define IP_REASSEMBLY                   1
#define IP_FRAG                         1
#define IP_REASS_MAXAGE                 3
#define IP_REASS_MAX_PBUFS              10
#define IP_FRAG_USES_STATIC_BUF         0
#define IP_FRAG_MAX_MTU                 1500
#define IP_DEFAULT_TTL                  255

// ICMP options
#define LWIP_ICMP                       1
#define ICMP_TTL                        (IP_DEFAULT_TTL)

// DHCP options
#define LWIP_DHCP                       1
#define DHCP_DOES_ARP_CHECK             ((LWIP_DHCP) && (LWIP_ARP))

// UDP options
#define LWIP_UDP                        1
#define LWIP_UDPLITE                    0
#define UDP_TTL                         255

#define LWIP_SNMP                       0
#define LWIP_IGMP                       1

// TCP options
#define LWIP_TCP                        1
#define LWIP_TCP_KEEPALIVE              1
#define TCP_TTL                         255
#define TCP_WND                         (7 * TCP_MSS)
#define TCP_MAXRTX                      12
#define TCP_SYNMAXRTX                   6
#define TCP_QUEUE_OOSEQ                 1
#define TCP_MSS                         1460
#define TCP_CALCULATE_EFF_SEND_MSS      1
#define TCP_SND_BUF                     (4 * TCP_MSS)
#define TCP_SND_QUEUELEN                (4 * (TCP_SND_BUF)/(TCP_MSS))
#define TCP_SNDLOWAT                    ((TCP_SND_BUF)/2)
#define TCP_LISTEN_BACKLOG              0

#define LWIP_TCPIP_CORE_LOCKING         0
#define LWIP_NETCONN_SEM_PER_THREAD     0

// MQTT options
#define LWIP_ALTCP                      1
#define LWIP_ALTCP_TLS                  0

// Threading options
#define TCPIP_THREAD_NAME               "tcpip_thread"
#define TCPIP_THREAD_STACKSIZE          2048
#define TCPIP_THREAD_PRIO               3
#define TCPIP_MBOX_SIZE                 8
#define DEFAULT_THREAD_STACKSIZE        512
#define DEFAULT_THREAD_PRIO             1
#define DEFAULT_UDP_RECVMBOX_SIZE       8
#define DEFAULT_TCP_RECVMBOX_SIZE       8
#define DEFAULT_ACCEPTMBOX_SIZE         8

#define LWIP_COMPAT_SOCKETS             1
#define LWIP_TIMEVAL_PRIVATE            0
#define LWIP_ERRNO_STDINCLUDE           1

#define LWIP_NETIF_STATUS_CALLBACK      1
#define LWIP_NETIF_LINK_CALLBACK        1
#define LWIP_NETIF_HOSTNAME             1

#define SO_REUSE                        1
#define LWIP_SO_RCVTIMEO                1
#define LWIP_SO_SNDTIMEO                1

// Debug options
#define LWIP_DEBUG                      0
#define LWIP_STATS                      0

// Específico para Pico W
#define LWIP_IPV4                       1
#define LWIP_IPV6                       0

#endif /* LWIPOPTS_H */