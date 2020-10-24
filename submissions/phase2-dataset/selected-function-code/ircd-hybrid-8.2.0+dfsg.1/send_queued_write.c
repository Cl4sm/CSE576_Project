send_queued_write(struct Client *to)
{
  int retlen = 0;

  /*
   ** Once socket is marked dead, we cannot start writing to it,
   ** even if the error is removed...
   */
  if (IsDead(to) || HasFlag(to, FLAGS_BLOCKED))
    return;  /* no use calling send() now */

  /* Next, lets try to write some data */
  if (dbuf_length(&to->localClient->buf_sendq))
  {
    do
    {
      struct dbuf_block *first = to->localClient->buf_sendq.blocks.head->data;

#ifdef HAVE_LIBCRYPTO
      if (to->localClient->fd.ssl)
      {
        retlen = SSL_write(to->localClient->fd.ssl, first->data + to->localClient->buf_sendq.pos, first->size - to->localClient->buf_sendq.pos);

        /* translate openssl error codes, sigh */
        if (retlen < 0)
        {
          switch (SSL_get_error(to->localClient->fd.ssl, retlen))
          {
            case SSL_ERROR_WANT_READ:
              return;  /* retry later, don't register for write events */
            case SSL_ERROR_WANT_WRITE:
              errno = EWOULDBLOCK;
            case SSL_ERROR_SYSCALL:
              break;
            case SSL_ERROR_SSL:
              if (errno == EAGAIN)
                break;
            default:
              retlen = errno = 0;  /* either an SSL-specific error or EOF */
          }
        }
      }
      else
#endif
        retlen = send(to->localClient->fd.fd, first->data + to->localClient->buf_sendq.pos, first->size - to->localClient->buf_sendq.pos, 0);

      if (retlen <= 0)
        break;

      dbuf_delete(&to->localClient->buf_sendq, retlen);

      /* We have some data written .. update counters */
      to->localClient->send.bytes += retlen;
      me.localClient->send.bytes += retlen;
    } while (dbuf_length(&to->localClient->buf_sendq));

    if (retlen < 0 && ignoreErrno(errno))
    {
      AddFlag(to, FLAGS_BLOCKED);

      /* we have a non-fatal error, reschedule a write */
      comm_setselect(&to->localClient->fd, COMM_SELECT_WRITE,
                     sendq_unblocked, to, 0);
    }
    else if (retlen <= 0)
    {
      dead_link_on_write(to, errno);
      return;
    }
  }
}
