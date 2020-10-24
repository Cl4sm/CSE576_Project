NOEXPORT char *parse_service_option(CMD cmd, SERVICE_OPTIONS *section,
        char *opt, char *arg) {
    char *tmpstr;
    int tmpnum, endpoints=0;

    if(cmd==CMD_DEFAULT || cmd==CMD_HELP) {
        s_log(LOG_NOTICE, " ");
        s_log(LOG_NOTICE, "Service-level options:");
    }

    /* accept */
    switch(cmd) {
    case CMD_BEGIN:
        section->option.accept=0;
        memset(&section->local_addr, 0, sizeof(SOCKADDR_UNION));
        section->local_addr.in.sin_family=AF_INET;
        section->fd=-1;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "accept"))
            break;
        section->option.accept=1;
        if(!name2addr(&section->local_addr, arg, DEFAULT_ANY))
            return "Failed to resolve accepting address";
        return NULL; /* OK */
    case CMD_END:
        if(section->option.accept)
            ++endpoints;
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = [host:]port accept connections on specified host:port",
            "accept");
        break;
    }

    /* CApath */
    switch(cmd) {
    case CMD_BEGIN:
#if 0
        section->ca_dir=(char *)X509_get_default_cert_dir();
#endif
        section->ca_dir=NULL;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "CApath"))
            break;
        if(arg[0]) /* not empty */
            section->ca_dir=str_dup(arg);
        else
            section->ca_dir=NULL;
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
#if 0
        s_log(LOG_NOTICE, "%-22s = %s", "CApath",
            section->ca_dir ? section->ca_dir : "(none)");
#endif
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = CA certificate directory for 'verify' option",
            "CApath");
        break;
    }

    /* CAfile */
    switch(cmd) {
    case CMD_BEGIN:
#if 0
        section->ca_file=(char *)X509_get_default_certfile();
#endif
        section->ca_file=NULL;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "CAfile"))
            break;
        if(arg[0]) /* not empty */
            section->ca_file=str_dup(arg);
        else
            section->ca_file=NULL;
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
#if 0
        s_log(LOG_NOTICE, "%-22s = %s", "CAfile",
            section->ca_file ? section->ca_file : "(none)");
#endif
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = CA certificate file for 'verify' option",
            "CAfile");
        break;
    }

    /* cert */
    switch(cmd) {
    case CMD_BEGIN:
        section->cert=NULL;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "cert"))
            break;
        section->cert=str_dup(arg);
        return NULL; /* OK */
    case CMD_END:
        if(!section->option.client && !section->cert)
#ifdef HAVE_OSSL_ENGINE_H
            if(!section->engine)
#endif
                return "SSL server needs a certificate";
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break; /* no default certificate */
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = certificate chain", "cert");
        break;
    }

    /* ciphers */
    switch(cmd) {
    case CMD_BEGIN:
        section->cipher_list=NULL;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "ciphers"))
            break;
        section->cipher_list=str_dup(arg);
        return NULL; /* OK */
    case CMD_END:
#ifdef USE_FIPS
        if(new_global_options.option.fips) {
            if(!new_service_options.cipher_list)
                new_service_options.cipher_list="FIPS";
        } else
#endif /* USE_FIPS */
        {
            if(!new_service_options.cipher_list)
                new_service_options.cipher_list=stunnel_cipher_list;
        }

        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
#ifdef USE_FIPS
        s_log(LOG_NOTICE, "%-22s = %s %s", "ciphers",
            "FIPS", "(with \"fips = yes\")");
        s_log(LOG_NOTICE, "%-22s = %s %s", "ciphers",
            stunnel_cipher_list, "(with \"fips = no\")");
#else
        s_log(LOG_NOTICE, "%-22s = %s", "ciphers", stunnel_cipher_list);
#endif /* USE_FIPS */
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = list of permitted SSL ciphers", "ciphers");
        break;
    }

    /* client */
    switch(cmd) {
    case CMD_BEGIN:
        section->option.client=0;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "client"))
            break;
        if(!strcasecmp(arg, "yes"))
            section->option.client=1;
        else if(!strcasecmp(arg, "no"))
            section->option.client=0;
        else
            return "Argument should be either 'yes' or 'no'";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = yes|no client mode (remote service uses SSL)",
            "client");
        break;
    }

    /* connect */
    switch(cmd) {
    case CMD_BEGIN:
        section->option.remote=0;
        addrlist_init(&section->connect_addr, 1);
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "connect"))
            break;
        section->option.remote=1;
        name_list_append(&section->connect_addr.names, arg);
        return NULL; /* OK */
    case CMD_END:
        if(!section->option.delayed_lookup &&
                section->connect_addr.names &&
                !addrlist_resolve(&section->connect_addr)) {
            s_log(LOG_INFO,
                "Cannot resolve connect target - delaying DNS lookup");
            section->option.delayed_lookup=1;
        }
        if(section->option.remote)
            ++endpoints;
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = [host:]port to connect",
            "connect");
        break;
    }

    /* CRLpath */
    switch(cmd) {
    case CMD_BEGIN:
        section->crl_dir=NULL;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "CRLpath"))
            break;
        if(arg[0]) /* not empty */
            section->crl_dir=str_dup(arg);
        else
            section->crl_dir=NULL;
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = CRL directory", "CRLpath");
        break;
    }

    /* CRLfile */
    switch(cmd) {
    case CMD_BEGIN:
        section->crl_file=NULL;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "CRLfile"))
            break;
        if(arg[0]) /* not empty */
            section->crl_file=str_dup(arg);
        else
            section->crl_file=NULL;
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = CRL file", "CRLfile");
        break;
    }

#ifndef OPENSSL_NO_ECDH

    /* curve */
#define DEFAULT_CURVE NID_X9_62_prime256v1
    switch(cmd) {
    case CMD_BEGIN:
        section->curve=DEFAULT_CURVE;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "curve"))
            break;
        section->curve=OBJ_txt2nid(arg);
        if(section->curve==NID_undef)
            return "Curve name not supported";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        s_log(LOG_NOTICE, "%-22s = %s", "curve", OBJ_nid2ln(DEFAULT_CURVE));
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = ECDH curve name", "curve");
        break;
    }

#endif /* !OPENSSL_NO_ECDH */

    /* delay */
    switch(cmd) {
    case CMD_BEGIN:
        section->option.delayed_lookup=0;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "delay"))
            break;
        if(!strcasecmp(arg, "yes"))
            section->option.delayed_lookup=1;
        else if(!strcasecmp(arg, "no"))
            section->option.delayed_lookup=0;
        else
            return "Argument should be either 'yes' or 'no'";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = yes|no delay DNS lookup for 'connect' option",
            "delay");
        break;
    }

#ifdef HAVE_OSSL_ENGINE_H

    /* engineId */
    switch(cmd) {
    case CMD_BEGIN:
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "engineId"))
            break;
        section->engine=engine_get_by_id(arg);
        if(!section->engine)
            return "Engine ID not found";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = ID of engine to read the key from",
            "engineId");
        break;
    }

    /* engineNum */
    switch(cmd) {
    case CMD_BEGIN:
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "engineNum"))
            break;
        tmpnum=strtol(arg, &tmpstr, 10);
        if(tmpstr==arg || *tmpstr) /* not a number */
            return "Illegal engine number";
        section->engine=engine_get_by_num(tmpnum-1);
        if(!section->engine)
            return "Illegal engine number";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = number of engine to read the key from",
            "engineNum");
        break;
    }

#endif

    /* exec */
    switch(cmd) {
    case CMD_BEGIN:
        section->option.program=0;
        section->execname=NULL;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "exec"))
            break;
        section->option.program=1;
        section->execname=str_dup(arg);
#ifdef USE_WIN32
        section->execargs=str_dup(arg);
#else
        if(!section->execargs) {
            section->execargs=str_alloc(2*sizeof(char *));
            section->execargs[0]=section->execname;
            section->execargs[1]=NULL; /* to show that it's null-terminated */
        }
#endif
        return NULL; /* OK */
    case CMD_END:
        if(section->option.program)
            ++endpoints;
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = file execute local inetd-type program",
            "exec");
        break;
    }

    /* execargs */
    switch(cmd) {
    case CMD_BEGIN:
        section->execargs=NULL;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "execargs"))
            break;
#ifdef USE_WIN32
        section->execargs=str_dup(arg);
#else
        section->execargs=argalloc(arg);
#endif
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = arguments for 'exec' (including $0)",
            "execargs");
        break;
    }

    /* failover */
    switch(cmd) {
    case CMD_BEGIN:
        section->failover=FAILOVER_RR;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "failover"))
            break;
        if(!strcasecmp(arg, "rr"))
            section->failover=FAILOVER_RR;
        else if(!strcasecmp(arg, "prio"))
            section->failover=FAILOVER_PRIO;
        else
            return "Argument should be either 'rr' or 'prio'";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = rr|prio failover strategy",
            "failover");
        break;
    }

    /* ident */
    switch(cmd) {
    case CMD_BEGIN:
        section->username=NULL;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "ident"))
            break;
        section->username=str_dup(arg);
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = username for IDENT (RFC 1413) checking", "ident");
        break;
    }

    /* key */
    switch(cmd) {
    case CMD_BEGIN:
        section->key=NULL;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "key"))
            break;
        section->key=str_dup(arg);
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = certificate private key", "key");
        break;
    }

#ifdef USE_LIBWRAP
    switch(cmd) {
    case CMD_BEGIN:
        section->option.libwrap=0; /* disable libwrap by default */
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "libwrap"))
            break;
        if(!strcasecmp(arg, "yes"))
            section->option.libwrap=1;
        else if(!strcasecmp(arg, "no"))
            section->option.libwrap=0;
        else
            return "Argument should be either 'yes' or 'no'";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = yes|no use /etc/hosts.allow and /etc/hosts.deny",
            "libwrap");
        break;
    }
#endif /* USE_LIBWRAP */

    /* local */
    switch(cmd) {
    case CMD_BEGIN:
        section->option.local=0;
        memset(&section->source_addr, 0, sizeof(SOCKADDR_UNION));
        section->source_addr.in.sin_family=AF_INET;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "local"))
            break;
        section->option.local=1;
        if(!hostport2addr(&section->source_addr, arg, "0"))
            return "Failed to resolve local address";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = IP address to be used as source for remote"
            " connections", "local");
        break;
    }

#ifdef HAVE_OSSL_OCSP_H

    /* OCSP */
    switch(cmd) {
    case CMD_BEGIN:
        section->option.ocsp=0;
        memset(&section->ocsp_addr, 0, sizeof(SOCKADDR_UNION));
        section->ocsp_addr.in.sin_family=AF_INET;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "ocsp"))
            break;
        section->option.ocsp=1;
        return parse_ocsp_url(section, arg);
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = OCSP server URL", "ocsp");
        break;
    }

    /* OCSPflag */
    switch(cmd) {
    case CMD_BEGIN:
        section->ocsp_flags=0;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "OCSPflag"))
            break;
        tmpnum=parse_ocsp_flag(arg);
        if(!tmpnum)
            return "Illegal OCSP flag";
        section->ocsp_flags|=tmpnum;
        return NULL;
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = OCSP server flags", "OCSPflag");
        break;
    }

#endif /* HAVE_OSSL_OCSP_H */

    /* options */
    switch(cmd) {
    case CMD_BEGIN:
        section->ssl_options_set|=SSL_OP_NO_SSLv2|SSL_OP_NO_SSLv3;
        section->ssl_options_clear=0;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "options"))
            break;
        if(*arg=='-') {
            tmpnum=parse_ssl_option(arg+1);
            if(!tmpnum)
                return "Illegal SSL option";
            section->ssl_options_clear|=tmpnum;
        } else {
            tmpnum=parse_ssl_option(arg);
            if(!tmpnum)
                return "Illegal SSL option";
            section->ssl_options_set|=tmpnum;
        }
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = SSL option", "options");
        s_log(LOG_NOTICE, "%25sset an SSL option", "");
        break;
    }

    /* protocol */
    switch(cmd) {
    case CMD_BEGIN:
        section->protocol=NULL;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "protocol"))
            break;
        section->protocol=str_dup(arg);
        return NULL; /* OK */
    case CMD_END:
        /* this also sets section->option.connect_before_ssl */
        tmpstr=protocol(NULL, section, PROTOCOL_CHECK);
        if(tmpstr)
            return tmpstr;
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = protocol to negotiate before SSL initialization",
            "protocol");
        s_log(LOG_NOTICE, "%25scurrently supported: cifs, connect, imap,", "");
        s_log(LOG_NOTICE, "%25s    nntp, pgsql, pop3, proxy, smtp", "");
        break;
    }

    /* protocolAuthentication */
    switch(cmd) {
    case CMD_BEGIN:
        section->protocol_authentication="basic";
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "protocolAuthentication"))
            break;
        section->protocol_authentication=str_dup(arg);
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = authentication type for protocol negotiations",
            "protocolAuthentication");
        break;
    }

    /* protocolHost */
    switch(cmd) {
    case CMD_BEGIN:
        section->protocol_host=NULL;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "protocolHost"))
            break;
        section->protocol_host=str_dup(arg);
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = host:port for protocol negotiations",
            "protocolHost");
        break;
    }

    /* protocolPassword */
    switch(cmd) {
    case CMD_BEGIN:
        section->protocol_password=NULL;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "protocolPassword"))
            break;
        section->protocol_password=str_dup(arg);
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = password for protocol negotiations",
            "protocolPassword");
        break;
    }

    /* protocolUsername */
    switch(cmd) {
    case CMD_BEGIN:
        section->protocol_username=NULL;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "protocolUsername"))
            break;
        section->protocol_username=str_dup(arg);
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = username for protocol negotiations",
            "protocolUsername");
        break;
    }

    /* pty */
#ifndef USE_WIN32
    switch(cmd) {
    case CMD_BEGIN:
        section->option.pty=0;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "pty"))
            break;
        if(!strcasecmp(arg, "yes"))
            section->option.pty=1;
        else if(!strcasecmp(arg, "no"))
            section->option.pty=0;
        else
            return "Argument should be either 'yes' or 'no'";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = yes|no allocate pseudo terminal for 'exec' option",
            "pty");
        break;
    }
#endif

    /* redirect */
    switch(cmd) {
    case CMD_BEGIN:
        addrlist_init(&section->redirect_addr, 1);
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "redirect"))
            break;
#ifdef SSL_OP_NO_TICKET
        /* disable RFC4507 support introduced in OpenSSL 0.9.8f */
        /* session tickets do not support SSL_SESSION_*_ex_data() */
        section->ssl_options_set|=SSL_OP_NO_TICKET;
#endif
        name_list_append(&section->redirect_addr.names, arg);
        return NULL; /* OK */
    case CMD_END:
        if(!section->option.delayed_lookup &&
                section->redirect_addr.names &&
                !addrlist_resolve(&section->redirect_addr)) {
            s_log(LOG_INFO,
                "Cannot resolve redirect target - delaying DNS lookup");
	    section->option.delayed_lookup=1;
        }
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = [host:]port to redirect on authentication failures",
            "redirect");
        break;
    }

    /* renegotiation */
    switch(cmd) {
    case CMD_BEGIN:
        section->option.renegotiation=1;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "renegotiation"))
            break;
        if(!strcasecmp(arg, "yes"))
            section->option.renegotiation=1;
        else if(!strcasecmp(arg, "no"))
            section->option.renegotiation=0;
        else
            return "argument should be either 'yes' or 'no'";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = yes|no support renegotiation",
              "renegotiation");
        break;
    }

    /* reset */
    switch(cmd) {
    case CMD_BEGIN:
        section->option.reset=1; /* enabled by default */
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "reset"))
            break;
        if(!strcasecmp(arg, "yes"))
            section->option.reset=1;
        else if(!strcasecmp(arg, "no"))
            section->option.reset=0;
        else
            return "Argument should be either 'yes' or 'no'";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = yes|no send TCP RST on error",
            "retry");
        break;
    }

    /* retry */
    switch(cmd) {
    case CMD_BEGIN:
        section->option.retry=0;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "retry"))
            break;
        if(!strcasecmp(arg, "yes"))
            section->option.retry=1;
        else if(!strcasecmp(arg, "no"))
            section->option.retry=0;
        else
            return "Argument should be either 'yes' or 'no'";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = yes|no retry connect+exec section",
            "retry");
        break;
    }

    /* sessionCacheSize */
    switch(cmd) {
    case CMD_BEGIN:
        section->session_size=1000L;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "sessionCacheSize"))
            break;
        section->session_size=strtol(arg, &tmpstr, 10);
        if(tmpstr==arg || *tmpstr) /* not a number */
            return "Illegal session cache size";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        s_log(LOG_NOTICE, "%-22s = %ld", "sessionCacheSize", 1000L);
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = session cache size", "sessionCacheSize");
        break;
    }

    /* sessionCacheTimeout */
    switch(cmd) {
    case CMD_BEGIN:
        section->session_timeout=300L;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "sessionCacheTimeout") && strcasecmp(opt, "session"))
            break;
        section->session_timeout=strtol(arg, &tmpstr, 10);
        if(tmpstr==arg || *tmpstr) /* not a number */
            return "Illegal session cache timeout";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        s_log(LOG_NOTICE, "%-22s = %ld seconds", "sessionCacheTimeout", 300L);
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = session cache timeout (in seconds)",
            "sessionCacheTimeout");
        break;
    }

    /* sessiond */
    switch(cmd) {
    case CMD_BEGIN:
        section->option.sessiond=0;
        memset(&section->sessiond_addr, 0, sizeof(SOCKADDR_UNION));
        section->sessiond_addr.in.sin_family=AF_INET;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "sessiond"))
            break;
        section->option.sessiond=1;
#ifdef SSL_OP_NO_TICKET
        /* disable RFC4507 support introduced in OpenSSL 0.9.8f */
        /* this prevents session callbacks from beeing executed */
        section->ssl_options_set|=SSL_OP_NO_TICKET;
#endif
        if(!name2addr(&section->sessiond_addr, arg, DEFAULT_LOOPBACK))
            return "Failed to resolve sessiond server address";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = [host:]port use sessiond at host:port",
            "sessiond");
        break;
    }

#ifndef OPENSSL_NO_TLSEXT
    /* sni */
    switch(cmd) {
    case CMD_BEGIN:
        section->servername_list_head=NULL;
        section->servername_list_tail=NULL;
        section->option.sni=0;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "sni"))
            break;
        section->sni=str_dup(arg);
        return NULL; /* OK */
    case CMD_END:
        tmpstr=init_sni(section);
        if(tmpstr)
            return tmpstr;
        if(section->option.sni)
            ++endpoints;
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = master_service:host_name for an SNI virtual service",
            "sni");
        break;
    }
#endif /* OPENSSL_NO_TLSEXT */

    /* sslVersion */
    switch(cmd) {
    case CMD_BEGIN:
        section->client_method=(SSL_METHOD *)SSLv23_client_method();
        section->server_method=(SSL_METHOD *)SSLv23_server_method();;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "sslVersion"))
            break;
        if(!strcasecmp(arg, "all")) {
            section->client_method=(SSL_METHOD *)SSLv23_client_method();
            section->server_method=(SSL_METHOD *)SSLv23_server_method();
        } else if(!strcasecmp(arg, "SSLv2")) {
#if !defined(OPENSSL_NO_SSL2)
            section->client_method=(SSL_METHOD *)SSLv2_client_method();
            section->server_method=(SSL_METHOD *)SSLv2_server_method();
#else
            return "SSLv2 not supported";
#endif
        } else if(!strcasecmp(arg, "SSLv3")) {
#if !defined(OPENSSL_NO_SSL3)
            section->client_method=(SSL_METHOD *)SSLv3_client_method();
            section->server_method=(SSL_METHOD *)SSLv3_server_method();
#else
            return "SSLv3 not supported";
#endif
        } else if(!strcasecmp(arg, "TLSv1")) {
#if !defined(OPENSSL_NO_TLS1)
            section->client_method=(SSL_METHOD *)TLSv1_client_method();
            section->server_method=(SSL_METHOD *)TLSv1_server_method();
#else
            return "TLSv1 not supported";
#endif
        } else if(!strcasecmp(arg, "TLSv1.1")) {
#if !defined(OPENSSL_NO_TLS1) && OPENSSL_VERSION_NUMBER>=0x10001000L
            section->client_method=(SSL_METHOD *)TLSv1_1_client_method();
            section->server_method=(SSL_METHOD *)TLSv1_1_server_method();
#else
            return "TLSv1.1 not supported";
#endif
        } else if(!strcasecmp(arg, "TLSv1.2")) {
#if !defined(OPENSSL_NO_TLS1) && OPENSSL_VERSION_NUMBER>=0x10001000L
            section->client_method=(SSL_METHOD *)TLSv1_2_client_method();
            section->server_method=(SSL_METHOD *)TLSv1_2_server_method();
#else
            return "TLSv1.2 not supported";
#endif
        } else
            return "Incorrect version of SSL protocol";
        return NULL; /* OK */
    case CMD_END:
#ifdef USE_FIPS
        if(new_global_options.option.fips) {
#ifndef OPENSSL_NO_SSL2
            if(section->option.client ?
                    section->client_method==(SSL_METHOD *)SSLv2_client_method() :
                    section->server_method==(SSL_METHOD *)SSLv2_server_method())
                return "\"sslVersion = SSLv2\" not supported in FIPS mode";
#endif
#ifndef OPENSSL_NO_SSL3
            if(section->option.client ?
                    section->client_method==(SSL_METHOD *)SSLv3_client_method() :
                    section->server_method==(SSL_METHOD *)SSLv3_server_method())
                return "\"sslVersion = SSLv3\" not supported in FIPS mode";
#endif
        }
#endif /* USE_FIPS */
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = all|SSLv2|SSLv3|TLSv1"
#if OPENSSL_VERSION_NUMBER>=0x10001000L
            "|TLSv1.1|TLSv1.2"
#endif
            " SSL method", "sslVersion");
        break;
    }

#ifndef USE_FORK
    /* stack */
    switch(cmd) {
    case CMD_BEGIN:
        section->stack_size=DEFAULT_STACK_SIZE;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "stack"))
            break;
        section->stack_size=strtol(arg, &tmpstr, 10);
        if(tmpstr==arg || *tmpstr) /* not a number */
            return "Illegal thread stack size";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        s_log(LOG_NOTICE, "%-22s = %d bytes", "stack", DEFAULT_STACK_SIZE);
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = thread stack size (in bytes)", "stack");
        break;
    }
#endif

    /* TIMEOUTbusy */
    switch(cmd) {
    case CMD_BEGIN:
        section->timeout_busy=300; /* 5 minutes */
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "TIMEOUTbusy"))
            break;
        section->timeout_busy=strtol(arg, &tmpstr, 10);
        if(tmpstr==arg || *tmpstr) /* not a number */
            return "Illegal busy timeout";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        s_log(LOG_NOTICE, "%-22s = %d seconds", "TIMEOUTbusy", 300);
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = seconds to wait for expected data", "TIMEOUTbusy");
        break;
    }

    /* TIMEOUTclose */
    switch(cmd) {
    case CMD_BEGIN:
        section->timeout_close=60; /* 1 minute */
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "TIMEOUTclose"))
            break;
        section->timeout_close=strtol(arg, &tmpstr, 10);
        if(tmpstr==arg || *tmpstr) /* not a number */
            return "Illegal close timeout";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        s_log(LOG_NOTICE, "%-22s = %d seconds", "TIMEOUTclose", 60);
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = seconds to wait for close_notify",
            "TIMEOUTclose");
        break;
    }

    /* TIMEOUTconnect */
    switch(cmd) {
    case CMD_BEGIN:
        section->timeout_connect=10; /* 10 seconds */
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "TIMEOUTconnect"))
            break;
        section->timeout_connect=strtol(arg, &tmpstr, 10);
        if(tmpstr==arg || *tmpstr) /* not a number */
            return "Illegal connect timeout";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        s_log(LOG_NOTICE, "%-22s = %d seconds", "TIMEOUTconnect", 10);
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = seconds to connect remote host", "TIMEOUTconnect");
        break;
    }

    /* TIMEOUTidle */
    switch(cmd) {
    case CMD_BEGIN:
        section->timeout_idle=43200; /* 12 hours */
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "TIMEOUTidle"))
            break;
        section->timeout_idle=strtol(arg, &tmpstr, 10);
        if(tmpstr==arg || *tmpstr) /* not a number */
            return "Illegal idle timeout";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        s_log(LOG_NOTICE, "%-22s = %d seconds", "TIMEOUTidle", 43200);
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE, "%-22s = seconds to keep an idle connection", "TIMEOUTidle");
        break;
    }

    /* transparent */
#ifndef USE_WIN32
    switch(cmd) {
    case CMD_BEGIN:
        section->option.transparent_src=0;
        section->option.transparent_dst=0;
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "transparent"))
            break;
        if(!strcasecmp(arg, "none") || !strcasecmp(arg, "no")) {
            section->option.transparent_src=0;
            section->option.transparent_dst=0;
        } else if(!strcasecmp(arg, "source") || !strcasecmp(arg, "yes")) {
            section->option.transparent_src=1;
            section->option.transparent_dst=0;
#ifdef SO_ORIGINAL_DST
        } else if(!strcasecmp(arg, "destination")) {
            section->option.transparent_src=0;
            section->option.transparent_dst=1;
        } else if(!strcasecmp(arg, "both")) {
            section->option.transparent_src=1;
            section->option.transparent_dst=1;
#endif
        } else
            return "Selected transparent proxy mode is not available";
        return NULL; /* OK */
    case CMD_END:
        if(section->option.transparent_dst)
            ++endpoints;
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE,
            "%-22s = none|source|destination|both transparent proxy mode",
            "transparent");
        break;
    }
#endif

    /* verify */
    switch(cmd) {
    case CMD_BEGIN:
        section->verify_level=-1; /* do not even request a certificate */
        break;
    case CMD_EXEC:
        if(strcasecmp(opt, "verify"))
            break;
        section->verify_level=strtol(arg, &tmpstr, 10);
        if(tmpstr==arg || *tmpstr) /* not a number */
            return "Bad verify level";
        if(section->verify_level<0 || section->verify_level>4)
            return "Bad verify level";
        return NULL; /* OK */
    case CMD_END:
        break;
    case CMD_FREE:
        break;
    case CMD_DEFAULT:
        s_log(LOG_NOTICE, "%-22s = none", "verify");
        break;
    case CMD_HELP:
        s_log(LOG_NOTICE,
            "%-22s = level of peer certificate verification", "verify");
        s_log(LOG_NOTICE,
            "%25slevel 0 - request and ignore peer cert", "");
        s_log(LOG_NOTICE,
            "%25slevel 1 - only validate peer cert if present", "");
        s_log(LOG_NOTICE,
            "%25slevel 2 - always require a valid peer cert", "");
        s_log(LOG_NOTICE,
            "%25slevel 3 - verify peer with locally installed cert", "");
        s_log(LOG_NOTICE,
            "%25slevel 4 - ignore CA chain and only verify peer cert", "");
        break;
    }

    if(cmd==CMD_EXEC)
        return option_not_found;

    if(cmd==CMD_END) {
        if(new_service_options.next) { /* daemon mode checks */
            if(endpoints!=2)
                return "Each service must define two endpoints";
        } else { /* inetd mode checks */
            if(section->option.accept)
                return "'accept' option is only allowed in a [section]";
            /* no need to check for section->option.sni in inetd mode,
               as it requires valid sections to be set */
            if(endpoints!=1)
                return "Inetd mode must define one endpoint";
        }
        if(context_init(section)) /* initialize SSL context */
            return "Failed to initialize SSL context";
    }

    return NULL; /* OK */
}
