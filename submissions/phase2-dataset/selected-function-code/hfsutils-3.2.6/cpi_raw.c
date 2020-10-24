int cpi_raw(const char *srcname, hfsvol *vol, const char *dstname)
{
  int ifile, result = 0;
  hfsfile *ofile;
  const char *dsthint;

  ifile = opensrc(srcname, &dsthint, 0, 1);
  if (ifile == -1)
    return -1;

  ofile = opendst(vol, dstname, dsthint, RAW_TYPE, RAW_CREA);
  if (ofile == 0)
    {
      close(ifile);
      return -1;
    }

  result = do_raw(ifile, ofile);

  closefiles(ifile, ofile, &result);

  return result;
}
