sk_free(resource *r)
{
  sock *s = (sock *) r;

  sk_free_bufs(s);
  if (s->fd >= 0)
  {
    close(s->fd);

    /* FIXME: we should call sk_stop() for SKF_THREAD sockets */
    if (s->flags & SKF_THREAD)
      return;

    if (s == current_sock)
      current_sock = sk_next(s);
    if (s == stored_sock)
      stored_sock = sk_next(s);
    rem_node(&s->n);
    sock_recalc_fdsets_p = 1;
  }
}
