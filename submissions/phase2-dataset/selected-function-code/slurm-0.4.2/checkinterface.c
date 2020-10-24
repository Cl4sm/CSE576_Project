int checkinterface()
{
    int validinterface = FALSE;
    int sockfd, i, numifs, numifreqs;
    size_t bufsize;
    char *buf;
    struct ifreq ifr, *ifrp;
    struct ifconf ifc;

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {   
        perror("socket");
        return FALSE;
    }
    if (ioctl(sockfd, SIOCGIFNUM, (char *) &numifs) < 0)
    {   
        perror("SIOCGIFNUM");
        close(sockfd);
        return FALSE;
    }
    bufsize = ((size_t) numifs) * sizeof(struct ifreq);
    buf = (char *) malloc(bufsize);
    if (!buf)
    {   
        perror("malloc");
        close(sockfd);
        return FALSE;
    }

    ifc.ifc_len = bufsize;
    ifc.ifc_buf = buf;

    if (ioctl(sockfd, SIOCGIFCONF, (char *) &ifc) < 0)
    {   
        perror("SIOCGIFCONF");
        close(sockfd);
        free(buf);
        return FALSE;
    }

    ifrp = ifc.ifc_req;
    numifreqs = ifc.ifc_len / sizeof(struct ifreq);

    for (i = 0; i < numifreqs; i++, ifrp++)
    {
        memset((char *)&ifr, 0, sizeof(ifr));
        strncpy(ifr.ifr_name, ifrp->ifr_name, sizeof(ifr.ifr_name));
        /* do not check for loopback device as it cannot be monitored */
        if (!strncmp(ifr.ifr_name, "lo", 2))
            continue;
        if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) < 0)
        {   
            perror("SIOCGIFFLAGS");
            continue;
        }
        if (!strcmp(ifdata.if_name, ifr.ifr_name) && (ifr.ifr_flags & IFF_UP))
        {
            validinterface = TRUE;
            break;
        }
    }
    free(buf);
    close(sockfd);

    return validinterface;
}
