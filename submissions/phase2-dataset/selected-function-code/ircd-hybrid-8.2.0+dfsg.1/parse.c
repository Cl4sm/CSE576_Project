parse(struct Client *client_p, char *pbuffer, char *bufend)
{
  struct Client *from = client_p;
  struct Message *msg_ptr = NULL;
  char *para[MAXPARA + 2];  /* <command> + <parameters> + NULL */
  char *ch = NULL;
  char *s = NULL;
  unsigned int numeric = 0;
  unsigned int parc = 0;
  unsigned int paramcount;

  if (IsDefunct(client_p))
    return;

  assert(client_p->localClient->fd.flags.open);
  assert((bufend - pbuffer) < IRCD_BUFSIZE);

  for (ch = pbuffer; *ch == ' '; ++ch)  /* Skip spaces */
    ;

  if (*ch == ':')
  {
    /*
     * Copy the prefix to 'sender' assuming it terminates
     * with SPACE (or NULL, which is an error, though).
     */
    const char *sender = ++ch;

    if ((s = strchr(ch, ' ')))
    {
      *s = '\0';
      ch = ++s;
    }

    if (*sender && IsServer(client_p))
    {
      if ((from = hash_find_id(sender)) == NULL)
        from = hash_find_client(sender);

      /*
       * Hmm! If the client corresponding to the prefix is not found--what is
       * the correct action??? Now, I will ignore the message (old IRC just
       * let it through as if the prefix just wasn't there...) --msa
       */
      if (from == NULL)
      {
        ++ServerStats.is_unpf;
        parse_remove_unknown(client_p, sender, pbuffer);
        return;
      }

      if (from->from != client_p)
      {
        ++ServerStats.is_wrdi;
        parse_cancel_clients(client_p, from, pbuffer);
        return;
      }
    }

    while (*ch == ' ')
      ++ch;
  }

  if (*ch == '\0')
  {
    ++ServerStats.is_empt;
    return;
  }

  /*
   * Extract the command code from the packet. Point s to the end
   * of the command code and calculate the length using pointer
   * arithmetic. Note: only need length for numerics and *all*
   * numerics must have parameters and thus a space after the command
   * code. -avalon
   */

  /* EOB is 3 characters long but is not a numeric */
  if (*(ch + 3) == ' ' &&  /* Ok, let's see if it's a possible numeric */
      IsDigit(*ch) && IsDigit(*(ch + 1)) && IsDigit(*(ch + 2)))
  {
    numeric = (*ch - '0') * 100 + (*(ch + 1) - '0') * 10 + (*(ch + 2) - '0');
    paramcount = 2;  /* Destination, and the rest of it */
    ++ServerStats.is_num;
    s = ch + 3;  /* I know this is ' ' from above if */
    *s++ = '\0';  /* Blow away the ' ', and point s to next part */
  }
  else
  {
    unsigned int ii = 0;

    if ((s = strchr(ch, ' ')))
      *s++ = '\0';

    if ((msg_ptr = find_command(ch)) == NULL)
    {
      /*
       * Note: Give error message *only* to recognized
       * persons. It's a nightmare situation to have
       * two programs sending "Unknown command"'s or
       * equivalent to each other at full blast....
       * If it has got to person state, it at least
       * seems to be well behaving. Perhaps this message
       * should never be generated, though...  --msa
       * Hm, when is the buffer empty -- if a command
       * code has been found ?? -Armin
       */
      if (*pbuffer)
        if (IsClient(from))
          sendto_one_numeric(from, &me, ERR_UNKNOWNCOMMAND, ch);

      ++ServerStats.is_unco;
      return;
    }

    assert(msg_ptr->cmd);

    paramcount = msg_ptr->args_max;
    ii = bufend - ((s) ? s : ch);
    msg_ptr->bytes += ii;
  }

  /*
   * Must the following loop really be so devious? On surface it
   * splits the message to parameters from blank spaces. But, if
   * paramcount has been reached, the rest of the message goes into
   * this last parameter (about same effect as ":" has...) --msa
   */

  /* Note initially true: s == NULL || *(s - 1) == '\0' !! */

  para[parc] = ch;

  if (msg_ptr && (msg_ptr->flags & MFLG_EXTRA))
  {
    /*
     * XXX: This will have to go away after the command handler rewrite
     */
    para[++parc] = msg_ptr->extra;
  }

  if (s)
  {
    if (paramcount > MAXPARA)
      paramcount = MAXPARA;

    while (1)
    {
       while (*s == ' ')
         *s++ = '\0';

       if (*s == '\0')
         break;

       if (*s == ':')
       {
         /* The rest is a single parameter */
         para[++parc] = s + (!numeric);  /* Keep the colon if it's a numeric */
         break;
       }

       para[++parc] = s;

       if (parc >= paramcount)
         break;

       while (*s && *s != ' ')
         ++s;
    }
  }

  para[++parc] = NULL;

  if (msg_ptr)
    parse_handle_command(msg_ptr, from, parc, para);
  else
    parse_handle_numeric(numeric, from, parc, para);
}
