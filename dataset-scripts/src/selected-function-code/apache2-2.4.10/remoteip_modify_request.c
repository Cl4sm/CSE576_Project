static int remoteip_modify_request(request_rec *r)
{
    conn_rec *c = r->connection;
    remoteip_config_t *config = (remoteip_config_t *)
        ap_get_module_config(r->server->module_config, &remoteip_module);
    remoteip_req_t *req = NULL;

    apr_sockaddr_t *temp_sa;

    apr_status_t rv;
    char *remote;
    char *proxy_ips = NULL;
    char *parse_remote;
    char *eos;
    unsigned char *addrbyte;
    void *internal = NULL;

    if (!config->header_name) {
        return DECLINED;
    }

    remote = (char *) apr_table_get(r->headers_in, config->header_name);
    if (!remote) {
        return OK;
    }
    remote = apr_pstrdup(r->pool, remote);

    temp_sa = c->client_addr;

    while (remote) {

        /* verify user agent IP against the trusted proxy list
         */
        if (config->proxymatch_ip) {
            int i;
            remoteip_proxymatch_t *match;
            match = (remoteip_proxymatch_t *)config->proxymatch_ip->elts;
            for (i = 0; i < config->proxymatch_ip->nelts; ++i) {
                if (apr_ipsubnet_test(match[i].ip, temp_sa)) {
                    internal = match[i].internal;
                    break;
                }
            }
            if (i && i >= config->proxymatch_ip->nelts) {
                break;
            }
        }

        if ((parse_remote = strrchr(remote, ',')) == NULL) {
            parse_remote = remote;
            remote = NULL;
        }
        else {
            *(parse_remote++) = '\0';
        }

        while (*parse_remote == ' ') {
            ++parse_remote;
        }

        eos = parse_remote + strlen(parse_remote) - 1;
        while (eos >= parse_remote && *eos == ' ') {
            *(eos--) = '\0';
        }

        if (eos < parse_remote) {
            if (remote) {
                *(remote + strlen(remote)) = ',';
            }
            else {
                remote = parse_remote;
            }
            break;
        }

        /* We map as IPv4 rather than IPv6 for equivalent host names
         * or IPV4OVERIPV6
         */
        rv = apr_sockaddr_info_get(&temp_sa,  parse_remote,
                                   APR_UNSPEC, temp_sa->port,
                                   APR_IPV4_ADDR_OK, r->pool);
        if (rv != APR_SUCCESS) {
            ap_log_rerror(APLOG_MARK, APLOG_DEBUG,  rv, r, APLOGNO(01568)
                          "RemoteIP: Header %s value of %s cannot be parsed "
                          "as a client IP",
                          config->header_name, parse_remote);

            if (remote) {
                *(remote + strlen(remote)) = ',';
            }
            else {
                remote = parse_remote;
            }
            break;
        }

        addrbyte = (unsigned char *) &temp_sa->sa.sin.sin_addr;

        /* For intranet (Internal proxies) ignore all restrictions below */
        if (!internal
              && ((temp_sa->family == APR_INET
                   /* For internet (non-Internal proxies) deny all
                    * RFC3330 designated local/private subnets:
                    * 10.0.0.0/8   169.254.0.0/16  192.168.0.0/16
                    * 127.0.0.0/8  172.16.0.0/12
                    */
                      && (addrbyte[0] == 10
                       || addrbyte[0] == 127
                       || (addrbyte[0] == 169 && addrbyte[1] == 254)
                       || (addrbyte[0] == 172 && (addrbyte[1] & 0xf0) == 16)
                       || (addrbyte[0] == 192 && addrbyte[1] == 168)))
#if APR_HAVE_IPV6
               || (temp_sa->family == APR_INET6
                   /* For internet (non-Internal proxies) we translated
                    * IPv4-over-IPv6-mapped addresses as IPv4, above.
                    * Accept only Global Unicast 2000::/3 defined by RFC4291
                    */
                      && ((temp_sa->sa.sin6.sin6_addr.s6_addr[0] & 0xe0) != 0x20))
#endif
        )) {
            ap_log_rerror(APLOG_MARK, APLOG_DEBUG,  rv, r, APLOGNO(01569)
                          "RemoteIP: Header %s value of %s appears to be "
                          "a private IP or nonsensical.  Ignored",
                          config->header_name, parse_remote);
            if (remote) {
                *(remote + strlen(remote)) = ',';
            }
            else {
                remote = parse_remote;
            }

            break;
        }

        /* save away our results */
        if (!req) {
            req = (remoteip_req_t *) apr_palloc(r->pool, sizeof(remoteip_req_t));
            req->useragent_ip = r->useragent_ip;
        }

        /* Set useragent_ip string */
        if (!internal) {
            if (proxy_ips) {
                proxy_ips = apr_pstrcat(r->pool, proxy_ips, ", ",
                                        req->useragent_ip, NULL);
            }
            else {
                proxy_ips = req->useragent_ip;
            }
        }

        req->useragent_addr = temp_sa;
        apr_sockaddr_ip_get(&req->useragent_ip, req->useragent_addr);
    }

    /* Nothing happened? */
    if (!req) {
        return OK;
    }

    req->proxied_remote = remote;
    req->proxy_ips = proxy_ips;

    if (req->proxied_remote) {
        apr_table_setn(r->headers_in, config->header_name,
                       req->proxied_remote);
    }
    else {
        apr_table_unset(r->headers_in, config->header_name);
    }
    if (req->proxy_ips) {
        apr_table_setn(r->notes, "remoteip-proxy-ip-list", req->proxy_ips);
        if (config->proxies_header_name) {
            apr_table_setn(r->headers_in, config->proxies_header_name,
                           req->proxy_ips);
        }
    }

    r->useragent_addr = req->useragent_addr;
    r->useragent_ip = req->useragent_ip;

    ap_log_rerror(APLOG_MARK, APLOG_TRACE1, 0, r,
                  req->proxy_ips
                      ? "Using %s as client's IP by proxies %s"
                      : "Using %s as client's IP by internal proxies%s",
                  req->useragent_ip,
                  (req->proxy_ips ? req->proxy_ips : ""));
    return OK;
}