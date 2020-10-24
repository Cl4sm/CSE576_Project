{
  hfsfile *file;

  if (getvol(&vol) == -1)
    goto fail;

  for (file = vol->files; file; file = file->next)
    {
      if (f_flush(file) == -1)
	goto fail;
    }

  if (v_flush(vol) == -1)
    goto fail;

  return 0;

fail:
  return -1;
}
