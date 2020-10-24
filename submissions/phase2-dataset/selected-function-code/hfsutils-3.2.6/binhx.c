static
int binhx(hfsfile *ifile)
{
  hfsdirent ent;
  unsigned char byte, word[2], lword[4];

  if (hfs_fstat(ifile, &ent) == -1)
    {
      ERROR(errno, hfs_error);
      return -1;
    }

  byte = strlen(ent.name);
  if (bh_insert(&byte, 1) == -1 ||
      bh_insert(ent.name, byte + 1) == -1 ||
      bh_insert(ent.u.file.type, 4) == -1 ||
      bh_insert(ent.u.file.creator, 4) == -1)
    {
      ERROR(errno, bh_error);
      return -1;
    }

  d_putsw(word, ent.fdflags);
  if (bh_insert(word, 2) == -1)
    {
      ERROR(errno, bh_error);
      return -1;
    }

  d_putul(lword, ent.u.file.dsize);
  if (bh_insert(lword, 4) == -1)
    {
      ERROR(errno, bh_error);
      return -1;
    }

  d_putul(lword, ent.u.file.rsize);
  if (bh_insert(lword, 4) == -1 ||
      bh_insertcrc() == -1)
    {
      ERROR(errno, bh_error);
      return -1;
    }

  if (hfs_setfork(ifile, 0) == -1)
    {
      ERROR(errno, hfs_error);
      return -1;
    }

  if (fork_binh(ifile, ent.u.file.dsize) == -1)
    return -1;

  if (hfs_setfork(ifile, 1) == -1)
    {
      ERROR(errno, hfs_error);
      return -1;
    }

  if (fork_binh(ifile, ent.u.file.rsize) == -1)
    return -1;

  return 0;
}
