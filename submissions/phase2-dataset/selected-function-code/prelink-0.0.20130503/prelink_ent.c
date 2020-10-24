prelink_ent (struct prelink_entry *ent)
{
  int i, j;
  DSO *dso;
  struct stat64 st;
  struct prelink_link *hardlink;
  char *move = NULL;
  size_t movelen = 0;

  for (i = 0; i < ent->ndepends; ++i)
    if (ent->depends[i]->done == 1)
      prelink_ent (ent->depends[i]);

  for (i = 0; i < ent->ndepends; ++i)
    if (ent->depends[i]->done != 2)
      {
	ent->done = 0;
	if (! undo)
	  ent->type = ET_UNPRELINKABLE;
	if (verbose)
	  error (0, 0, "Could not prelink %s because its dependency %s could not be prelinked",
		 ent->filename, ent->depends[i]->filename);
	return;
      }

  ent->u.tmp = 1;
  for (i = 0; i < ent->ndepends; ++i)
    ent->depends[i]->u.tmp = 1;
  for (i = 0; i < ent->ndepends; ++i)
    {
      struct prelink_entry *dent = ent->depends[i];
      for (j = 0; j < dent->ndepends; ++j)
	if (dent->depends[j]->u.tmp == 0)
	  {
	    ent->done = 0;
	    if (! undo)
	      ent->type = ET_UNPRELINKABLE;
	    if (verbose)
	      error (0, 0, "Could not prelink %s because it doesn't use %s, but one of its dependencies has been prelinked against it",
		     ent->filename, dent->depends[j]->filename);
	    ent->u.tmp = 0;
	    for (i = 0; i < ent->ndepends; ++i)
	      ent->depends[i]->u.tmp = 0;
	    return;
	  }
    }
  ent->u.tmp = 0;
  for (i = 0; i < ent->ndepends; ++i)
    ent->depends[i]->u.tmp = 0;

  if (verbose)
    {
      if (dry_run)
	printf ("Would prelink %s\n", ent->canon_filename);
      else
	printf ("Prelinking %s\n", ent->canon_filename);
    }

  dso = open_dso (ent->canon_filename);
  if (dso == NULL)
    goto error_out;

  if (fstat64 (dso->fd, &st) < 0)
    {
      error (0, errno, "%s changed during prelinking", ent->filename);
      goto error_out;
    }

  if (st.st_dev != ent->dev || st.st_ino != ent->ino)
    {
      error (0, 0, "%s changed during prelinking", ent->filename);
      goto error_out;
    }

  if (dry_run)
    close_dso (dso);
  else
    {
      if (prelink_prepare (dso))
	goto make_unprelinkable;
      if (ent->type == ET_DYN && relocate_dso (dso, ent->base))
	goto make_unprelinkable;
      if (prelink (dso, ent))
	goto make_unprelinkable;
      if (update_dso (dso, NULL))
	{
	  dso = NULL;
	  goto error_out;
	}
    }
  ent->done = 2;
  ent->flags |= PCF_PRELINKED;

  /* Redo hardlinks.  */
  for (hardlink = ent->hardlink; hardlink; hardlink = hardlink->next)
    {
      size_t len;

      if (lstat64 (hardlink->canon_filename, &st) < 0)
	{
	  error (0, 0, "Could not stat %s (former hardlink to %s)",
		 hardlink->canon_filename, ent->canon_filename);
	  continue;
	}

      if (st.st_dev != ent->dev || st.st_ino != ent->ino)
	{
	  error (0, 0, "%s is no longer hardlink to %s",
		 hardlink->canon_filename, ent->canon_filename);
	  continue;
	}

      if (verbose)
	{
	  if (dry_run)
	    printf ("Would link %s to %s\n", hardlink->canon_filename,
		    ent->canon_filename);
	  else
	    printf ("Linking %s to %s\n", hardlink->canon_filename,
		    ent->canon_filename);
	}

      if (dry_run)
	continue;

      len = strlen (hardlink->canon_filename);
      if (len + sizeof (".#prelink#") > movelen)
	{
	  movelen = len + sizeof (".#prelink#");
	  move = realloc (move, movelen);
	  if (move == NULL)
	    {
	      error (0, ENOMEM, "Could not hardlink %s to %s",
		     hardlink->canon_filename, ent->canon_filename);
	      movelen = 0;
	      continue;
	    }
	}

      memcpy (mempcpy (move, hardlink->canon_filename, len), ".#prelink#",
	      sizeof (".#prelink#"));
      if (rename (hardlink->canon_filename, move) < 0)
	{
	  error (0, errno, "Could not hardlink %s to %s",
		 hardlink->canon_filename, ent->canon_filename);
	  continue;
	}

      if (link (ent->canon_filename, hardlink->canon_filename) < 0)
	{
	  error (0, errno, "Could not hardlink %s to %s",
		 hardlink->canon_filename, ent->canon_filename);

	  if (rename (move, hardlink->canon_filename) < 0)
	    {
	      error (0, errno, "Could not rename %s back to %s",
		     move, hardlink->canon_filename);
	    }
	  continue;
	}

      if (unlink (move) < 0)
	{
	  error (0, errno, "Could not unlink %s", move);
	  continue;
	}
    }
  free (move);

  if (! dry_run && stat64 (ent->canon_filename, &st) >= 0)
    {
      ent->dev = st.st_dev;
      ent->ino = st.st_ino;
      ent->ctime = st.st_ctime;
      ent->mtime = st.st_mtime;
    }
  return;

make_unprelinkable:
  if (! undo)
    ent->type = ET_UNPRELINKABLE;
error_out:
  ent->done = 0;
  if (dso)
    close_dso (dso);
  return;
}
