TEXTDOMAIN (domainname)
     const char *domainname;
{
  char *new_domain;
  char *old_domain;

  /* A NULL pointer requests the current setting.  */
  if (domainname == NULL)
    return (char *) _nl_current_default_domain;

  __libc_rwlock_wrlock (_nl_state_lock);

  old_domain = (char *) _nl_current_default_domain;

  /* If domain name is the null string set to default domain "messages".  */
  if (domainname[0] == '\0'
      || strcmp (domainname, _nl_default_default_domain) == 0)
    {
      _nl_current_default_domain = _nl_default_default_domain;
      new_domain = (char *) _nl_current_default_domain;
    }
  else if (strcmp (domainname, old_domain) == 0)
    /* This can happen and people will use it to signal that some
       environment variable changed.  */
    new_domain = old_domain;
  else
    {
      /* If the following malloc fails `_nl_current_default_domain'
	 will be NULL.  This value will be returned and so signals we
	 are out of core.  */
#if defined _LIBC || defined HAVE_STRDUP
      new_domain = strdup (domainname);
#else
      size_t len = strlen (domainname) + 1;
      new_domain = (char *) malloc (len);
      if (new_domain != NULL)
	memcpy (new_domain, domainname, len);
#endif

      if (new_domain != NULL)
	_nl_current_default_domain = new_domain;
    }

  /* We use this possibility to signal a change of the loaded catalogs
     since this is most likely the case and there is no other easy we
     to do it.  Do it only when the call was successful.  */
  if (new_domain != NULL)
    {
      ++_nl_msg_cat_cntr;

      if (old_domain != new_domain && old_domain != _nl_default_default_domain)
	free (old_domain);
    }

  __libc_rwlock_unlock (_nl_state_lock);

  return new_domain;
}
