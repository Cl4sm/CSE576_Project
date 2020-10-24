static int
send_res_msg(const unsigned char *msg, int len, unsigned int rcount)
{
  int sent = 0;
  unsigned int max_queries = IRCD_MIN(irc_nscount, rcount);

  /* RES_PRIMARY option is not implemented
   * if (res.options & RES_PRIMARY || 0 == max_queries)
   */
  if (max_queries == 0)
    max_queries = 1;

  for (unsigned int i = 0; i < max_queries; ++i)
  {
    if (sendto(ResolverFileDescriptor.fd, msg, len, 0,
        (struct sockaddr*)&(irc_nsaddr_list[i]),
        irc_nsaddr_list[i].ss_len) == len)
      ++sent;
  }

  return sent;
}
