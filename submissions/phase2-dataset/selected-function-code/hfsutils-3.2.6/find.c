const byte *find(rsrcmap *map, const char *type,
		 int (*compare)(rsrcmap *, const byte *, const void *),
		 const void *key)
{
  const byte *ptr;
  short nitems;
  unsigned short rlistoff;

  ptr = findtype(map, type);
  if (ptr == 0)
    goto fail;

  ptr += 4;
  d_fetchsw(&ptr, &nitems);
  d_fetchuw(&ptr, &rlistoff);

  for (ptr = map->tlist + rlistoff; nitems >= 0; ptr += 12, --nitems)
    {
      if (compare(map, ptr, key))
	break;
    }

  if (nitems < 0)
    ERROR(EINVAL, "resource not found");

  return ptr;

fail:
  return 0;
}
