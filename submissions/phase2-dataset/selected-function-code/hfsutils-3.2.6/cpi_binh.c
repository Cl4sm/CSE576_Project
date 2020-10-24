int cpi_binh(const char *srcname, hfsvol *vol, const char *dstname)
{
  int ifile, result;
  hfsfile *ofile;
  hfsdirent ent;
  const char *dsthint;
  char fname[HFS_MAX_FLEN + 1], type[5], creator[5];
  short fdflags;
  unsigned long dsize, rsize;

  ifile = opensrc(srcname, &dsthint, ".hqx", 0);
  if (ifile == -1)
    return -1;

  if (bh_open(ifile) == -1)
    {
      ERROR(errno, bh_error);

      close(ifile);
      return -1;
    }

  if (binhx(fname, type, creator, &fdflags, &dsize, &rsize) == -1)
    {
      bh_close();
      close(ifile);
      return -1;
    }

  dsthint = fname;

  ofile = opendst(vol, dstname, dsthint, type, creator);
  if (ofile == 0)
    {
      bh_close();
      close(ifile);
      return -1;
    }

  result = do_binh(ofile, dsize, rsize);

  if (bh_close() == -1 && result == 0)
    {
      ERROR(errno, bh_error);
      result = -1;
    }

  if (result == 0 && hfs_fstat(ofile, &ent) == -1)
    {
      ERROR(errno, hfs_error);
      result = -1;
    }

  ent.fdflags = fdflags &
    ~(HFS_FNDR_ISONDESK | HFS_FNDR_HASBEENINITED | HFS_FNDR_ISINVISIBLE);

  if (result == 0 && hfs_fsetattr(ofile, &ent) == -1)
    {
      ERROR(errno, hfs_error);
      result = -1;
    }

  closefiles(ifile, ofile, &result);

  return result;
}
