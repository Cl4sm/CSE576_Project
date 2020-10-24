void
check_splitmode(void *unused)
{
  if (splitchecking && (ConfigChannel.no_join_on_split ||
                        ConfigChannel.no_create_on_split))
  {
    const unsigned int server = dlink_list_length(&global_server_list);

    if (!splitmode && ((server < split_servers) || (Count.total < split_users)))
    {
      splitmode = 1;

      sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                           "Network split, activating splitmode");
      event_add(&splitmode_event, NULL); 
    }
    else if (splitmode && (server >= split_servers) && (Count.total >= split_users))
    {
      splitmode = 0;

      sendto_realops_flags(UMODE_ALL, L_ALL, SEND_NOTICE,
                           "Network rejoined, deactivating splitmode");
      event_delete(&splitmode_event);
    }
  }
}
