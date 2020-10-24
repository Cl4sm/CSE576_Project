char *
getdata(fd)
FILE * fd;
{
  int i, j;
  char c;

 repeat:
  i = fgetc(fd);
  if (i==EOF) return NULL;

  c = (char) i;

  if (c=='%') {
     while (i!=EOF && (char) i!= '\n') {
        i = fgetc(fd);
     }
     if (i==EOF) return NULL;
     goto repeat;
  }

  while(isspace(c)) goto repeat;
  
  j = 0;
  while(!isspace(c) && j<125) {
    buf[j] = c;
    ++j;
    i = fgetc(fd);
    if (i==EOF) break;
    c = (char) i;
  }

  buf[j] = '\0';
  return buf;
}
