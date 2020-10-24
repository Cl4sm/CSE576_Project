int diff_mid_split(n, rr, cc, cost)
     int n;
     int *rr;
     int *cc;
     int cost;
{
  {
    int cdx = 1+((n)/2);
    int rdx = (n)/2;
L_loop:
    if ((cost)==((cc[rdx])+(rr[(n)-(rdx)])))
      return rdx;
    else if ((cost)==((cc[cdx])+(rr[(n)-(cdx)])))
      return cdx;
    else {
      cdx = 1+(cdx);
      rdx =  -1+(rdx);
      goto L_loop;
    }
  }
}
