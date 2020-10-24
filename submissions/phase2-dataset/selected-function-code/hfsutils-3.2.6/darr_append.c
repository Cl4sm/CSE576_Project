{
  char *eltend = array->eltend;
  size_t elemsz = array->elemsz;

  if ((size_t) (eltend - array->mem) == array->memsz)
    {
      char *newmem;
      size_t newsz;

      newsz  = array->memsz * 2;
      newmem = realloc(array->mem, newsz);
      if (newmem == 0)
	return 0;

      eltend = newmem + array->memsz;

      array->mem   = newmem;
      array->memsz = newsz;
    }

  memcpy(eltend, elem, elemsz);
  array->eltend = eltend + elemsz;

  return eltend;
}
