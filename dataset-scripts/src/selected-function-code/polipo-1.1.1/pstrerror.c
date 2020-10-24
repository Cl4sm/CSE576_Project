char *
pstrerror(int e)
{
    char *s;
    static char buf[20];

    switch(e) {
    case EDOSHUTDOWN: s = "Immediate shutdown requested"; break;
    case EDOGRACEFUL: s = "Graceful shutdown requested"; break;
    case EDOTIMEOUT: s = "Timeout"; break;
    case ECLIENTRESET: s = "Connection reset by client"; break;
    case ESYNTAX: s = "Incorrect syntax"; break;
    case EREDIRECTOR: s = "Redirector error"; break;
    case EDNS_HOST_NOT_FOUND: s = "Host not found"; break;
    case EDNS_NO_ADDRESS: s = "No address"; break;
    case EDNS_NO_RECOVERY: s = "Permanent name server failure"; break;
    case EDNS_TRY_AGAIN: s = "Temporary name server failure"; break;
    case EDNS_INVALID: s = "Invalid reply from name server"; break;
    case EDNS_UNSUPPORTED: s = "Unsupported DNS reply"; break;
    case EDNS_FORMAT: s = "Invalid DNS query"; break;
    case EDNS_REFUSED: s = "DNS query refused by server"; break;
    case EDNS_CNAME_LOOP: s = "DNS CNAME loop"; break;
#ifndef NO_SOCKS
    case ESOCKS_PROTOCOL: s = "SOCKS protocol error"; break;
    case ESOCKS_REJECT_FAIL: s = "SOCKS request rejected or failed"; break;
    case ESOCKS_REJECT_IDENTD: s = "SOCKS request rejected: "
                                   "server couldn't connect to identd";
        break;
    case ESOCKS_REJECT_UID_MISMATCH: s = "SOCKS request rejected: "
                                         "uid mismatch";
        break;
    case ESOCKS5_BASE: s = "SOCKS success"; break;
    case ESOCKS5_BASE + 1: s = "General SOCKS server failure"; break;
    case ESOCKS5_BASE + 2: s = "SOCKS connection not allowed"; break;
    case ESOCKS5_BASE + 3: s = "SOCKS error: network unreachable"; break;
    case ESOCKS5_BASE + 4: s = "SOCKS error: host unreachable"; break;
    case ESOCKS5_BASE + 5: s = "SOCKS error: connection refused"; break;
    case ESOCKS5_BASE + 6: s = "SOCKS error: TTL expired"; break;
    case ESOCKS5_BASE + 7: s = "SOCKS command not supported"; break;
    case ESOCKS5_BASE + 8: s = "SOCKS error: address type not supported";
        break;
#endif
    case EUNKNOWN: s = "Unknown error"; break;
    default: s = NULL; break;
    }
    if(!s) s = strerror(e);
#ifdef WIN32 /*MINGW*/
    if(!s) {
        if(e >= WSABASEERR && e <= WSABASEERR + 2000) {
            /* This should be okay, as long as the caller discards the
               pointer before another error occurs. */
            snprintf(buf, 20, "Winsock error %d", e);
            s = buf;
        }
    }
#endif
    if(!s) {
        snprintf(buf, 20, "Unknown error %d", e);
        s = buf;
    }
    return s;
}