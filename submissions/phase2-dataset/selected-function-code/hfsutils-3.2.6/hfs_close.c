{
  hfsvol *vol = file->vol;
  int result = 0;

  if (f_trunc(file) == -1 ||
      f_flush(file) == -1)
    result = -1;

  if (file->prev)
    file->prev->next = file->next;
  if (file->next)
    file->next->prev = file->prev;
  if (file == vol->files)
    vol->files = file->next;

  FREE(file);

  return result;
}
