static
int strmatch(const char *str, const char *pat)
{
  while (1)
    {
      if (*str == 0 && *pat != 0 && *pat != '*')
	return 0;

      switch (*pat)
	{
	case 0:
	  return (*str == 0);

	case '*':
	  if (*++pat == 0)
	    return 1;

	  while (1)
	    {
	      if (strmatch(str, pat))
		return 1;

	      if (*str++ == 0)
		return 0;
	    }

	case '?':
	  break;

	case '[':
	  {
	    ++pat;

	    while (1)
	      {
		unsigned char p0, p1, s;

		p0 = *pat;

		if (p0 == ']' || p0 == 0)
		  return 0;

		s = *str;

		if (hfs_charorder[p0] == hfs_charorder[s])
		  break;

		if (pat[1] == '-')
		  {
		    p1 = pat[2];

		    if (p1 == 0)
		      return 0;

		    if ((hfs_charorder[p0] <= hfs_charorder[s] &&
			 hfs_charorder[p1] >= hfs_charorder[s]) ||
			(hfs_charorder[p0] >= hfs_charorder[s] &&
			 hfs_charorder[p1] <= hfs_charorder[s]))
		      break;

		    pat += 2;
		  }

		++pat;
	      }

	    while (*pat != ']')
	      {
		if (*pat == 0)
		  {
		    --pat;
		    break;
		  }

		++pat;
	      }
	  }
	  break;

	case '\\':
	  if (*++pat == 0)
	    return 0;

	  /* fall through */

	default:
	  if (hfs_charorder[(unsigned char) *pat] !=
	      hfs_charorder[(unsigned char) *str])
	    return 0;
	}

      ++pat, ++str;
    }
}
