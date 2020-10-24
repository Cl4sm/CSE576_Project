static
int fork_binh(hfsfile *ifile, unsigned long size)
{
  char buf[HFS_BLOCKSZ * 4];
  long bytes;
  unsigned long total = 0;

  while (1)
    {
      bytes = hfs_read(ifile, buf, sizeof(buf));
      if (bytes == -1)
	{
	  ERROR(errno, hfs_error);
	  return -1;
	}
      else if (bytes == 0)
	break;

      if (bh_insert(buf, bytes) == -1)
	{
	  ERROR(errno, bh_error);
	  return -1;
	}

      total += bytes;
    }

  if (total != size)
    {
      ERROR(EIO, "inconsistent fork length");
      return -1;
    }

  if (bh_insertcrc() == -1)
    {
      ERROR(errno, bh_error);
      return -1;
    }

  return 0;
}
