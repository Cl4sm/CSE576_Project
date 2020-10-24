static apr_status_t rfc1413_query(apr_socket_t *sock, conn_rec *conn,
                                  server_rec *srv)
{
    apr_port_t rmt_port, our_port;
    apr_port_t sav_rmt_port, sav_our_port;
    apr_size_t i;
    char *cp;
    char buffer[RFC1413_MAXDATA + 1];
    char user[RFC1413_USERLEN + 1];     /* XXX */
    apr_size_t buflen;

    sav_our_port = conn->local_addr->port;
    sav_rmt_port = conn->client_addr->port;

    /* send the data */
    buflen = apr_snprintf(buffer, sizeof(buffer), "%hu,%hu\r\n", sav_rmt_port,
                          sav_our_port);
    ap_xlate_proto_to_ascii(buffer, buflen);

    /* send query to server. Handle short write. */
    i = 0;
    while (i < buflen) {
        apr_size_t j = strlen(buffer + i);
        apr_status_t status;
        status  = apr_socket_send(sock, buffer+i, &j);
        if (status != APR_SUCCESS) {
            ap_log_error(APLOG_MARK, APLOG_CRIT, status, srv, APLOGNO(01497)
                         "write: rfc1413: error sending request");
            return status;
        }
        else if (j > 0) {
            i+=j;
        }
    }

    /*
     * Read response from server. - the response should be newline
     * terminated according to rfc - make sure it doesn't stomp its
     * way out of the buffer.
     */

    i = 0;
    memset(buffer, '\0', sizeof(buffer));
    /*
     * Note that the strchr function below checks for \012 instead of '\n'
     * this allows it to work on both ASCII and EBCDIC machines.
     */
    while((cp = strchr(buffer, '\012')) == NULL && i < sizeof(buffer) - 1) {
        apr_size_t j = sizeof(buffer) - 1 - i;
        apr_status_t status;
        status = apr_socket_recv(sock, buffer+i, &j);
        if (status != APR_SUCCESS) {
            ap_log_error(APLOG_MARK, APLOG_CRIT, status, srv, APLOGNO(01498)
                         "read: rfc1413: error reading response");
            return status;
        }
        else if (j > 0) {
            i+=j;
        }
        else if (status == APR_SUCCESS && j == 0) {
            /* Oops... we ran out of data before finding newline */
            return APR_EINVAL;
        }
    }

/* RFC1413_USERLEN = 512 */
    ap_xlate_proto_from_ascii(buffer, i);
    if (sscanf(buffer, "%hu , %hu : USERID :%*[^:]:%512s", &rmt_port, &our_port,
               user) != 3 || sav_rmt_port != rmt_port
        || sav_our_port != our_port)
        return APR_EINVAL;

    /*
     * Strip trailing carriage return. It is part of the
     * protocol, not part of the data.
     */

    if ((cp = strchr(user, '\r')))
        *cp = '\0';

    conn->remote_logname = apr_pstrdup(conn->pool, user);

    return APR_SUCCESS;
}