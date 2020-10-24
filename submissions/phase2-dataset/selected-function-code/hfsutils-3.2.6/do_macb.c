int do_macb(hfsfile *ifile, int ofile)
{
  hfsdirent ent;
  unsigned char buf[MACB_BLOCKSZ];
  long bytes;

  if (hfs_fstat(ifile, &ent) == -1)
    {
      ERROR(errno, hfs_error);
      return -1;
    }

  memset(buf, 0, MACB_BLOCKSZ);

  buf[1] = strlen(ent.name);
  strcpy((char *) &buf[2], ent.name);

  memcpy(&buf[65], ent.u.file.type,    4);
  memcpy(&buf[69], ent.u.file.creator, 4);

  buf[73] = ent.fdflags >> 8;

  d_putul(&buf[83], ent.u.file.dsize);
  d_putul(&buf[87], ent.u.file.rsize);

  d_putul(&buf[91], d_mtime(ent.crdate));
  d_putul(&buf[95], d_mtime(ent.mddate));

  buf[101] = ent.fdflags & 0xff;
  buf[122] = buf[123] = 129;

  d_putuw(&buf[124], crc_macb(buf, 124, 0x0000));

  bytes = write(ofile, buf, MACB_BLOCKSZ);
  if (bytes == -1)
    {
      ERROR(errno, "error writing data");
      return -1;
    }
  else if (bytes != MACB_BLOCKSZ)
    {
      ERROR(EIO, "wrote incomplete chunk");
      return -1;
    }

  if (hfs_setfork(ifile, 0) == -1)
    {
      ERROR(errno, hfs_error);
      return -1;
    }

  if (fork_macb(ifile, ofile, ent.u.file.dsize) == -1)
    return -1;

  if (hfs_setfork(ifile, 1) == -1)
    {
      ERROR(errno, hfs_error);
      return -1;
    }

  if (fork_macb(ifile, ofile, ent.u.file.rsize) == -1)
    return -1;

  return 0;
}
