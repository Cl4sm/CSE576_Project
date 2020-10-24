static int
proc_answer(struct reslist *request, HEADER *header, unsigned char *buf, unsigned char *eob)
{
  char hostbuf[RFC1035_MAX_DOMAIN_LENGTH + 100]; /* working buffer */
  unsigned char *current = buf + sizeof(HEADER); /* current position in buf */
  unsigned int type = 0;       /* answer type */
  unsigned int rd_length = 0;
  int n;                       /* temp count */
  struct sockaddr_in *v4;      /* conversion */
  struct sockaddr_in6 *v6;

  for (; header->qdcount > 0; --header->qdcount)
  {
    if ((n = irc_dn_skipname(current, eob)) < 0)
      break;

    current += (size_t)n + QFIXEDSZ;
  }

  /*
   * Process each answer sent to us blech.
   */
  while (header->ancount > 0 && current < eob)
  {
    header->ancount--;

    n = irc_dn_expand(buf, eob, current, hostbuf, sizeof(hostbuf));

    if (n < 0  /* Broken message */ || n == 0  /* No more answers left */)
      return 0;

    hostbuf[RFC1035_MAX_DOMAIN_LENGTH] = '\0';

    /*
     * With Address arithmetic you have to be very anal
     * this code was not working on alpha due to that
     * (spotted by rodder/jailbird/dianora)
     */
    current += (size_t)n;

    if (!((current + ANSWER_FIXED_SIZE) < eob))
      break;

    type = irc_ns_get16(current);
    current += TYPE_SIZE;
    current += CLASS_SIZE;
    current += TTL_SIZE;
    rd_length = irc_ns_get16(current);
    current += RDLENGTH_SIZE;

    /*
     * Wait to set request->type until we verify this structure
     */
    switch (type)
    {
      case T_A:
        if (request->type != T_A)
          return 0;

        /*
         * Check for invalid rd_length or too many addresses
         */
        if (rd_length != sizeof(struct in_addr))
          return 0;

        request->addr.ss_len = sizeof(struct sockaddr_in);
        v4 = (struct sockaddr_in *)&request->addr;
        v4->sin_family = AF_INET;
        memcpy(&v4->sin_addr, current, sizeof(struct in_addr));
        return 1;
        break;
      case T_AAAA:
        if (request->type != T_AAAA)
          return 0;

        if (rd_length != sizeof(struct in6_addr))
          return 0;

        request->addr.ss_len = sizeof(struct sockaddr_in6);
        v6 = (struct sockaddr_in6 *)&request->addr;
        v6->sin6_family = AF_INET6;
        memcpy(&v6->sin6_addr, current, sizeof(struct in6_addr));
        return 1;
        break;
      case T_PTR:
        if (request->type != T_PTR)
          return 0;

        n = irc_dn_expand(buf, eob, current, hostbuf, sizeof(hostbuf));
        if (n < 0  /* Broken message */ || n == 0  /* No more answers left */)
          return 0;

        request->namelength = strlcpy(request->name, hostbuf, sizeof(request->name));
        return 1;
        break;
      case T_CNAME:
        current += rd_length;
        break;
      default:
        return 0;
        break;
    }
  }

  return 0;
}
