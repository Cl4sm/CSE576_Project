{
  size_t len;

  len = strlen(str) + 1;

  /* make sure there is room */

  if (sizeof(char *) + len > (size_t) (list->strs - (char *) list->eltend))
    {
      dlist newlist;
      size_t strsz;
      char **elt;

      strsz = (list->mem + list->memsz) - list->strs;

      newlist.memsz = list->memsz * 2 + sizeof(char *) + len;
      newlist.mem   = malloc(newlist.memsz);
      if (newlist.mem == 0)
	return -1;

      newlist.eltend = (char **) newlist.mem;
      newlist.strs   = newlist.mem + newlist.memsz - strsz;

      memcpy(newlist.strs, list->strs, strsz);

      for (elt = (char **) list->mem; elt < list->eltend; ++elt)
	*newlist.eltend++ = newlist.strs + (*elt - list->strs);

      free(list->mem);

      *list = newlist;
    }

  list->strs -= len;
  strcpy(list->strs, str);

  *list->eltend++ = list->strs;

  return 0;
}
