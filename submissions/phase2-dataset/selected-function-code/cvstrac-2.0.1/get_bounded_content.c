static char *get_bounded_content(
  char **pz,         /* Content taken from here */
  int *pLen,         /* Number of bytes of data in (*pz)[] */
  char *zBoundry,    /* Boundry text marking the end of content */
  int *pnContent     /* Write the size of the content here */
){
  char *z = *pz;
  int len = *pLen;
  int i;
  int nBoundry = strlen(zBoundry);
  *pnContent = len;
  for(i=0; i<len; i++){
    if( z[i]=='\n' && strncmp(zBoundry, &z[i+1], nBoundry)==0 ){
      if( i>0 && z[i-1]=='\r' ) i--;
      z[i] = 0;
      *pnContent = i;
      i += nBoundry;
      break;
    }
  }
  *pz = &z[i];
  get_line_from_string(pz, pLen);
  return z;      
}