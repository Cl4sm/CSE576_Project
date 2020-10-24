static void
timeout_auth_queries_event(void *notused)
{
  dlink_node *ptr = NULL, *ptr_next = NULL;

  DLINK_FOREACH_SAFE(ptr, ptr_next, auth_pending_list.head)
  {
    struct AuthRequest *auth = ptr->data;

    if (auth->timeout > CurrentTime)
      break;

    if (IsDoingAuth(auth))
    {
      ++ServerStats.is_abad;
      fd_close(&auth->fd);
      ClearAuth(auth);
      sendheader(auth->client, REPORT_FAIL_ID);
    }

    if (IsDNSPending(auth))
    {
      delete_resolver_queries(auth);
      ClearDNSPending(auth);
      sendheader(auth->client, REPORT_FAIL_DNS);
    }

    ilog(LOG_TYPE_IRCD, "DNS/AUTH timeout %s",
         get_client_name(auth->client, SHOW_IP));
    release_auth_client(auth);
  }
}
