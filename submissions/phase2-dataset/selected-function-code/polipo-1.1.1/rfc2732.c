AtomPtr
rfc2732(AtomPtr name)
{
    char buf[40]; /* 8*4 (hexdigits) + 7 (colons) + 1 ('\0') */
    int rc;
    AtomPtr a = NULL;

    if(name->length < 40+2 && 
       name->string[0] == '[' && name->string[name->length - 1] == ']') {
        struct in6_addr in6a;
        memcpy(buf, name->string + 1, name->length - 2);
        buf[name->length - 2] = '\0';
        rc = inet_pton(AF_INET6, buf, &in6a);
        if(rc == 1) {
            char s[1 + sizeof(HostAddressRec)];
            memset(s, 0, sizeof(s));
            s[0] = DNS_A;
            s[1] = 6;
            memcpy(s + 2, &in6a, 16);
            a = internAtomN(s, 1 + sizeof(HostAddressRec));
            if(a == NULL)
                return NULL;
        }
    }
    return a;
}