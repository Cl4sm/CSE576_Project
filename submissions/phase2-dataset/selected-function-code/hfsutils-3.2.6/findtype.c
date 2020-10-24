static
const byte *findtype(rsrcmap *map, const char *type)
{
  const byte *ptr;
  long typeint;
  short ntypes;

  typeint = d_getsl((const unsigned char *) type);
  ntypes  = d_getsw(map->tlist);

  for (ptr = map->tlist + 2; ntypes >= 0; ptr += 8, --ntypes)
    {
      if (d_getsl(ptr) == typeint)
	break;
    }

  if (ntypes < 0)
    ERROR(EINVAL, "resource type not found");

  return ptr;

fail:
  return 0;
}
