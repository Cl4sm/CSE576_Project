static
int fork_macb(hfsfile *ifile, int ofile, unsigned long size)
{
  char buf[HFS_BLOCKSZ * 4];
  long chunk, bytes;
  unsigned long total = 0;

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

      total += bytes;
    }

  if (total != size)
    {
      ERROR(EIO, "inconsistent fork length");
      return -1;
    }

  chunk = total % MACB_BLOCKSZ;
  if (chunk)
    {
      memset(buf, 0, MACB_BLOCKSZ);
      bytes = write(ofile, buf, MACB_BLOCKSZ - chunk);
      if (bytes == -1)
	{
	  ERROR(errno, "error writing data");
	  return -1;
	}
      else if (bytes != MACB_BLOCKSZ - chunk)
	{
	  ERROR(EIO, "wrong incomplete chunk");
	  return -1;
	}
    }

  return 0;
}
