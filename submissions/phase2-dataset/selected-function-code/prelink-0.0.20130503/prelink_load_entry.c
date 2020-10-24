static struct prelink_entry *
prelink_load_entry (const char *filename)
{
  struct prelink_entry e, *ent = NULL;
  void **filename_slot, *dummy = NULL;
  void **devino_slot = &dummy;
  struct stat64 st;
  uint32_t ctime = 0, mtime = 0;
  char *canon_filename = NULL;

  e.filename = filename;
  filename_slot = htab_find_slot (prelink_filename_htab, &e, INSERT);
  if (filename_slot == NULL)
    goto error_out;

  if (*filename_slot != NULL)
    return (struct prelink_entry *) *filename_slot;

  canon_filename = prelink_canonicalize (filename, &st);
  if (canon_filename == NULL)
    goto error_out2;
  if (strcmp (canon_filename, filename) != 0)
    {
      *filename_slot = &dummy;
      htab_clear_slot (prelink_filename_htab, filename_slot);

      e.filename = canon_filename;
      filename_slot = htab_find_slot (prelink_filename_htab, &e, INSERT);
      if (filename_slot == NULL)
	goto error_out;

      if (*filename_slot != NULL)
	{
	  free (canon_filename);
	  return (struct prelink_entry *) *filename_slot;
	}
    }

  if (! S_ISREG (st.st_mode))
    {
      free (canon_filename);
      *filename_slot = &dummy;
      htab_clear_slot (prelink_filename_htab, filename_slot);
      return NULL;
    }
  else
    {
      e.dev = st.st_dev;
      e.ino = st.st_ino;
      ctime = (uint32_t) st.st_ctime;
      mtime = (uint32_t) st.st_mtime;
      devino_slot = htab_find_slot (prelink_devino_htab, &e, INSERT);
      if (devino_slot == NULL)
	goto error_out;
    }

  if (*devino_slot != NULL)
    {
      free (canon_filename);
      *filename_slot = &dummy;
      htab_clear_slot (prelink_filename_htab, filename_slot);
      return (struct prelink_entry *) *devino_slot;
    }

  ent = (struct prelink_entry *) calloc (sizeof (struct prelink_entry), 1);
  if (ent == NULL)
    goto error_out;

  ent->filename = strdup (filename);
  if (ent->filename == NULL)
    goto error_out;

  ent->canon_filename = canon_filename;
  ent->dev = e.dev;
  ent->ino = e.ino;
  ent->ctime = ctime;
  ent->mtime = mtime;
  *filename_slot = ent;
  *devino_slot = ent;
  ++prelink_entry_count;
  return ent;

error_out:
  free (ent);
  error (0, ENOMEM, "Could not insert %s into hash table", filename);
error_out2:
  if (filename_slot != NULL)
    {
      *filename_slot = &dummy;
      htab_clear_slot (prelink_filename_htab, filename_slot);
    }
  if (devino_slot != NULL && devino_slot != &dummy)
    {
      *devino_slot = &dummy;
      htab_clear_slot (prelink_devino_htab, devino_slot);
    }
  free (canon_filename);
  return NULL;
}
