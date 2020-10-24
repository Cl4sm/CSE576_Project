int
netAddressMatch(int fd, NetAddressPtr list)
{
    int rc;
    unsigned int len;
    struct sockaddr_in sain;
#ifdef HAVE_IPv6
    struct sockaddr_in6 sain6;
#endif

    len = sizeof(sain);
    rc = getpeername(fd, (struct sockaddr*)&sain, &len);
    if(rc < 0) {
        do_log_error(L_ERROR, errno, "Couldn't get peer name");
        return -1;
    }

    if(sain.sin_family == AF_INET) {
        return match(4, (unsigned char*)&sain.sin_addr, list);
#ifdef HAVE_IPv6
    } else if(sain.sin_family == AF_INET6) {
        len = sizeof(sain6);
        rc = getpeername(fd, (struct sockaddr*)&sain6, &len);
        if(rc < 0) {
            do_log_error(L_ERROR, errno, "Couldn't get peer name");
            return -1;
        }
        if(sain6.sin6_family != AF_INET6) {
            do_log(L_ERROR, "Inconsistent peer name");
            return -1;
        }
        return match(6, (unsigned char*)&sain6.sin6_addr, list);
#endif
    } else {
        do_log(L_ERROR, "Unknown address family %d\n", sain.sin_family);
        return -1;
    }
    return 0;
}