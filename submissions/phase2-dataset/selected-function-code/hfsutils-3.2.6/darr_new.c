{
  darray *array;

  array = malloc(sizeof(darray));
  if (array == 0)
    return 0;

  array->memsz = 8 * elemsz;
  array->mem   = malloc(array->memsz);
  if (array->mem == 0)
    {
      free(array);
      return 0;
    }

  array->eltend = array->mem;
  array->elemsz = elemsz;

  return array;
}
