static void
comm_connect_dns_callback(void *vptr, const struct irc_ssaddr *addr, const char *name, size_t namelength)
{
  fde_t *F = vptr;

  if (!addr)
  {
    comm_connect_callback(F, COMM_ERR_DNS);
    return;
  }

  /* No error, set a 10 second timeout */
  comm_settimeout(F, 30*1000, comm_connect_timeout, NULL);

  /* Copy over the DNS reply info so we can use it in the connect() */
  /*
   * Note we don't fudge the refcount here, because we aren't keeping
   * the DNS record around, and the DNS cache is gone anyway..
   *     -- adrian
   */
  memcpy(&F->connect.hostaddr, addr, addr->ss_len);
  /* The cast is hacky, but safe - port offset is same on v4 and v6 */
  ((struct sockaddr_in *) &F->connect.hostaddr)->sin_port = F->connect.hostaddr.ss_port;
  F->connect.hostaddr.ss_len = addr->ss_len;

  /* Now, call the tryconnect() routine to try a connect() */
  comm_connect_tryconnect(F, NULL);
}
