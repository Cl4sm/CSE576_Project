void
exit_aborted_clients(void)
{
  dlink_node *ptr;
  struct Client *target_p;
  const char *notice;

  DLINK_FOREACH_SAFE(ptr, eac_next, abort_list.head)
  {
    target_p = ptr->data;
    eac_next = ptr->next;

    if (target_p == NULL)
    {
      sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                           "Warning: null client on abort_list!");
      dlinkDelete(ptr, &abort_list);
      free_dlink_node(ptr);
      continue;
    }

    dlinkDelete(ptr, &abort_list);

    if (IsSendQExceeded(target_p))
      notice = "Max SendQ exceeded";
    else
      notice = "Write error: connection closed";

    exit_client(target_p, notice);
    free_dlink_node(ptr);
  }
}
