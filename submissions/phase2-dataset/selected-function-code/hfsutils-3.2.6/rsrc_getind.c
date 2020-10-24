{
  const byte *ptr;
  short nitems;
  rsrchandle *rsrc;

  ptr = findtype(&rfile->map, type);
  if (ptr == 0)
    goto fail;

  nitems = d_getsw(ptr + 4) + 1;
  if (index < 1 || index > nitems)
    ERROR(EINVAL, "index out of range");

  rsrc = load(rfile, rfile->map.tlist + d_getsw(ptr + 6) + 12 * (index - 1));
  if (rsrc == 0)
    goto fail;

  return getrdata(rsrc);

fail:
  return 0;
}
