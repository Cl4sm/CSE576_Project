static int
serverSocket(int af)
{
    int fd, rc;
    if(af == 4) {
        fd = socket(PF_INET, SOCK_STREAM, 0);
    } else if(af == 6) {
#ifdef HAVE_IPv6
        fd = socket(PF_INET6, SOCK_STREAM, 0);
#else
        fd = -1;
        errno = EAFNOSUPPORT;
#endif
    } else {
        abort();
    }

    if(fd >= 0) {
        rc = setNonblocking(fd, 1);
        if(rc < 0) {
            int errno_save = errno;
            CLOSE(fd);
            errno = errno_save;
            return -1;
        }
#ifdef HAVE_IPV6_PREFER_TEMPADDR
	if (af == 6 && useTemporarySourceAddress != 1) {
            int value;
            value = (useTemporarySourceAddress == 2) ? 1 : 0;
            rc = setsockopt(fd, IPPROTO_IPV6, IPV6_PREFER_TEMPADDR,
                            &value, sizeof(value));
            if (rc < 0) {
                /* no error, warning only */
                do_log_error(L_WARN, errno, "Couldn't set IPV6CTL_USETEMPADDR");
            }
	}

#endif
    }
    return fd;
}