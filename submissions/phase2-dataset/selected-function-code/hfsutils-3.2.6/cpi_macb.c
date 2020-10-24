{
  int ifile, result = 0;
  hfsfile *ofile;
  hfsdirent ent;
  const char *dsthint;
  char type[5], creator[5];
  unsigned char buf[MACB_BLOCKSZ];
  unsigned short crc;
  unsigned long dsize, rsize;

  ifile = opensrc(srcname, &dsthint, ".bin", 1);
  if (ifile == -1)
    return -1;

  if (read(ifile, buf, MACB_BLOCKSZ) < MACB_BLOCKSZ)
    {
      ERROR(errno, "error reading MacBinary file header");

      close(ifile);
      return -1;
    }

  if (buf[0] != 0 || buf[74] != 0)
    {
      ERROR(EINVAL, "invalid MacBinary file header");

      close(ifile);
      return -1;
    }

  crc = d_getuw(&buf[124]);

  if (crc_macb(buf, 124, 0x0000) != crc)
    {
      /* (buf[82] == 0) => MacBinary I? */

      ERROR(EINVAL, "unknown, unsupported, or corrupt MacBinary file");

      close(ifile);
      return -1;
    }

  if (buf[123] > 129)
    {
      ERROR(EINVAL, "unsupported MacBinary file version");

      close(ifile);
      return -1;
    }

  if (buf[1] < 1 || buf[1] > 63 ||
      buf[2 + buf[1]] != 0)
    {
      ERROR(EINVAL, "invalid MacBinary file header (bad file name)");

      close(ifile);
      return -1;
    }

  dsize = d_getul(&buf[83]);
  rsize = d_getul(&buf[87]);

  if (dsize > 0x7fffffff || rsize > 0x7fffffff)
    {
      ERROR(EINVAL, "invalid MacBinary file header (bad file length)");

      close(ifile);
      return -1;
    }

  dsthint = (char *) &buf[2];

  memcpy(type,    &buf[65], 4);
  memcpy(creator, &buf[69], 4);
  type[4] = creator[4] = 0;

  ofile = opendst(vol, dstname, dsthint, type, creator);
  if (ofile == 0)
    {
      close(ifile);
      return -1;
    }

  result = do_macb(ifile, ofile, dsize, rsize);

  if (result == 0 && hfs_fstat(ofile, &ent) == -1)
    {
      ERROR(errno, hfs_error);
      result = -1;
    }

  ent.fdflags = (buf[73] << 8 | buf[101]) &
    ~(HFS_FNDR_ISONDESK | HFS_FNDR_HASBEENINITED | HFS_FNDR_RESERVED);

  ent.crdate = d_ltime(d_getul(&buf[91]));
  ent.mddate = d_ltime(d_getul(&buf[95]));

  if (result == 0 && hfs_fsetattr(ofile, &ent) == -1)
    {
      ERROR(errno, hfs_error);
      result = -1;
    }

  closefiles(ifile, ofile, &result);

  return result;
}
