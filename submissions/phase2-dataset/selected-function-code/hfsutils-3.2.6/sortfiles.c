void sortfiles(darray *files, int flags, int options)
{
  int (*compare)(const queueent *, const queueent *);

  switch (options & S_MASK)
    {
    case S_NAME:
      compare = compare_names;
      break;

    case S_TIME:
      switch (options & T_MASK)
	{
	case T_MOD:
	  compare = compare_mtimes;
	  break;

	case T_CREATE:
	  compare = compare_ctimes;
	  break;

	default:
	  abort();
	}
      break;

    case S_SIZE:
      compare = compare_sizes;
      break;

    default:
      return;
    }

  reverse = (flags & HLS_REVERSE) ? -1 : 1;

  darr_sort(files, (int (*)(const void *, const void *)) compare);
}
