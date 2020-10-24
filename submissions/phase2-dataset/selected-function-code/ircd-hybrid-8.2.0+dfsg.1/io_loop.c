io_loop(void)
{
  while (1)
  {
    if (listing_client_list.head)
    {
      dlink_node *ptr = NULL, *ptr_next = NULL;
      DLINK_FOREACH_SAFE(ptr, ptr_next, listing_client_list.head)
        safe_list_channels(ptr->data, 0);
    }

    /* Run pending events */
    event_run();

    comm_select();
    exit_aborted_clients();
    free_exited_clients();

    /* Check to see whether we have to rehash the configuration .. */
    if (dorehash)
    {
      rehash(1);
      dorehash = 0;
    }

    if (doremotd)
    {
      motd_recache();
      sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                           "Got signal SIGUSR1, reloading motd file(s)");
      doremotd = 0;
    }
  }
}
