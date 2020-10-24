char *
blacknwhite_image()
{
  int i, j, k, l, u;
  char *bits;

  bits = (char *)
        salloc(((mapwidth+7)/8)*mapheight*sizeof(char));

  k = 0;
  for (j=0; j<mapheight; j++) {
    l = 1;
    u = 0;
    for (i=0; i<mapwidth; i++) {
      if (grid[(i+1)*mapheight+j]<65536) u = u+l;
      l = l+l;
      if (l==256 || i==mapwidth-1) {
        bits[k] = u;
	u = 0;
	l = 1;
        ++k;
      }
    }
  }
  return bits;
}
