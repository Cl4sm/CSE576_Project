int
as_path_get_last(struct adata *path, u32 *orig_as)
{
  int found = 0;
  u32 res = 0;
  u8 *p = path->data;
  u8 *q = p+path->length;
  int len;

  while (p<q)
    {
      switch (*p++)
	{
	case AS_PATH_SET:
	  if (len = *p++)
	    {
	      found = 0;
	      p += BS * len;
	    }
	  break;
	case AS_PATH_SEQUENCE:
	  if (len = *p++)
	    {
	      found = 1;
	      res = get_as(p + BS * (len - 1));
	      p += BS * len;
	    }
	  break;
	default: bug("as_path_get_first: Invalid path segment");
	}
    }

  if (found)
    *orig_as = res;
  return found;
}
