send_mode_list(struct Client *client_p, struct Channel *chptr,
               const dlink_list *list, const char flag)
{
  const dlink_node *ptr = NULL;
  char mbuf[IRCD_BUFSIZE] = "";
  char pbuf[IRCD_BUFSIZE] = "";
  int tlen, mlen, cur_len;
  char *pp = pbuf;

  if (list->length == 0)
    return;

  mlen = snprintf(mbuf, sizeof(mbuf), ":%s BMASK %lu %s %c :", me.id,
                  (unsigned long)chptr->channelts, chptr->chname, flag);
  cur_len = mlen;

  DLINK_FOREACH(ptr, list->head)
  {
    const struct Ban *banptr = ptr->data;

    tlen = banptr->len + 3;  /* +3 for ! + @ + space */

    /*
     * Send buffer and start over if we cannot fit another ban
     */
    if (cur_len + (tlen - 1) > IRCD_BUFSIZE - 2)
    {
      *(pp - 1) = '\0';  /* Get rid of trailing space on buffer */
      sendto_one(client_p, "%s%s", mbuf, pbuf);

      cur_len = mlen;
      pp = pbuf;
    }

    pp += sprintf(pp, "%s!%s@%s ", banptr->name, banptr->user,
                  banptr->host);
    cur_len += tlen;
  }

  *(pp - 1) = '\0';  /* Get rid of trailing space on buffer */
  sendto_one(client_p, "%s%s", mbuf, pbuf);
}
