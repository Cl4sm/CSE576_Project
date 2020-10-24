char *common_readfp(FILE* fp) {
  char *zContent = NULL;
  char *z;
  size_t n = 0, m = 0;
  size_t rc;

  while( fp && !feof(fp) && !ferror(fp) ) {
    if( (n+1)>=m ){
      m = m ? (m*2) : 1024;
      z = realloc(zContent, m);
      if( z==NULL ){
        if( zContent!=NULL ) free(zContent);
        return NULL;
      }
      zContent = z;
    }
    rc = fread(&zContent[n], 1, m-(n+1), fp);
    if( rc>0 ){
      n += rc;
    }
    zContent[n] = 0;
  }

  return zContent;
}