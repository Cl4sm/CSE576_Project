static apr_status_t rfc1413_connect(apr_socket_t **newsock, conn_rec *conn,
                                    server_rec *srv, apr_time_t timeout)
{
    apr_status_t rv;
    apr_sockaddr_t *localsa, *destsa;

    if ((rv = apr_sockaddr_info_get(&localsa, conn->local_ip, APR_UNSPEC,
                              0, /* ephemeral port */
                              0, conn->pool)) != APR_SUCCESS) {
        /* This should not fail since we have a numeric address string
         * as the host. */
        ap_log_error(APLOG_MARK, APLOG_CRIT, rv, srv, APLOGNO(01492)
                     "rfc1413: apr_sockaddr_info_get(%s) failed",
                     conn->local_ip);
        return rv;
    }

    if ((rv = apr_sockaddr_info_get(&destsa, conn->client_ip,
                              localsa->family, /* has to match */
                              RFC1413_PORT, 0, conn->pool)) != APR_SUCCESS) {
        /* This should not fail since we have a numeric address string
         * as the host. */
        ap_log_error(APLOG_MARK, APLOG_CRIT, rv, srv, APLOGNO(01493)
                     "rfc1413: apr_sockaddr_info_get(%s) failed",
                     conn->client_ip);
        return rv;
    }

    if ((rv = apr_socket_create(newsock,
                                localsa->family, /* has to match */
                                SOCK_STREAM, 0, conn->pool)) != APR_SUCCESS) {
        ap_log_error(APLOG_MARK, APLOG_CRIT, rv, srv, APLOGNO(01494)
                     "rfc1413: error creating query socket");
        return rv;
    }

    if ((rv = apr_socket_timeout_set(*newsock, timeout)) != APR_SUCCESS) {
        ap_log_error(APLOG_MARK, APLOG_CRIT, rv, srv, APLOGNO(01495)
                     "rfc1413: error setting query socket timeout");
        apr_socket_close(*newsock);
        return rv;
    }

/*
 * Bind the local and remote ends of the query socket to the same
 * IP addresses as the connection under investigation. We go
 * through all this trouble because the local or remote system
 * might have more than one network address. The RFC1413 etc.
 * client sends only port numbers; the server takes the IP
 * addresses from the query socket.
 */

    if ((rv = apr_socket_bind(*newsock, localsa)) != APR_SUCCESS) {
        ap_log_error(APLOG_MARK, APLOG_CRIT, rv, srv, APLOGNO(01496)
                     "rfc1413: Error binding query socket to local port");
        apr_socket_close(*newsock);
        return rv;
    }

/*
 * errors from connect usually imply the remote machine doesn't support
 * the service; don't log such an error
 */
    if ((rv = apr_socket_connect(*newsock, destsa)) != APR_SUCCESS) {
        apr_socket_close(*newsock);
        return rv;
    }

    return APR_SUCCESS;
}