    char *tmpstr;
    SERVICE_OPTIONS *tmpsrv;

    /* server mode: update servername_list based on the SNI option */
    if(!section->option.client && section->sni) {
        tmpstr=strchr(section->sni, ':');
        if(!tmpstr)
            return "Invalid SNI parameter format";
        *tmpstr++='\0';
        for(tmpsrv=new_service_options.next; tmpsrv; tmpsrv=tmpsrv->next)
            if(!strcmp(tmpsrv->servname, section->sni))
                break;
        if(!tmpsrv)
            return "SNI section name not found";
        if(tmpsrv->option.client)
            return "SNI master service is a TLS client";
        if(tmpsrv->servername_list_tail) {
            tmpsrv->servername_list_tail->next=str_alloc(sizeof(SERVERNAME_LIST));
            tmpsrv->servername_list_tail=tmpsrv->servername_list_tail->next;
        } else { /* first virtual service */
            tmpsrv->servername_list_head=
                tmpsrv->servername_list_tail=
                str_alloc(sizeof(SERVERNAME_LIST));
            tmpsrv->ssl_options_set|=
                SSL_OP_NO_SESSION_RESUMPTION_ON_RENEGOTIATION;
        }
        tmpsrv->servername_list_tail->servername=str_dup(tmpstr);
        tmpsrv->servername_list_tail->opt=section;
        tmpsrv->servername_list_tail->next=NULL;
        section->option.sni=1;
        /* always negotiate a new session on renegotiation, as the SSL
         * context settings (including access control) may be different */
        section->ssl_options_set|=
            SSL_OP_NO_SESSION_RESUMPTION_ON_RENEGOTIATION;
    }

    /* client mode: setup SNI default based on 'protocolHost' and 'connect' options */
    if(section->option.client && !section->sni) {
        /* setup host_name for SNI, prefer SNI and protocolHost if specified */
        if(section->protocol_host) /* 'protocolHost' option */
            section->sni=str_dup(section->protocol_host);
        else if(section->connect_addr.names) /* 'connect' option */
            section->sni=str_dup(section->connect_addr.names->name); /* first hostname */
        if(section->sni) { /* either 'protocolHost' or 'connect' specified */
            tmpstr=strrchr(section->sni, ':');
            if(tmpstr) { /* 'host:port' -> drop ':port' */
                *tmpstr='\0';
            } else { /* 'port' -> default to 'localhost' */
                str_free(section->sni);
                section->sni=str_dup("localhost");
            }
        }
    }
    return NULL;
}
