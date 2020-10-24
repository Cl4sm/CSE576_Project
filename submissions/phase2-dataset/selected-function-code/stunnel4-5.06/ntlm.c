    char *line, buf[BUFSIZ], *ntlm1_txt, *ntlm2_txt, *ntlm3_txt, *tmpstr;
    long content_length=0; /* no HTTP content */

    /* send Proxy-Authorization (phase 1) */
    fd_printf(c, c->remote_fd.fd, "Proxy-Connection: keep-alive");
    ntlm1_txt=ntlm1();
    if(!ntlm1_txt) {
        s_log(LOG_ERR, "Proxy-Authenticate: Failed to build NTLM request");
        longjmp(c->err, 1);
    }
    fd_printf(c, c->remote_fd.fd, "Proxy-Authorization: NTLM %s", ntlm1_txt);
    str_free(ntlm1_txt);
    fd_putline(c, c->remote_fd.fd, ""); /* empty line */
    line=fd_getline(c, c->remote_fd.fd);

    /* receive Proxy-Authenticate (phase 2) */
    if(!is_prefix(line, "HTTP/1.0 407") && !is_prefix(line, "HTTP/1.1 407")) {
        s_log(LOG_ERR, "Proxy-Authenticate: NTLM authorization request rejected");
        do { /* read all headers */
            str_free(line);
            line=fd_getline(c, c->remote_fd.fd);
        } while(*line);
        str_free(line);
        longjmp(c->err, 1);
    }
    ntlm2_txt=NULL;
    do { /* read all headers */
        str_free(line);
        line=fd_getline(c, c->remote_fd.fd);
        if(is_prefix(line, "Proxy-Authenticate: NTLM "))
            ntlm2_txt=str_dup(line+25);
        else if(is_prefix(line, "Content-Length: ")) {
            content_length=strtol(line+16, &tmpstr, 10);
            if(tmpstr==line+16 || *tmpstr || content_length<0) {
                s_log(LOG_ERR, "Proxy-Authenticate: Invalid Content-Length");
                str_free(line);
                longjmp(c->err, 1);
            }
        }
    } while(*line);
    if(!ntlm2_txt) { /* no Proxy-Authenticate: NTLM header */
        s_log(LOG_ERR, "Proxy-Authenticate: NTLM header not found");
        str_free(line);
        longjmp(c->err, 1);
    }

    /* read and ignore HTTP content (if any) */
    while(content_length>0) {
        s_read(c, c->remote_fd.fd, buf, s_min(content_length, BUFSIZ));
        content_length-=s_min(content_length, BUFSIZ);
    }

    /* send Proxy-Authorization (phase 3) */
    fd_printf(c, c->remote_fd.fd, "CONNECT %s HTTP/1.1", opt->protocol_host);
    fd_printf(c, c->remote_fd.fd, "Host: %s", opt->protocol_host);
    ntlm3_txt=ntlm3(opt->protocol_username, opt->protocol_password, ntlm2_txt);
    str_free(ntlm2_txt);
    if(!ntlm3_txt) {
        s_log(LOG_ERR, "Proxy-Authenticate: Failed to build NTLM response");
        longjmp(c->err, 1);
    }
    fd_printf(c, c->remote_fd.fd, "Proxy-Authorization: NTLM %s", ntlm3_txt);
    str_free(ntlm3_txt);
}
