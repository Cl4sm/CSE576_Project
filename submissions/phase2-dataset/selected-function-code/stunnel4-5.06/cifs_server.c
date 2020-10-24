NOEXPORT char *cifs_server(CLI *c, SERVICE_OPTIONS *opt, const PHASE phase) {
    u8 buffer[128];
    u8 response_access_denied[5] = {0x83, 0, 0, 1, 0x81};
    u8 response_use_ssl[5] = {0x83, 0, 0, 1, 0x8e};
    u16 len;

    (void)opt; /* skip warning about unused parameter */
    if(phase!=PROTOCOL_EARLY)
        return NULL;
    s_read(c, c->local_rfd.fd, buffer, 4) ;/* NetBIOS header */
    len=buffer[3];
    len|=(u16)(buffer[2]) << 8;
    if(len>sizeof buffer-4) {
        s_log(LOG_ERR, "Received block too long");
        longjmp(c->err, 1);
    }
    s_read(c, c->local_rfd.fd, buffer+4, len);
    if(buffer[0]!=0x81) { /* NB_SSN_REQUEST */
        s_log(LOG_ERR, "Client did not send session setup");
        s_write(c, c->local_wfd.fd, response_access_denied, 5);
        longjmp(c->err, 1);
    }
    s_write(c, c->local_wfd.fd, response_use_ssl, 5);
    return NULL;
}
