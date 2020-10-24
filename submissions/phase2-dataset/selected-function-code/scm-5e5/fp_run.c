     int *fp;
     int fpoff;
     int k;
     void *a;
     int m;
     void *b;
     int n;
     int_function array_refsEql_P;
     int *cc;
     int p;
{
  int cost = (k)+(p)+(p);
  {
    int y = MAX((fp[ -1+(k)+(fpoff)])+1, fp[1+(k)+(fpoff)]);
L_snloop:
    {
      int x = (y)-(k);
      if ((cc)
	  && ((y)<=(n)))
	{
	  int xcst = (m)-(x);
	  if (0 > (xcst))
	    ;
	  else cc[y] = MIN((xcst)+(cost), cc[y]);
	}
      if (((x)<(m))
	  && ((y)<(n))
	  && (array_refsEql_P(a, x, m, b, y, n)))
	{
	  y = 1+(y);
	  goto L_snloop;
	}
      else {
	fp[(fpoff)+(k)] = y;
	return y;
      }
    }
  }
}
