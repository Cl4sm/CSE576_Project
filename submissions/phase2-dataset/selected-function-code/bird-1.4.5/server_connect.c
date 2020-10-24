server_connect(void)
{
  struct sockaddr_un sa;

  server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (server_fd < 0)
    die("Cannot create socket: %m");

  if (strlen(server_path) >= sizeof(sa.sun_path))
    die("server_connect: path too long");

  bzero(&sa, sizeof(sa));
  sa.sun_family = AF_UNIX;
  strcpy(sa.sun_path, server_path);
  if (connect(server_fd, (struct sockaddr *) &sa, SUN_LEN(&sa)) < 0)
    die("Unable to connect to server control socket (%s): %m", server_path);
  if (fcntl(server_fd, F_SETFL, O_NONBLOCK) < 0)
    die("fcntl: %m");
}
