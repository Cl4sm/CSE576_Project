struct prelink_entry *
prelink_find_entry (const char *filename, const struct stat64 *stp,
		    int insert)
{
  struct prelink_entry e, *ent = NULL;
  void **filename_slot, *dummy = NULL;
  void **devino_slot = NULL;
  struct stat64 st;
  char *canon_filename = NULL;

  e.filename = filename;
  filename_slot = htab_find_slot (prelink_filename_htab, &e,
				  insert ? INSERT : NO_INSERT);
  if (filename_slot == NULL)
    {
      if (insert)
	goto error_out;
      filename_slot = &dummy;
    }

  if (*filename_slot != NULL)
    return (struct prelink_entry *) *filename_slot;

  if (! stp)
    {
      canon_filename = prelink_canonicalize (filename, &st);
      if (canon_filename == NULL && stat64 (filename, &st) < 0)
	{
	  error (0, errno, "Could not stat %s", filename);
	  if (insert)
	    {
	      *filename_slot = &dummy;
	      htab_clear_slot (prelink_filename_htab, filename_slot);
	    }
	  return NULL;
	}
      stp = &st;
    }

  e.dev = stp->st_dev;
  e.ino = stp->st_ino;
  devino_slot = htab_find_slot (prelink_devino_htab, &e,
				insert ? INSERT : NO_INSERT);
  if (devino_slot == NULL)
    {
      if (insert)
	goto error_out;
      free (canon_filename);
      return NULL;
    }

  if (*devino_slot != NULL)
    {
      ent = (struct prelink_entry *) *devino_slot;
      if (canon_filename == NULL)
	canon_filename = prelink_canonicalize (filename, NULL);
      if (canon_filename == NULL)
	{
	  error (0, 0, "Could not canonicalize filename %s", filename);
	  goto error_out2;
	}

      if (strcmp (canon_filename, ent->canon_filename) != 0)
	{
	  struct prelink_link *hardlink;

	  hardlink = (struct prelink_link *)
		     malloc (sizeof (struct prelink_link));
	  if (hardlink == NULL)
	    {
	      error (0, ENOMEM, "Could not record hardlink %s to %s",
		     canon_filename, ent->canon_filename);
	      goto error_out2;
	    }

	  hardlink->canon_filename = canon_filename;
	  hardlink->next = ent->hardlink;
	  ent->hardlink = hardlink;
	}
      else
	free (canon_filename);
      return ent;
    }

  if (! insert)
    {
      if (canon_filename != NULL)
	free (canon_filename);
      return NULL;
    }

  ent = (struct prelink_entry *) calloc (sizeof (struct prelink_entry), 1);
  if (ent == NULL)
    goto error_out;

  ent->filename = strdup (filename);
  if (ent->filename == NULL)
    goto error_out;

  if (canon_filename != NULL)
    ent->canon_filename = canon_filename;
  else
    ent->canon_filename = prelink_canonicalize (filename, NULL);
  if (ent->canon_filename == NULL)
    {
      error (0, 0, "Could not canonicalize filename %s", filename);
      free ((char *) ent->filename);
      free (ent);
      goto error_out2;
    }

  ent->dev = stp->st_dev;
  ent->ino = stp->st_ino;
  ent->ctime = stp->st_ctime;
  ent->mtime = stp->st_mtime;

  *filename_slot = ent;
  *devino_slot = ent;
  ++prelink_entry_count;
  return ent;

error_out:
  free (ent);
  error (0, ENOMEM, "Could not insert %s into hash table", filename);
error_out2:
  if (insert)
    {
      if (filename_slot != NULL)
	{
	  assert (*filename_slot == NULL);
	  *filename_slot = &dummy;
	  htab_clear_slot (prelink_filename_htab, filename_slot);
	}
      if (devino_slot != NULL && *devino_slot == NULL)
	{
	  *devino_slot = &dummy;
	  htab_clear_slot (prelink_devino_htab, devino_slot);
	}
    }
  free (canon_filename);
  return NULL;
}
