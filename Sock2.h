
/*
Sock2 is an upgrade from Sock 
It adds has 3 types of lengths/protocols
1. SHORTLEN - netshort prefixs all data
2. LONGLEN - netlong prefixs all data
3. WEBSOCK - Websock protocol prefixes data
and a user HTML connection
4. HTMLSOCK - the user handles this connection htmlttyd does a Single Page App get

Once connectted the data transfer:
put,get and getwait are identical

The client side requests the connection type:

The class provide server side for SHORTLEN, LONGLEN and WEBSOCK
and client side for SHORTLEN and LONGLEN.
A javascript WEBSOCK client side is required for client side WEBSOCK connection.
*/

#ifndef SOCK2_H_INCLUDED
#define SOCK2_H_INCLUDED

#include "gsys.h"

#include <inttypes.h>

#ifdef TGOSWIN32
#include <winsock2.h>
#include <windows.h>
#include <sys/types.h>

#ifdef _WIN64
#undef ETIMEDOUT
#undef ECONNREFUSED
#undef EWOULDBLOCK
#undef EINTR
#endif
#define ETIMEDOUT WSAETIMEDOUT
#define ECONNREFUSED WSAECONNREFUSED
#define EWOULDBLOCK WSAEWOULDBLOCK
#define EINTR WSAEINTR
#define GETERR WSAGetLastError()
#endif

#define NOTDEFINED 0
#define SHORTLEN 1
#define LONGLEN 2
#define WEBSOCK 3
#define HTMLSOCK 4

#define BFLAG 1

class Sock2{

public:
    enum SockRet{CLOSED,NOT_READY,ERR,READY};
    int st;

private:
#ifdef TGOSWIN32
    int cleanup ();
    int startup ();
#endif

    char mask[4];
    int err;
    int length;
    int offset;
    unsigned char *buffer;
    enum {INVALID,READING,SERVING}state;
    uint32_t mlength;

    void DoCHandShake(); //Client side  handshake after accept
    enum SockRet put_len(uint32_t dlen,int bflag);
    enum SockRet get_datax(char *d,uint32_t l);
    enum SockRet get_data(char *rdata, uint32_t rlen);
    enum SockRet get_len(uint32_t *dlen);
    void apply_mask(char *obp,uint32_t len);

public:
    SOCKET fd;
    char hsbuf[2048]; //used by handshake left for caller on HTMLSOCK

    Sock2();
    ~Sock2();

    enum SockRet waitsock(int tim);
    void DoHandShake(); //Server side  handshake after accept
    enum SockRet put_data(char *data,uint32_t dlen); // let client handle html connection
    void open(const char *host,int port);
    void open(const char *host,int port,int t);
    void open(const char *host,int port,int t,uint32_t m);
    void open(int port);
    void open(int port,char *inter);
    void open(int port,unsigned int saddr);
    void open(const Sock2 &s);
    void close();
    enum SockRet get(char **data, uint32_t *len);
    enum SockRet getwait(char **rdata, uint32_t *rlen,int tim);
    enum SockRet put(char *data,uint32_t dlen);
    enum SockRet put(char *data,uint32_t dlen,int bfag);
};
//!gn!
#endif     /* !SOCK_H_INCLUDED */
