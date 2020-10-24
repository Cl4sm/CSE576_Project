void
server_die(const char *message, int rboot)
{
  char buffer[IRCD_BUFSIZE] = "";
  dlink_node *ptr = NULL;
  static int was_here = 0;

  if (rboot && was_here++)
    abort();

  if (EmptyString(message))
    snprintf(buffer, sizeof(buffer), "Server %s",
             rboot ? "Restarting" : "Terminating");
  else
    snprintf(buffer, sizeof(buffer), "Server %s: %s",
             rboot ? "Restarting" : "Terminating", message);

  DLINK_FOREACH(ptr, local_client_list.head)
    sendto_one_notice(ptr->data, &me, ":%s", buffer);

  sendto_server(NULL, NOCAPS, NOCAPS, ":%s ERROR :%s", me.id, buffer);

  ilog(LOG_TYPE_IRCD, "%s", buffer);

  save_all_databases(NULL);

  send_queued_all();
  close_fds(NULL);

  unlink(pidFileName);

  if (rboot)
  {
    execv(SPATH, myargv);
    exit(1);
  }
  else
    exit(0);
}
