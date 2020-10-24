read_auth_reply(fde_t *fd, void *data)
{
  struct AuthRequest *auth = data;
  const char *username = NULL;
  ssize_t len = 0;
  char buf[RFC1413_BUFSIZ + 1];

  if ((len = recv(fd->fd, buf, RFC1413_BUFSIZ, 0)) > 0)
  {
    buf[len] = '\0';
    username = check_ident_reply(buf);
  }

  fd_close(fd);

  ClearAuth(auth);

  if (EmptyString(username))
  {
    sendheader(auth->client, REPORT_FAIL_ID);
    ++ServerStats.is_abad;
  }
  else
  {
    strlcpy(auth->client->username, username, sizeof(auth->client->username));
    sendheader(auth->client, REPORT_FIN_ID);
    ++ServerStats.is_asuc;
    SetGotId(auth->client);
  }

  release_auth_client(auth);
}
