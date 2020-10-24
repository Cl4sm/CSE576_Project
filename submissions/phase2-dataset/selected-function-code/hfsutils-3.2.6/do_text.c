int do_text(hfsfile *ifile, int ofile)
{
  char buf[HFS_BLOCKSZ * 4], *ptr;
  long chunk, bytes;
  int len;

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

      for (ptr = buf; ptr < buf + chunk; ++ptr)
	{
	  if (*ptr == '\r')
	    *ptr = '\n';
	}

      len = chunk;
      ptr = cs_latin1(buf, &len);
      if (ptr == 0)
	{
	  ERROR(ENOMEM, 0);
	  return -1;
	}

      bytes = write(ofile, ptr, len);
      free(ptr);

      if (bytes == -1)
	{
	  ERROR(errno, "error writing data");
	  return -1;
	}
      else if (bytes != len)
	{
	  ERROR(EIO, "wrote incomplete chunk");
	  return -1;
	}
    }

  return 0;
}
