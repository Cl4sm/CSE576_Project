hfsfile *hfs_open(hfsvol *vol, const char *path)
{
  hfsfile *file = 0;

  if (getvol(&vol) == -1)
    goto fail;

  file = ALLOC(hfsfile, 1);
  if (file == 0)
    ERROR(ENOMEM, 0);

  if (v_resolve(&vol, path, &file->cat, &file->parid, file->name, 0) <= 0)
    goto fail;

  if (file->cat.cdrType != cdrFilRec)
    ERROR(EISDIR, 0);

  /* package file handle for user */

  file->vol   = vol;
  file->flags = 0;

  f_selectfork(file, fkData);

  file->prev = 0;
  file->next = vol->files;

  if (vol->files)
    vol->files->prev = file;

  vol->files = file;

  return file;

fail:
  FREE(file);
  return 0;
}
