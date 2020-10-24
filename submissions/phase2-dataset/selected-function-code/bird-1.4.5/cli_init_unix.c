static void
cli_init_unix(uid_t use_uid, gid_t use_gid)
{
  sock *s;

  cli_init();
  s = cli_sk = sk_new(cli_pool);
  s->type = SK_UNIX_PASSIVE;
  s->rx_hook = cli_connect;
  s->rbsize = 1024;

  /* Return value intentionally ignored */
  unlink(path_control_socket);

  if (sk_open_unix(s, path_control_socket) < 0)
    die("Cannot create control socket %s: %m", path_control_socket);

  if (use_uid || use_gid)
    if (chown(path_control_socket, use_uid, use_gid) < 0)
      die("chown: %m");

  if (chmod(path_control_socket, 0660) < 0)
    die("chmod: %m");
}
