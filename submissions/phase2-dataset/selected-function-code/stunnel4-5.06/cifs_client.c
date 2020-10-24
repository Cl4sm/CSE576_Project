NOEXPORT char *cifs_client(CLI *c, SERVICE_OPTIONS *opt, const PHASE phase) {
    u8 buffer[5];
    u8 request_dummy[4] = {0x81, 0, 0, 0}; /* a zero-length request */

    (void)opt; /* skip warning about unused parameter */
    if(phase!=PROTOCOL_MIDDLE)
        return NULL;
    s_write(c, c->remote_fd.fd, request_dummy, 4);
    s_read(c, c->remote_fd.fd, buffer, 5);
    if(buffer[0]!=0x83) { /* NB_SSN_NEGRESP */
        s_log(LOG_ERR, "Negative response expected");
        longjmp(c->err, 1);
    }
    if(buffer[2]!=0 || buffer[3]!=1) { /* length != 1 */
        s_log(LOG_ERR, "Unexpected NetBIOS response size");
        longjmp(c->err, 1);
    }
    if(buffer[4]!=0x8e) { /* use SSL */
        s_log(LOG_ERR, "Remote server does not require SSL");
        longjmp(c->err, 1);
    }
    return NULL;
}
