bfd_notify_init(struct bfd_proto *p)
{
  int pfds[2];
  sock *sk;

  int rv = pipe(pfds);
  if (rv < 0)
    die("pipe: %m");

  sk = sk_new(p->p.pool);
  sk->type = SK_MAGIC;
  sk->rx_hook = bfd_notify_hook;
  sk->err_hook = bfd_noterr_hook;
  sk->fd = pfds[0];
  sk->data = p;
  if (sk_open(sk) < 0)
    die("bfd: sk_open failed");
  p->notify_rs = sk;

  /* The write sock is not added to any event loop */
  sk = sk_new(p->p.pool);
  sk->type = SK_MAGIC;
  sk->fd = pfds[1];
  sk->data = p;
  sk->flags = SKF_THREAD;
  if (sk_open(sk) < 0)
    die("bfd: sk_open failed");
  p->notify_ws = sk;
}
