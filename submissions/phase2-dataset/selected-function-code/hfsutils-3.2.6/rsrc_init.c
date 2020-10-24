rsrcfile *rsrc_init(void *priv, const struct rsrcprocs *procs)
{
  rsrcfile *rfile;
  byte head[16];
  const byte *ptr = head;
  unsigned long nbytes;
  unsigned short tlistoff, nlistoff;

  rfile = ALLOC(rsrcfile, 1);
  if (rfile == 0)
    ERROR(ENOMEM, 0);

  rfile->priv     = priv;
  rfile->procs    = *procs;

  rfile->map.data = 0;

  if (rfile->procs.seek(rfile->priv, 0, RSRC_SEEK_SET) == (unsigned long) -1)
    ERROR(errno, "error seeking resource header");

  nbytes = rfile->procs.read(rfile->priv, head, sizeof(head));
  if (nbytes != sizeof(head))
    {
      if (nbytes == (unsigned long) -1)
	ERROR(errno, "error reading resource header");
      else
	ERROR(EINVAL, "truncated resource header");
    }

  d_fetchul(&ptr, &rfile->hdr.dstart);
  d_fetchul(&ptr, &rfile->hdr.mstart);

  d_fetchul(&ptr, &rfile->hdr.dlen);
  d_fetchul(&ptr, &rfile->hdr.mlen);

  rfile->map.data = ALLOC(byte, rfile->hdr.mlen);
  if (rfile->map.data == 0)
    ERROR(ENOMEM, 0);

  if (rfile->procs.seek(rfile->priv, rfile->hdr.mstart, RSRC_SEEK_SET) ==
      (unsigned long) -1)
    ERROR(errno, "error seeking resource map");

  nbytes = rfile->procs.read(rfile->priv, rfile->map.data, rfile->hdr.mlen);
  if (nbytes != rfile->hdr.mlen)
    {
      if (nbytes == (unsigned long) -1)
	ERROR(errno, "error reading resource map");
      else
	ERROR(EIO, "truncated resource map");
    }

  memcpy(rfile->map.data, head, sizeof(head));

  ptr = rfile->map.data + 22;

  d_fetchuw(&ptr, &rfile->map.attrs);

  d_fetchuw(&ptr, &tlistoff);
  d_fetchuw(&ptr, &nlistoff);

  rfile->map.tlist = rfile->map.data + tlistoff;
  rfile->map.nlist = rfile->map.data + nlistoff;

  return rfile;

fail:
  if (rfile)
    FREE(rfile->map.data);

  FREE(rfile);

  return 0;
}
