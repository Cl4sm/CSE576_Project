void
flood_recalc(fde_t *fd, void *data)
{
  struct Client *client_p = data;
  struct LocalUser *lclient_p = client_p->localClient;

  /* allow a bursting client their allocation per second, allow
   * a client whos flooding an extra 2 per second
   */
  if (IsFloodDone(client_p))
    lclient_p->sent_parsed -= 2;
  else
    lclient_p->sent_parsed = 0;

  if (lclient_p->sent_parsed < 0)
    lclient_p->sent_parsed = 0;

  parse_client_queued(client_p);

  /* And now, try flushing .. */
  if (!IsDead(client_p))
  {
    /* and finally, reset the flood check */
    comm_setflush(fd, 1000, flood_recalc, client_p);
  }
}
