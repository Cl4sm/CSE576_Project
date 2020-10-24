static
int do_raw(hfsfile *ifile, int ofile)
{
  char buf[HFS_BLOCKSZ * 4];
  long chunk, bytes;

  while (1)
    {
      chunk = hfs_read(ifile, buf, sizeof(buf));
      if (chunk == -1)
	{
	  ERROR(errno, hfs_error);
	  return -1;
	}
      else if (chunk == 0)
	break;

      bytes = write(ofile, buf, chunk);
      if (bytes == -1)
	{
	  ERROR(errno, "error writing data");
	  return -1;
	}
      else if (bytes != chunk)
	{
	  ERROR(EIO, "wrote incomplete chunk");
	  return -1;
	}
    }

  return 0;
}
