static
int addent(mountent *ent)
{
  if (nmounts >= mtabsz)
    {
      mountent *newmem;

      mtabsz += TABCHUNKSZ;
      newmem = mounts ? realloc(mounts, mtabsz * sizeof(mountent)) :
	malloc(mtabsz * sizeof(mountent));
      if (newmem == 0)
	return -1;

      mounts = newmem;
    }

  mounts[nmounts++] = *ent;

  dirty = 1;

  return 0;
}
