int write_to_temp(const char *zText, char *zFile, size_t nLen){
  FILE *f;
  int fd;
  char *zTmp = getenv("TMPDIR");        /* most Unices */
  if( zTmp==0 ) zTmp = getenv("TEMP");  /* Windows/Cygwin */
  if( zTmp==0 ) zTmp = "/tmp";

  bprintf(zFile,nLen,"%s/cvstrac_XXXXXX", zTmp);
  fd = mkstemp(zFile);
  if( fd<0 || 0==(f = fdopen(fd, "w+")) ){
    if( fd>=0 ){
      unlink(zFile);
      close(fd);
    }
    zFile[0] = 0;
    return 1;
  }
  fwrite(zText, 1, strlen(zText), f);
  fprintf(f, "\n");
  fclose(f);
  return 0;
}