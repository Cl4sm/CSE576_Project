void drag_coord(coord_type ctype, coord first, coord last,
		int x1, int x2,
		int n,
		coord *newfirst, coord *newlast)
{
  coord c1,c2;
  coord diff;

  c1 = impls[(int)ctype]->unmap(first, last, n, x2);
  c2 = impls[(int)ctype]->unmap(first, last, n, x1);
  
  switch(impls[(int)ctype]->cmp(c1,c2)) {
  case 1:
    diff = impls[(int)ctype]->subtract(c1, c2);
    *newfirst = impls[(int)ctype]->add(first, diff);
    *newlast  = impls[(int)ctype]->add(last, diff);
    break;
  case 0:
    *newfirst = first;
    *newlast  = last;
    break;
  case -1:
    diff = impls[(int)ctype]->subtract(c2, c1);
    *newfirst = impls[(int)ctype]->subtract(first, diff);
    *newlast  = impls[(int)ctype]->subtract(last, diff);
    break;
  }
    
  return;
}