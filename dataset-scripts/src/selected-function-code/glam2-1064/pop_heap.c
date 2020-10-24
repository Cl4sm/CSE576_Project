void pop_heap(void *base, size_t n, size_t size,
               int (*cmp)(const void *, const void *)) {
  size_t hole = 1;
  if (n == 0)
    return;

  memswap(base, (char *)base + (n-1) * size, size);

  while (1) {
    size_t child1 = hole * 2;
    size_t child2 = hole * 2 + 1;

    if (child1 >= n)
      break;
    else if (child2 < n)
      if ((*cmp)((const char *)base + (child1-1) * size,
		 (const char *)base + (child2-1) * size) < 0)
	child1 = child2;
    memswap((char *)base + (hole-1) * size,
	    (char *)base + (child1-1) * size, size);
    hole = child1;
  }

  push_heap(base, hole, size, cmp);
}