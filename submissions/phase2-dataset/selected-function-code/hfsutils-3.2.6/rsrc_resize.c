void *rsrc_resize(void *rdata, unsigned long newsize)
{
  rsrchandle *newrsrc, *rsrc = gethandle(rdata);

  if (rsrc->len == newsize)
    goto done;

  newrsrc = (rsrchandle *) REALLOC(rsrc, byte, sizeof(rsrchandle) + newsize);
  if (newrsrc == 0)
    ERROR(ENOMEM, 0);

  newrsrc->attrs |= RSRC_RES_CHANGED;
  newrsrc->len    = newsize;

  rdata = getrdata(newrsrc);

done:
  return rdata;

fail:
  return 0;
}
