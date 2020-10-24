static const char *ap_ident_lookup(request_rec *r)
{
    ident_config_rec *conf;
    apr_socket_t *sock;
    apr_status_t rv;
    conn_rec *conn = r->connection;
    server_rec *srv = r->server;

    conf = ap_get_module_config(r->per_dir_config, &ident_module);

    /* return immediately if ident requests are disabled */
    if (!(conf->do_rfc1413 & ~RFC1413_UNSET)) {
        return NULL;
    }

    rv = rfc1413_connect(&sock, conn, srv, conf->timeout);
    if (rv == APR_SUCCESS) {
        rv = rfc1413_query(sock, conn, srv);
        apr_socket_close(sock);
    }
    if (rv != APR_SUCCESS) {
        conn->remote_logname = FROM_UNKNOWN;
    }

    return (const char *)conn->remote_logname;
}