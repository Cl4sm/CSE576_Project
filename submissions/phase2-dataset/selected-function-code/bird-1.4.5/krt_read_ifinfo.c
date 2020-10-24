static void
krt_read_ifinfo(struct ks_msg *msg, int scan)
{
  struct if_msghdr *ifm = (struct if_msghdr *)&msg->rtm;
  void *body = (void *)(ifm + 1);
  struct sockaddr_dl *dl = NULL;
  unsigned int i;
  struct iface *iface = NULL, f = {};
  int fl = ifm->ifm_flags;
  int nlen = 0;

  for (i = 1; i<=RTA_IFP; i <<= 1)
  {
    if (i & ifm->ifm_addrs)
    {
      if (i == RTA_IFP)
      {
        dl = (struct sockaddr_dl *)body;
        break;
      }
      body += ROUNDUP(((struct sockaddr *)&(body))->sa_len);
    }
  }

  if (dl && (dl->sdl_family != AF_LINK))
  {
    log(L_WARN "Ignoring strange IFINFO");
    return;
  }

  if (dl)
    nlen = MIN(sizeof(f.name)-1, dl->sdl_nlen);

  /* Note that asynchronous IFINFO messages do not contain iface
     name, so we have to found an existing iface by iface index */

  iface = if_find_by_index(ifm->ifm_index);
  if (!iface)
  {
    /* New interface */
    if (!dl)
      return;	/* No interface name, ignoring */

    memcpy(f.name, dl->sdl_data, nlen);
    DBG("New interface '%s' found\n", f.name);
  }
  else if (dl && memcmp(iface->name, dl->sdl_data, nlen))
  {
    /* Interface renamed */
    if_delete(iface);
    memcpy(f.name, dl->sdl_data, nlen);
  }
  else
  {
    /* Old interface */
    memcpy(f.name, iface->name, sizeof(f.name));
  }

  f.index = ifm->ifm_index;
  f.mtu = ifm->ifm_data.ifi_mtu;

  if (fl & IFF_UP)
    f.flags |= IF_ADMIN_UP;
  if (ifm->ifm_data.ifi_link_state != LINK_STATE_DOWN)
    f.flags |= IF_LINK_UP;          /* up or unknown */
  if (fl & IFF_LOOPBACK)            /* Loopback */
    f.flags |= IF_MULTIACCESS | IF_LOOPBACK | IF_IGNORE;
  else if (fl & IFF_POINTOPOINT)    /* PtP */
    f.flags |= IF_MULTICAST;
  else if (fl & IFF_BROADCAST)      /* Broadcast */
    f.flags |= IF_MULTIACCESS | IF_BROADCAST | IF_MULTICAST;
  else
    f.flags |= IF_MULTIACCESS;      /* NBMA */

  iface = if_update(&f);

  if (!scan)
    if_end_partial_update(iface);
}
