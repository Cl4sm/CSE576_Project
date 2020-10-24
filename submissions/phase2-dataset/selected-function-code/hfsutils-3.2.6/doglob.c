static
int doglob(hfsvol *vol, dlist *list, const char *dir, const char *rem)
{
  dstring new;
  int special, len, result = 0;
  const char *obrace, *cbrace, *ptr;

  dstr_init(&new);

  special = 0;
  obrace = cbrace = 0;

  for (ptr = rem; *ptr && (obrace || *ptr != ':'); ++ptr)
    {
      switch (*ptr)
	{
	case '{':
	  if (obrace == 0)
	    obrace = ptr;
	  break;

	case '}':
	  if (obrace && cbrace == 0)
	    cbrace = ptr;
	  break;

	case '\\':
	  if (*++ptr == 0)
	    --ptr;

	case '*':
	case '[':
	case '?':
	  special = 1;
	  break;
	}
    }

  if (obrace)
    {
      const char *elt;

      if (cbrace == 0 ||
	  dstr_append(&new, rem, obrace - rem) == -1)
	{
	  dstr_free(&new);
	  return -1;
	}
      len = dstr_length(&new);

      for (ptr = obrace; *ptr != '}'; )
	{
	  elt = ptr + 1;

	  ptr = elt;
	  while (*ptr != '}' && *ptr != ',')
	    ++ptr;

	  if (dstr_append(&new, elt, ptr - elt) == -1 ||
	      dstr_append(&new, cbrace + 1, -1) == -1 ||
	      doglob(vol, list, dir, dstr_string(&new)) == -1)
	    {
	      dstr_free(&new);
	      return -1;
	    }

	  dstr_shrink(&new, len);
	}

      dstr_free(&new);
      return 0;
    }

  if (dstr_append(&new, dir, -1) == -1)
    {
      dstr_free(&new);
      return -1;
    }
  len = dstr_length(&new);

  if (special)
    {
      hfsdirent ent;
      hfsdir *d;
      dstring pat;
      int found = 0;

      dstr_init(&pat);
      if (dstr_append(&pat, rem, ptr - rem) == -1)
	{
	  dstr_free(&pat);
	  dstr_free(&new);
	  return -1;
	}

      if (*dir == 0 && strchr(rem, ':') == 0)
	d = hfs_opendir(vol, ":");
      else
	d = hfs_opendir(vol, dir);

      if (d == 0)
	{
	  dstr_free(&pat);
	  dstr_free(&new);
	  return -1;
	}

      while (hfs_readdir(d, &ent) != -1)
	{
	  if (ent.fdflags & HFS_FNDR_ISINVISIBLE)
	    continue;

	  if (strmatch(ent.name, dstr_string(&pat)))
	    {
	      dstr_shrink(&new, len);
	      if (dstr_append(&new, ent.name, -1) == -1)
		{
		  result = -1;
		  break;
		}

	      if (*ptr == 0)
		{
		  found  = 1;
		  result = dl_append(list, dstr_string(&new));

		  if (result == -1)
		    break;
		}
	      else if (ent.flags & HFS_ISDIR)
		{
		  if (dstr_append(&new, ":", 1) == -1)
		    result = -1;
		  else
		    {
		      found  = 1;
		      result = doglob(vol, list, dstr_string(&new), ptr + 1);
		    }

		  if (result == -1)
		    break;
		}
	    }
	}

      hfs_closedir(d);

      if (result == 0 && ! found)
	{
	  dstr_shrink(&new, len);
	  if (dstr_append(&new, rem, -1) == -1)
	    result = -1;
	  else
	    {
	      char *ptr, *rem;

	      for (rem = dstr_string(&new) + len, ptr = rem; *rem;
		   ++rem, ++ptr)
		{
		  if (*rem == '\\')
		    ++rem;

		  *ptr = *rem;
		}
	      *ptr = 0;

	      result = dl_append(list, dstr_string(&new));
	    }
	}

      dstr_free(&pat);
      dstr_free(&new);

      return result;
    }

  if (dstr_append(&new, rem, ptr - rem) == -1)
    result = -1;
  else
    {
      if (*ptr)
	{
	  if (dstr_append(&new, ":", 1) == -1)
	    result = -1;
	  else
	    result = doglob(vol, list, dstr_string(&new), ptr + 1);

	  dstr_free(&new);
	  return result;
	}

      result = dl_append(list, dstr_string(&new));
    }

  dstr_free(&new);
  return result;
}
