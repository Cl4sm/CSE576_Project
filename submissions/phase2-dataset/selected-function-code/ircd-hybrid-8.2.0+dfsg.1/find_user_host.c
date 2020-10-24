static int
find_user_host(struct Client *source_p, char *user_host_or_nick,
               char *luser, char *lhost, unsigned int flags)
{
  struct Client *target_p = NULL;
  char *hostp = NULL;

  if (lhost == NULL)
  {
    strlcpy(luser, user_host_or_nick, USERLEN*4 + 1);
    return 1;
  }

  if ((hostp = strchr(user_host_or_nick, '@')) || *user_host_or_nick == '*')
  {
    /* Explicit user@host mask given */

    if (hostp != NULL)                            /* I'm a little user@host */
    {
      *(hostp++) = '\0';                       /* short and squat */
      if (*user_host_or_nick)
        strlcpy(luser, user_host_or_nick, USERLEN*4 + 1); /* here is my user */
      else
        strcpy(luser, "*");

      if (*hostp)
        strlcpy(lhost, hostp, HOSTLEN + 1);    /* here is my host */
      else
        strcpy(lhost, "*");
    }
    else
    {
      luser[0] = '*';             /* no @ found, assume its *@somehost */
      luser[1] = '\0';
      strlcpy(lhost, user_host_or_nick, HOSTLEN*4 + 1);
    }

    return 1;
  }
  else
  {
    /* Try to find user@host mask from nick */
    /* Okay to use source_p as the first param, because source_p == client_p */
    if ((target_p =
        find_chasing(source_p, user_host_or_nick)) == NULL)
      return 0;  /* find_chasing sends ERR_NOSUCHNICK */

    if (IsExemptKline(target_p))
    {
      if (!IsServer(source_p))
        sendto_one_notice(source_p, &me, ":%s is E-lined", target_p->name);
      return 0;
    }

    /*
     * turn the "user" bit into "*user", blow away '~'
     * if found in original user name (non-idented)
     */
    strlcpy(luser, target_p->username, USERLEN*4 + 1);

    if (target_p->username[0] == '~')
      luser[0] = '*';

    if (target_p->sockhost[0] == '\0' ||
        (target_p->sockhost[0] == '0' && target_p->sockhost[1] == '\0'))
      strlcpy(lhost, target_p->host, HOSTLEN*4 + 1);
    else
      strlcpy(lhost, target_p->sockhost, HOSTLEN*4 + 1);
    return 1;
  }

  return 0;
}
