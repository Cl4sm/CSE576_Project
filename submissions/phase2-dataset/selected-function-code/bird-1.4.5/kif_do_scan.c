kif_do_scan(struct kif_proto *p UNUSED)
{
  struct nlmsghdr *h;

  if_start_update();

  nl_request_dump(RTM_GETLINK);
  while (h = nl_get_scan())
    if (h->nlmsg_type == RTM_NEWLINK || h->nlmsg_type == RTM_DELLINK)
      nl_parse_link(h, 1);
    else
      log(L_DEBUG "nl_scan_ifaces: Unknown packet received (type=%d)", h->nlmsg_type);

  nl_request_dump(RTM_GETADDR);
  while (h = nl_get_scan())
    if (h->nlmsg_type == RTM_NEWADDR || h->nlmsg_type == RTM_DELADDR)
      nl_parse_addr(h, 1);
    else
      log(L_DEBUG "nl_scan_ifaces: Unknown packet received (type=%d)", h->nlmsg_type);

  if_end_update();
}
