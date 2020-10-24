     int *fp;
     int fpoff;
     int *cc;
     void *a;
     int m;
     void *b;
     int n;
     int_function array_refsEql_P;
     int p_lim;
{
  int delta = (n)-(m);
  {
    int p = 0;
L_loop:
    {
      int k = -(p);
      while (!((k)>=(delta))) {
	fp_run(fp, fpoff, k, a, m, b, n, array_refsEql_P, cc, p);
	{
	  k = 1+(k);
	}
      }
    }
    {
      int k = (delta)+(p);
      while (!((k)<=(delta))) {
	fp_run(fp, fpoff, k, a, m, b, n, array_refsEql_P, cc, p);
	{
	  k =  -1+(k);
	}
      }
    }
    {
      int fpval = fp_run(fp, fpoff, delta, a, m, b, n, array_refsEql_P, cc, p);
      if ((!(cc))
	  && ((n)<=(fpval)))
	return (delta)+(2*(p));
      else if ((!(0 > (p_lim)))
	  && ((p)>=(p_lim)))
	return -1;
      else {
	p = 1+(p);
	goto L_loop;
      }
    }
  }
}
