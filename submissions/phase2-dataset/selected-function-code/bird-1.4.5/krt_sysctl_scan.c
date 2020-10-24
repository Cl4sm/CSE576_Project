krt_sysctl_scan(struct proto *p, int cmd, int table_id)
{
  byte *buf, *next;
  int mib[7], mcnt;
  size_t needed;
  struct ks_msg *m;
  int retries = 3;
  int rv;

  mib[0] = CTL_NET;
  mib[1] = PF_ROUTE;
  mib[2] = 0;
  mib[3] = BIRD_AF;
  mib[4] = cmd;
  mib[5] = 0;
  mcnt = 6;

#ifdef KRT_USE_SYSCTL_7
  if (table_id >= 0)
  {
    mib[6] = table_id;
    mcnt = 7;
  }
#endif

#ifdef KRT_USE_SETFIB_SCAN
  if (table_id > 0)
    if (setfib(table_id) < 0)
    {
      log(L_ERR "KRT: setfib(%d) failed: %m", table_id);
      return;
    }
#endif

 try:
  rv = sysctl(mib, mcnt, NULL, &needed, NULL, 0);
  if (rv < 0)
  {
    /* OpenBSD returns EINVAL for not yet used tables */
    if ((errno == EINVAL) && (table_id > 0))
      goto exit;

    log(L_ERR "KRT: Route scan estimate failed: %m");
    goto exit;
  }

  /* The table is empty */
  if (needed == 0)
    goto exit;

  buf = krt_buffer_update(p, &needed);

  rv = sysctl(mib, mcnt, buf, &needed, NULL, 0);
  if (rv < 0)
  {
    /* The buffer size changed since last sysctl ('needed' is not changed) */
    if ((errno == ENOMEM) && retries--)
      goto try;

    log(L_ERR "KRT: Route scan failed: %m");
    goto exit;
  }

#ifdef KRT_USE_SETFIB_SCAN
  if (table_id > 0)
    if (setfib(0) < 0)
      die("KRT: setfib(%d) failed: %m", 0);
#endif

  /* Process received messages */
  for (next = buf; next < (buf + needed); next += m->rtm.rtm_msglen)
  {
    m = (struct ks_msg *)next;
    krt_read_msg(p, m, 1);
  }

  return;

 exit:
  krt_buffer_release(p);

#ifdef KRT_USE_SETFIB_SCAN
  if (table_id > 0)
    if (setfib(0) < 0)
      die("KRT: setfib(%d) failed: %m", 0);
#endif
}
