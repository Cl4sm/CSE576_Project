void
show_lusers(struct Client *source_p)
{
  if (!ConfigServerHide.hide_servers || HasUMode(source_p, UMODE_OPER))
    sendto_one_numeric(source_p, &me, RPL_LUSERCLIENT, (Count.total-Count.invisi),
                       Count.invisi, dlink_list_length(&global_server_list));
  else
    sendto_one_numeric(source_p, &me, RPL_LUSERCLIENT,
                       (Count.total - Count.invisi), Count.invisi, 1);

  if (Count.oper)
    sendto_one_numeric(source_p, &me, RPL_LUSEROP, Count.oper);

  if (dlink_list_length(&unknown_list))
    sendto_one_numeric(source_p, &me, RPL_LUSERUNKNOWN, dlink_list_length(&unknown_list));

  if (dlink_list_length(&channel_list))
    sendto_one_numeric(source_p, &me, RPL_LUSERCHANNELS, dlink_list_length(&channel_list));

  if (!ConfigServerHide.hide_servers || HasUMode(source_p, UMODE_OPER))
  {
    sendto_one_numeric(source_p, &me, RPL_LUSERME, Count.local, Count.myserver);
    sendto_one_numeric(source_p, &me, RPL_LOCALUSERS, Count.local, Count.max_loc);
  }
  else
  {
    sendto_one_numeric(source_p, &me, RPL_LUSERME, Count.total, 0);
    sendto_one_numeric(source_p, &me, RPL_LOCALUSERS, Count.total, Count.max_tot);
  }

  sendto_one_numeric(source_p, &me, RPL_GLOBALUSERS, Count.total, Count.max_tot);

  if (!ConfigServerHide.hide_servers || HasUMode(source_p, UMODE_OPER))
    sendto_one_numeric(source_p, &me, RPL_STATSCONN, Count.max_loc_con,
                       Count.max_loc_cli, Count.totalrestartcount);

  if (Count.local > Count.max_loc_cli)
    Count.max_loc_cli = Count.local;

  if ((Count.local + Count.myserver) > Count.max_loc_con)
    Count.max_loc_con = Count.local + Count.myserver;
}
