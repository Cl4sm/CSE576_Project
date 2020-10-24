{
  int ifile, result = 0;
  hfsfile *ofile;
  const char *dsthint;

  ifile = opensrc(srcname, &dsthint, ".txt", 0);
  if (ifile == -1)
    return -1;

  ofile = opendst(vol, dstname, dsthint, TEXT_TYPE, TEXT_CREA);
  if (ofile == 0)
    {
      close(ifile);
      return -1;
    }

  result = do_text(ifile, ofile);

  closefiles(ifile, ofile, &result);

  return result;
}
