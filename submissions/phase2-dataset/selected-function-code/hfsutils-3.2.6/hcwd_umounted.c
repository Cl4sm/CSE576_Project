int hcwd_umounted(int vol)
{
  mountent *entry;
  int i;

  if (vol < 0)
    vol = curvol;

  if (vol < 0 || vol >= nmounts)
    return -1;

  entry = &mounts[vol];

  if (entry->path)
    free(entry->path);
  if (entry->cwd)
    free(entry->cwd);

  for (i = vol + 1; i < nmounts; ++i)
    mounts[i - 1] = mounts[i];

  --nmounts;

  if (curvol > vol)
    --curvol;
  else if (curvol == vol)
    curvol = -1;

  return 0;
}
