{
  if (statef && mounts && dirty)
    {
      int i;

      rewind(statef);
      if (ftruncate(fileno(statef), 0) == -1)
	return -1;

      fprintf(statef, "%d\n", curvol);

      for (i = 0; i < nmounts; ++i)
	fprintf(statef, "%s\t%ld\t%s\t%d\t%s\n",
		mounts[i].vname,
		mounts[i].vcrdate,
		mounts[i].path,
		mounts[i].partno,
		mounts[i].cwd);

      dirty = 0;
    }

  if (mounts)
    {
      mountent *ent;

      for (ent = mounts; ent < mounts + nmounts; ++ent)
	{
	  if (ent->path)
	    free(ent->path);
	  if (ent->cwd)
	    free(ent->cwd);
	}

      free(mounts);
    }

  if (statef && fclose(statef) == EOF)
    return -1;

  return 0;
}
