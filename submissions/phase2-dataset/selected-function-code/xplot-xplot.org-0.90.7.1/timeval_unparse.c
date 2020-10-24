char *timeval_unparse(coord c)
{
  char *cp;
  char *r;
  char buf[50];
  struct tm *tmp;
  
  tmp = localtime((time_t *) &(c.t.tv_sec));
  (void) sprintf(buf,"%s",asctime(tmp));

  if (c.t.tv_usec == 0 && tmp->tm_sec == 0 && tmp->tm_min == 0 && tmp->tm_hour == 0) {
    cp = buf+4;
    sprintf(cp+7,"midn");
  } else if (c.t.tv_usec == 0 && tmp->tm_sec == 0 && tmp->tm_min == 0 && tmp->tm_hour == 12) {
    cp = buf+4;
    sprintf(cp+7,"noon");
  } else {
    cp = buf+10;
    cp[10] = '\0';
    if (c.t.tv_usec != 0) {
      if (c.t.tv_usec % 100 == 0) {
	(void) sprintf(cp+9,".%04u",(unsigned) c.t.tv_usec/100);
	cp += 7;
      } else {
	(void) sprintf(cp+9,".%06u",(unsigned) c.t.tv_usec);
	cp += 9;
      }
    }
  }
  r = malloc((unsigned) strlen(cp)+1);
  if (r == 0)
    fatalerror("malloc returned 0");
  (void) strcpy(r, cp);
  return r;
}