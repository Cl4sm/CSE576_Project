query_name(const char *name, int query_class, int type, struct reslist *request)
{
  unsigned char buf[MAXPACKET];
  int request_len = 0;

  memset(buf, 0, sizeof(buf));

  if ((request_len = irc_res_mkquery(name, query_class, type, buf, sizeof(buf))) > 0)
  {
    HEADER *header = (HEADER *)buf;

    /*
     * Generate an unique id.
     * NOTE: we don't have to worry about converting this to and from
     * network byte order, the nameserver does not interpret this value
     * and returns it unchanged.
     */
    do
      header->id = (header->id + genrand_int32()) & 0xFFFF;
    while (find_id(header->id));

    request->id = header->id;
    ++request->sends;

    request->sent += send_res_msg(buf, request_len, request->sends);
  }
}
