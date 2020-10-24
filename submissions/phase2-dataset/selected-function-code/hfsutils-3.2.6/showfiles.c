static
int showfiles(darray *files, int flags, int options, int width)
{
  dlist list;
  int i, sz, result = 0;
  queueent *ents;
  dstring str;
  char **strs;
  void (*show)(int, queueent *, char **, int, int, int);

  if (dl_init(&list) == -1)
    {
      fprintf(stderr, "%s: not enough memory\n", argv0);
      return -1;
    }

  sz   = darr_size(files);
  ents = darr_array(files);

  dstr_init(&str);

  for (i = 0; i < sz; ++i)
    {
      if (outpath(&str, &ents[i], flags) == -1 ||
	  dl_append(&list, dstr_string(&str)) == -1)
	{
	  result = -1;
	  break;
	}
    }

  dstr_free(&str);

  strs = dl_array(&list);

  switch (options & F_MASK)
    {
    case F_LONG:
      show = show_long;
      break;

    case F_ONE:
      show = show_one;
      break;

    case F_MANY:
      show = show_many;
      break;

    case F_HORIZ:
      show = show_horiz;
      break;

    case F_COMMAS:
      show = show_commas;
      break;

    default:
      abort();
    }

  show(sz, ents, strs, flags, options, width);

  dl_free(&list);

  return result;
}
