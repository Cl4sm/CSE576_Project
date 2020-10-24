split_nuh(struct split_nuh_item *const iptr)
{
  char *p = NULL, *q = NULL;

  if (iptr->nickptr)
    strlcpy(iptr->nickptr, "*", iptr->nicksize);
  if (iptr->userptr)
    strlcpy(iptr->userptr, "*", iptr->usersize);
  if (iptr->hostptr)
    strlcpy(iptr->hostptr, "*", iptr->hostsize);

  if ((p = strchr(iptr->nuhmask, '!')))
  {
    *p = '\0';

    if (iptr->nickptr && *iptr->nuhmask)
      strlcpy(iptr->nickptr, iptr->nuhmask, iptr->nicksize);

    if ((q = strchr(++p, '@')))
    {
      *q++ = '\0';

      if (*p)
        strlcpy(iptr->userptr, p, iptr->usersize);

      if (*q)
        strlcpy(iptr->hostptr, q, iptr->hostsize);
    }
    else
    {
      if (*p)
        strlcpy(iptr->userptr, p, iptr->usersize);
    }
  }
  else
  {
    /* No ! found so lets look for a user@host */
    if ((p = strchr(iptr->nuhmask, '@')))
    {
      /* if found a @ */
      *p++ = '\0';

      if (*iptr->nuhmask)
        strlcpy(iptr->userptr, iptr->nuhmask, iptr->usersize);

      if (*p)
        strlcpy(iptr->hostptr, p, iptr->hostsize);
    }
    else
    {
      /* no @ found */
      if (!iptr->nickptr || strpbrk(iptr->nuhmask, ".:"))
        strlcpy(iptr->hostptr, iptr->nuhmask, iptr->hostsize);
      else
        strlcpy(iptr->nickptr, iptr->nuhmask, iptr->nicksize);
    }
  }
}
