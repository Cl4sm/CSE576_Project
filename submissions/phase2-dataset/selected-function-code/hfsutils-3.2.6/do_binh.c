int do_binh(hfsfile *ifile, int ofile)
{
  int result;

  if (bh_start(ofile) == -1)
    {
      ERROR(errno, bh_error);
      return -1;
    }

  result = binhx(ifile);

  if (bh_end() == -1 && result == 0)
    {
      ERROR(errno, bh_error);
      result = -1;
    }

  return result;
}
