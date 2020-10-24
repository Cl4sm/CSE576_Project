int hfs_mkdir(hfsvol *vol, const char *path)
{
  CatDataRec data;
  unsigned long parid;
  char name[HFS_MAX_FLEN + 1];
  int found;

  if (getvol(&vol) == -1)
    goto fail;

  found = v_resolve(&vol, path, &data, &parid, name, 0);
  if (found == -1 || parid == 0)
    goto fail;

  if (found)
    ERROR(EEXIST, 0);

  if (parid == HFS_CNID_ROOTPAR)
    ERROR(EINVAL, 0);

  if (vol->flags & HFS_VOL_READONLY)
    ERROR(EROFS, 0);

  return v_mkdir(vol, parid, name);

fail:
  return -1;
}
