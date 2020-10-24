char *
dtime_unparse(coord c)
{
  char *r;
  char buf[50];

  if ( fabs(c.d) <= 1E-16 )	/* XXX kludge */
    (void) sprintf(buf, "0 s");
  else if ( fabs(c.d) < .9995E-6 )
    (void) sprintf(buf, "%7.3f ns", c.d * 1E9);
  else if ( fabs(c.d) < .9995E-3 )
    (void) sprintf(buf, "%7.3f us", c.d * 1E6);
  else if ( fabs(c.d) < .9995E0 )
    (void) sprintf(buf, "%7.3f ms", c.d * 1E3);
  else if  ( fabs(c.d) < .9995E3 )
    (void) sprintf(buf, "%7.3f  s", c.d);
  else
    (void) sprintf(buf, "%7.3f ks", c.d/ 1E3);
    
  r = malloc((unsigned) strlen(buf)+1);
  if (r == 0)
    fatalerror("malloc returned 0");
  (void) strcpy(r, buf);
  return r;
}