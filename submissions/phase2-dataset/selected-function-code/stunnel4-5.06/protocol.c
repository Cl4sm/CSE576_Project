    if(phase==PROTOCOL_CHECK) /* default to be overridden by protocols */
        opt->option.connect_before_ssl=opt->option.client;
    if(!opt->protocol) /* no protocol specified */
        return NULL; /* skip further actions */
    if(!strcasecmp(opt->protocol, "proxy"))
        return opt->option.client ?
            "The 'proxy' protocol is not supported in client mode" :
            proxy_server(c, opt, phase);
    if(!strcasecmp(opt->protocol, "cifs"))
        return opt->option.client ?
            cifs_client(c, opt, phase) :
            cifs_server(c, opt, phase);
    if(!strcasecmp(opt->protocol, "pgsql"))
        return opt->option.client ?
            pgsql_client(c, opt, phase) :
            pgsql_server(c, opt, phase);
    if(!strcasecmp(opt->protocol, "smtp"))
        return opt->option.client ?
            smtp_client(c, opt, phase) :
            smtp_server(c, opt, phase);
    if(!strcasecmp(opt->protocol, "pop3"))
        return opt->option.client ?
            pop3_client(c, opt, phase) :
            pop3_server(c, opt, phase);
    if(!strcasecmp(opt->protocol, "imap"))
        return opt->option.client ?
            imap_client(c, opt, phase) :
            imap_server(c, opt, phase);
    if(!strcasecmp(opt->protocol, "nntp"))
        return opt->option.client ?
            nntp_client(c, opt, phase) :
            "The 'nntp' protocol is not supported in server mode";
    if(!strcasecmp(opt->protocol, "connect"))
        return opt->option.client ?
            connect_client(c, opt, phase) :
            connect_server(c, opt, phase);
    return "Protocol not supported";
}
