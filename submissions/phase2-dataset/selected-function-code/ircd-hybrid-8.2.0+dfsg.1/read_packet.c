void
read_packet(fde_t *fd, void *data)
{
  struct Client *client_p = data;
  int length = 0;

  if (IsDefunct(client_p))
    return;

  /*
   * Read some data. We *used to* do anti-flood protection here, but
   * I personally think it makes the code too hairy to make sane.
   *     -- adrian
   */
  do
  {
#ifdef HAVE_LIBCRYPTO
    if (fd->ssl)
    {
      length = SSL_read(fd->ssl, readBuf, sizeof(readBuf));

      /* translate openssl error codes, sigh */
      if (length < 0)
        switch (SSL_get_error(fd->ssl, length))
        {
          case SSL_ERROR_WANT_WRITE:
            comm_setselect(fd, COMM_SELECT_WRITE, sendq_unblocked, client_p, 0);
            return;
          case SSL_ERROR_WANT_READ:
              errno = EWOULDBLOCK;
          case SSL_ERROR_SYSCALL:
              break;
          case SSL_ERROR_SSL:
            if (errno == EAGAIN)
              break;
          default:
            length = errno = 0;
        }
    }
    else
#endif
    {
      length = recv(fd->fd, readBuf, sizeof(readBuf), 0);
    }

    if (length <= 0)
    {
      /*
       * If true, then we can recover from this error.  Just jump out of
       * the loop and re-register a new io-request.
       */
      if (length < 0 && ignoreErrno(errno))
        break;

      dead_link_on_read(client_p, length);
      return;
    }

    dbuf_put(&client_p->localClient->buf_recvq, readBuf, length);

    if (client_p->localClient->lasttime < CurrentTime)
      client_p->localClient->lasttime = CurrentTime;
    if (client_p->localClient->lasttime > client_p->localClient->since)
      client_p->localClient->since = CurrentTime;

    ClearPingSent(client_p);

    /* Attempt to parse what we have */
    parse_client_queued(client_p);

    if (IsDefunct(client_p))
      return;

    /* Check to make sure we're not flooding */
    if (!(IsServer(client_p) || IsHandshake(client_p) || IsConnecting(client_p))
        && (dbuf_length(&client_p->localClient->buf_recvq) >
            get_recvq(&client_p->localClient->confs)))
    {
      if (!(ConfigGeneral.no_oper_flood && HasUMode(client_p, UMODE_OPER)))
      {
        exit_client(client_p, "Excess Flood");
        return;
      }
    }
  }
#ifdef HAVE_LIBCRYPTO
  while (length == sizeof(readBuf) || fd->ssl);
#else
  while (length == sizeof(readBuf));
#endif

  /* If we get here, we need to register for another COMM_SELECT_READ */
  comm_setselect(fd, COMM_SELECT_READ, read_packet, client_p, 0);
}
