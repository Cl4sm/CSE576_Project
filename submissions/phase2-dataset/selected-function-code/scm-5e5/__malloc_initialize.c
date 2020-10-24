__malloc_initialize ()
{
  if (__malloc_initialized)
    return 0;

  if (__malloc_initialize_hook)
    (*__malloc_initialize_hook) ();

  heapsize = HEAP / BLOCKSIZE;
  _heapinfo = (malloc_info *) align (heapsize * sizeof (malloc_info));
  if (_heapinfo == NULL)
    return 0;
  memset (_heapinfo, 0, heapsize * sizeof (malloc_info));
  _heapinfo[0].free.size = 0;
  _heapinfo[0].free.next = _heapinfo[0].free.prev = 0;
  _heapindex = 0;
  _heapbase = (char *) _heapinfo;
  _heaplimit = BLOCK (_heapbase + heapsize * sizeof (malloc_info));

  register_heapinfo ();

  __malloc_initialized = 1;
  return 1;
}
