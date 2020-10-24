void
release_auth_client(struct AuthRequest *auth)
{
  struct Client *client = auth->client;

  if (IsDoingAuth(auth) || IsDNSPending(auth))
    return;

  if (IsInAuth(auth))
  {
    dlinkDelete(&auth->node, &auth_pending_list);
    ClearInAuth(auth);
  }

  /*
   * When a client has auth'ed, we want to start reading what it sends
   * us. This is what read_packet() does.
   *     -- adrian
   */
  client->localClient->allow_read = MAX_FLOOD;
  comm_setflush(&client->localClient->fd, 1000, flood_recalc, client);

  client->localClient->since     = CurrentTime;
  client->localClient->lasttime  = CurrentTime;
  client->localClient->firsttime = CurrentTime;
  client->flags |= FLAGS_FINISHED_AUTH;

  read_packet(&client->localClient->fd, client);
}
