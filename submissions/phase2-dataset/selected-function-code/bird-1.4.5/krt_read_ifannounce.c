krt_read_ifannounce(struct ks_msg *msg)
{
  struct if_announcemsghdr *ifam = (struct if_announcemsghdr *)&msg->rtm;

  if (ifam->ifan_what == IFAN_ARRIVAL)
  {
    /* Not enough info to create the iface, so we just trigger iface scan */
    kif_request_scan();
  }
  else if (ifam->ifan_what == IFAN_DEPARTURE)
  {
    struct iface *iface = if_find_by_index(ifam->ifan_index);

    /* Interface is destroyed */
    if (!iface)
    {
      DBG("KRT: unknown interface (%s, #%d) going down. Ignoring\n", ifam->ifan_name, ifam->ifan_index);
      return;
    }

    if_delete(iface);
  }

  DBG("KRT: IFANNOUNCE what: %d index %d name %s\n", ifam->ifan_what, ifam->ifan_index, ifam->ifan_name);
}
