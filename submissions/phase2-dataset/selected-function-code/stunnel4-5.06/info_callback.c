#if OPENSSL_VERSION_NUMBER>=0x0090700fL
        const
#endif
        SSL *ssl, int where, int ret) {
    CLI *c;

    c=SSL_get_ex_data((SSL *)ssl, cli_index);
    if(c) {
        if((where&SSL_CB_HANDSHAKE_DONE)
                && c->reneg_state==RENEG_INIT) {
            /* first (initial) handshake was completed, remember this,
             * so that further renegotiation attempts can be detected */
            c->reneg_state=RENEG_ESTABLISHED;
        } else if((where&SSL_CB_ACCEPT_LOOP)
                && c->reneg_state==RENEG_ESTABLISHED) {
            int state=SSL_get_state((SSL *)ssl);

            if(state==SSL3_ST_SR_CLNT_HELLO_A
                    || state==SSL23_ST_SR_CLNT_HELLO_A) {
                /* client hello received after initial handshake,
                 * this means renegotiation -> mark it */
                c->reneg_state=RENEG_DETECTED;
            }
        }
    }

    if(global_options.debug_level<LOG_DEBUG)
        return; /* performance optimization */

    if(where & SSL_CB_LOOP) {
        s_log(LOG_DEBUG, "SSL state (%s): %s",
            where & SSL_ST_CONNECT ? "connect" :
            where & SSL_ST_ACCEPT ? "accept" :
            "undefined", SSL_state_string_long(ssl));
    } else if(where & SSL_CB_ALERT) {
        s_log(LOG_DEBUG, "SSL alert (%s): %s: %s",
            where & SSL_CB_READ ? "read" : "write",
            SSL_alert_type_string_long(ret),
            SSL_alert_desc_string_long(ret));
    } else if(where==SSL_CB_HANDSHAKE_DONE) {
        s_log(LOG_DEBUG, "%4ld items in the session cache",
            SSL_CTX_sess_number(ssl->ctx));
        s_log(LOG_DEBUG, "%4ld client connects (SSL_connect())",
            SSL_CTX_sess_connect(ssl->ctx));
        s_log(LOG_DEBUG, "%4ld client connects that finished",
            SSL_CTX_sess_connect_good(ssl->ctx));
        s_log(LOG_DEBUG, "%4ld client renegotiations requested",
            SSL_CTX_sess_connect_renegotiate(ssl->ctx));
        s_log(LOG_DEBUG, "%4ld server connects (SSL_accept())",
            SSL_CTX_sess_accept(ssl->ctx));
        s_log(LOG_DEBUG, "%4ld server connects that finished",
            SSL_CTX_sess_accept_good(ssl->ctx));
        s_log(LOG_DEBUG, "%4ld server renegotiations requested",
            SSL_CTX_sess_accept_renegotiate(ssl->ctx));
        s_log(LOG_DEBUG, "%4ld session cache hits",
            SSL_CTX_sess_hits(ssl->ctx));
        s_log(LOG_DEBUG, "%4ld external session cache hits",
            SSL_CTX_sess_cb_hits(ssl->ctx));
        s_log(LOG_DEBUG, "%4ld session cache misses",
            SSL_CTX_sess_misses(ssl->ctx));
        s_log(LOG_DEBUG, "%4ld session cache timeouts",
            SSL_CTX_sess_timeouts(ssl->ctx));
    }
}
