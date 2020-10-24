rsrchandle *load(rsrcfile *rfile, const byte *ritem)
{
  unsigned long offs, nbytes, len, count;
  byte data[260];
  rsrchandle *rsrc = 0;

  offs = d_getul(ritem + 4) & 0x00ffffff;

  if (rfile->procs.seek(rfile->priv,
			rfile->hdr.dstart + offs, RSRC_SEEK_SET) ==
      (unsigned long) -1)
    ERROR(errno, "error seeking resource data");

  nbytes = rfile->procs.read(rfile->priv, data, sizeof(data));
  if (nbytes < 4)
    {
      if (nbytes == (unsigned long) -1)
	ERROR(errno, "error reading resource data");
      else
	ERROR(EIO, "truncated resource data");
    }

  len = d_getul(data);

  rsrc = (rsrchandle *) ALLOC(byte, sizeof(rsrchandle) + len);
  if (rsrc == 0)
    ERROR(ENOMEM, 0);

  count = nbytes - 4;
  if (count > len)
    count = len;

  memcpy(rsrc->data, data + 4, count);

  if (count < len)
    {
      nbytes = rfile->procs.read(rfile->priv, rsrc->data + count, len - count);
      if (nbytes != len - count)
	{
	  if (nbytes == (unsigned long) -1)
	    ERROR(errno, "error reading resource data");
	  else
	    ERROR(EIO, "truncated resource data");
	}
    }

  rsrc->rfile = rfile;
  rsrc->ritem = ritem;
  rsrc->len   = len;

  return rsrc;

fail:
  FREE(rsrc);
  return 0;
}
