void zoom_in_coord(coord_type ctype, coord first, coord last,
		   int x1, int x2, 
		   int n,
		   coord *newfirst, coord *newlast)
{
  int xf,xl;

#ifdef SDO
  if (x2 < 0) x2 = 0;
#endif

  if (x1 < x2) {
    xl = x2; xf = x1;
  } else {
    xl = x1; xf = x2;
  }

  *newfirst = impls[(int)ctype]->unmap(first, last, n, xf);
  *newlast  = impls[(int)ctype]->unmap(first, last, n, xl);

  if (impls[(int)ctype]->cmp(*newfirst, *newlast) == 0) {
    *newfirst = first;
    *newlast = last;
  }

  return;
}