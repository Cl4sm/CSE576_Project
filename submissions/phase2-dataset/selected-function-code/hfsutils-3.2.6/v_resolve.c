	      CatDataRec *data, long *parid, char *fname, node *np)
{
  unsigned long dirid;
  char name[HFS_MAX_FLEN + 1], *nptr;
  int found = 0;

  if (*path == 0)
    ERROR(ENOENT, "empty path");

  if (parid)
    *parid = 0;

  nptr = strchr(path, ':');

  if (*path == ':' || nptr == 0)
    {
      dirid = (*vol)->cwd;  /* relative path */

      if (*path == ':')
	++path;

      if (*path == 0)
	{
	  found = v_getdthread(*vol, dirid, data, 0);
	  if (found == -1)
	    goto fail;

	  if (found)
	    {
	      if (parid)
		*parid = data->u.dthd.thdParID;

	      found = v_catsearch(*vol, data->u.dthd.thdParID,
				  data->u.dthd.thdCName, data, fname, np);
	      if (found == -1)
		goto fail;
	    }

	  goto done;
	}
    }
  else
    {
      hfsvol *check;

      dirid = HFS_CNID_ROOTPAR;  /* absolute path */

      if (nptr - path > HFS_MAX_VLEN)
	ERROR(ENAMETOOLONG, 0);

      strncpy(name, path, nptr - path);
      name[nptr - path] = 0;

      for (check = hfs_mounts; check; check = check->next)
	{
	  if (d_relstring(check->mdb.drVN, name) == 0)
	    {
	      *vol = check;
	      break;
	    }
	}
    }

  while (1)
    {
      while (*path == ':')
	{
	  ++path;

	  found = v_getdthread(*vol, dirid, data, 0);
	  if (found == -1)
	    goto fail;
	  else if (! found)
	    goto done;

	  dirid = data->u.dthd.thdParID;
	}

      if (*path == 0)
	{
	  found = v_getdthread(*vol, dirid, data, 0);
	  if (found == -1)
	    goto fail;

	  if (found)
	    {
	      if (parid)
		*parid = data->u.dthd.thdParID;

	      found = v_catsearch(*vol, data->u.dthd.thdParID,
				  data->u.dthd.thdCName, data, fname, np);
	      if (found == -1)
		goto fail;
	    }

	  goto done;
	}

      nptr = name;
      while (nptr < name + sizeof(name) - 1 && *path && *path != ':')
	*nptr++ = *path++;

      if (*path && *path != ':')
	ERROR(ENAMETOOLONG, 0);

      *nptr = 0;
      if (*path == ':')
	++path;

      if (parid)
	*parid = dirid;

      found = v_catsearch(*vol, dirid, name, data, fname, np);
      if (found == -1)
	goto fail;

      if (! found)
	{
	  if (*path && parid)
	    *parid = 0;

	  if (*path == 0 && fname)
	    strcpy(fname, name);

	  goto done;
	}

      switch (data->cdrType)
	{
	case cdrDirRec:
	  if (*path == 0)
	    goto done;

	  dirid = data->u.dir.dirDirID;
	  break;

	case cdrFilRec:
	  if (*path == 0)
	    goto done;

	  ERROR(ENOTDIR, "invalid pathname");

	default:
	  ERROR(EIO, "unexpected catalog record");
	}
    }

done:
  return found;

fail:
  return -1;
}
