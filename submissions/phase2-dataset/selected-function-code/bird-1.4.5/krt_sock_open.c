static sock *
krt_sock_open(pool *pool, void *data, int table_id)
{
  sock *sk;
  int fd;

  fd = socket(PF_ROUTE, SOCK_RAW, AF_UNSPEC);
  if (fd < 0)
    die("Cannot open kernel socket for routes");

#ifdef KRT_USE_SETFIB_SOCK
  if (table_id > 0)
  {
    if (setsockopt(fd, SOL_SOCKET, SO_SETFIB, &table_id, sizeof(table_id)) < 0)
      die("Cannot set FIB %d for kernel socket: %m", table_id);
  }
#endif

  sk = sk_new(pool);
  sk->type = SK_MAGIC;
  sk->rx_hook = krt_sock_hook;
  sk->fd = fd;
  sk->data = data;

  if (sk_open(sk) < 0)
    bug("krt-sock: sk_open failed");

  return sk;
}
