    char *host, int hostlen, char *serv, int servlen, int flags) {

#if defined(USE_WIN32) && !defined(_WIN32_WCE)
    if(s_getnameinfo)
        return s_getnameinfo(sa, salen, host, hostlen, serv, servlen, flags);
#endif
    if(host && hostlen) {
#if defined(USE_IPv6) && !defined(USE_WIN32)
        inet_ntop(sa->sa_family, sa->sa_family==AF_INET6 ?
                (void *)&((struct sockaddr_in6 *)sa)->sin6_addr :
                (void *)&((struct sockaddr_in *)sa)->sin_addr,
            host, hostlen);
#else /* USE_IPv6 */
        enter_critical_section(CRIT_INET); /* inet_ntoa is not mt-safe */
        strncpy(host, inet_ntoa(((struct sockaddr_in *)sa)->sin_addr),
            hostlen);
        leave_critical_section(CRIT_INET);
        host[hostlen-1]='\0';
#endif /* USE_IPv6 */
    }
    if(serv && servlen)
        sprintf(serv, "%u", ntohs(((struct sockaddr_in *)sa)->sin_port));
    /* sin_port is in the same place both in sockaddr_in and sockaddr_in6 */
    /* ignore servlen since it's long enough in stunnel code */
    return 0;
}
