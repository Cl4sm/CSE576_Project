send_members(struct Client *client_p, struct Channel *chptr,
             char *modebuf, char *parabuf)
{
  char buf[IRCD_BUFSIZE] = "";
  const dlink_node *ptr = NULL;
  int tlen;              /* length of text to append */
  char *t, *start;       /* temp char pointer */

  start = t = buf + snprintf(buf, sizeof(buf), ":%s SJOIN %lu %s %s %s:",
                             me.id, (unsigned long)chptr->channelts,
                             chptr->chname, modebuf, parabuf);

  DLINK_FOREACH(ptr, chptr->members.head)
  {
    const struct Membership *ms = ptr->data;

    tlen = strlen(ms->client_p->id) + 1;  /* +1 for space */

    if (ms->flags & CHFL_CHANOP)
      ++tlen;
    if (ms->flags & CHFL_HALFOP)
      ++tlen;
    if (ms->flags & CHFL_VOICE)
      ++tlen;

    /*
     * Space will be converted into CR, but we also need space for LF..
     * That's why we use '- 1' here -adx
     */
    if (t + tlen - buf > IRCD_BUFSIZE - 1)
    {
      *(t - 1) = '\0';  /* Kill the space and terminate the string */
      sendto_one(client_p, "%s", buf);
      t = start;
    }

    if (ms->flags & CHFL_CHANOP)
      *t++ = '@';
    if (ms->flags & CHFL_HALFOP)
      *t++ = '%';
    if (ms->flags & CHFL_VOICE)
      *t++ = '+';

    strcpy(t, ms->client_p->id);

    t += strlen(t);
    *t++ = ' ';
  }

  /* Should always be non-NULL unless we have a kind of persistent channels */
  if (chptr->members.head)
    t--;  /* Take the space out */
  *t = '\0';
  sendto_one(client_p, "%s", buf);
}
