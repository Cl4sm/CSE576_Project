void
preinitIo()
{
#ifdef HAVE_IPV6_PREFER_TEMPADDR
    CONFIG_VARIABLE_SETTABLE(useTemporarySourceAddress, CONFIG_TRISTATE,
                             configIntSetter,
                             "Prefer IPv6 temporary source address.");
#endif

#ifdef HAVE_WINSOCK
    /* Load the winsock dll */
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2);
    int err = WSAStartup( wVersionRequested, &wsaData );
    if (err != 0) {
        do_log_error(L_ERROR, err, "Couldn't load winsock dll");
        exit(-1);
    }
#endif
    return;
}