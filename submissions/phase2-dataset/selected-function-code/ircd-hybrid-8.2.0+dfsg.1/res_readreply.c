res_readreply(fde_t *fd, void *data)
{
  unsigned char buf[sizeof(HEADER) + MAXPACKET];
  HEADER *header;
  struct reslist *request = NULL;
  ssize_t rc = 0;
  socklen_t len = sizeof(struct irc_ssaddr);
  struct irc_ssaddr lsin;

  while ((rc = recvfrom(fd->fd, buf, sizeof(buf), 0, (struct sockaddr *)&lsin, &len)) != -1)
  {
    if (rc <= (ssize_t)sizeof(HEADER))
      continue;

    /*
     * Check against possibly fake replies
     */
    if (!res_ourserver(&lsin))
      continue;

    /*
     * Convert DNS reply reader from Network byte order to CPU byte order.
     */
    header = (HEADER *)buf;
    header->ancount = ntohs(header->ancount);
    header->qdcount = ntohs(header->qdcount);
    header->nscount = ntohs(header->nscount);
    header->arcount = ntohs(header->arcount);

    /*
     * Response for an id which we have already received an answer for
     * just ignore this response.
     */
    if ((request = find_id(header->id)) == NULL)
      continue;

    if (header->rcode != NO_ERRORS || header->ancount == 0)
    {
      /*
       * If a bad error was returned, stop here and don't
       * send any more (no retries granted).
       */
      (*request->callback)(request->callback_ctx, NULL, NULL, 0);
      rem_request(request);
      continue;
    }

    /*
     * If this fails there was an error decoding the received packet.
     * We only give it one shot. If it fails, just leave the client
     * unresolved.
     */
    if (!proc_answer(request, header, buf, buf + rc))
    {
      (*request->callback)(request->callback_ctx, NULL, NULL, 0);
      rem_request(request);
      continue;
    }

    if (request->type == T_PTR)
    {
      if (request->namelength == 0)
      {
        /*
         * Got a PTR response with no name, something bogus is happening
         * don't bother trying again, the client address doesn't resolve
         */
        (*request->callback)(request->callback_ctx, NULL, NULL, 0);
        rem_request(request);
        continue;
      }

      /*
       * Lookup the 'authoritative' name that we were given for the ip#.
       */
      if (request->addr.ss.ss_family == AF_INET6)
        gethost_byname_type(request->callback, request->callback_ctx, request->name, T_AAAA);
      else
        gethost_byname_type(request->callback, request->callback_ctx, request->name, T_A);
      rem_request(request);
    }
    else
    {
      /*
       * Got a name and address response, client resolved
       */
      (*request->callback)(request->callback_ctx, &request->addr, request->name, request->namelength);
      rem_request(request);
    }

    continue;
  }

  comm_setselect(fd, COMM_SELECT_READ, res_readreply, NULL, 0);
}
