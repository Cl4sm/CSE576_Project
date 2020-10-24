NetAddressPtr
parseNetAddress(AtomListPtr list)
{
    NetAddressPtr nl;
    int i, rc, rc6;
    char buf[100];
    struct in_addr ina;
#ifdef HAVE_IPv6
    struct in6_addr ina6;
#endif

    nl = malloc((list->length + 1) * sizeof(NetAddressRec));
    if(nl == NULL) {
        do_log(L_ERROR, "Couldn't allocate network list.\n");
        return NULL;
    }

    for(i = 0; i < list->length; i++) {
        int prefix;
        char *s = list->list[i]->string, *p;
        int n = list->list[i]->length;
        char *suffix;

        while(*s == ' ' || *s == '\t') {
            s++;
            n--;
        }

        if(n >= 100) {
            do_log(L_ERROR, "Network name too long.\n");
            goto fail;
        }
        p = memchr(s, '/', n);
        if(p) {
            memcpy(buf, s, p - s);
            buf[p - s] = '\0';
            prefix = strtol(p + 1, &suffix, 10);
        } else {
            char *s1, *s2;
            prefix = -1;
            strcpy(buf, s);
            s1 = strchr(s, ' ');
            s2 = strchr(s, '\t');
            if(s1 == NULL) suffix = s2;
            else if(s2 == NULL) suffix = s1;
            else if(s1 < s2) suffix = s1;
            else suffix = s2;
            if(suffix == NULL)
                suffix = s + n;
        }

        if(!isWhitespace(suffix)) {
            do_log(L_ERROR, "Couldn't parse network %s.\n", buf);
            goto fail;
        }

        rc = 0; rc6 = 0;
        rc = inet_aton(buf, &ina);
#ifdef HAVE_IPv6
        if(rc == 0) {
            rc6 = inet_pton(AF_INET6, buf, &ina6);
        }
#endif
        if(rc == 0 && rc6 == 0) {
            do_log(L_ERROR, "Couldn't parse network %s.\n", buf);
            goto fail;
        }
        nl[i].prefix = prefix;
        if(rc) {
            nl[i].af = 4;
            memcpy(nl[i].data, &ina, 4);
        } else {
#ifdef HAVE_IPv6
            nl[i].af = 6;
            memcpy(nl[i].data, &ina6, 16);
#else
            abort();
#endif
        }
    }
    nl[i].af = 0;
    return nl;

 fail:
    free(nl);
    return NULL;
}