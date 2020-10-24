static void check_path(int nErr, const char *zProg){
  char *zPath;
  char *zBuf;
  char *z, *zEnd;
  int size;
  z = getenv("PATH");
  if( z==0 ){
    error_init(&nErr);
    @ <li><p>No PATH environment variable</p></li>
    error_finish(nErr);
  }
  size = strlen(zProg) + strlen(z) + 2;
  zPath = malloc( size*2 );
  if( zPath==0 ){
    error_init(&nErr);
    @ <li><p>Out of memory!</p></li>
    error_finish(nErr);
  }
  strcpy(zPath, z);
  zBuf = &zPath[size];
  for(z=zPath; z && *z; z=zEnd){
    zEnd = strchr(z,':');
    if( zEnd ){
      zEnd[0] = 0;
      zEnd++;
    }else{
      zEnd = &z[strlen(z)];
    }
    bprintf(zBuf, size, "%s/%s", z, zProg);
    if( access(zBuf,X_OK)==0 ){
      free(zPath);
      return;
    }
  }
  error_init(&nErr);
  @ <li><p>Unable to locate program "<b>%h(zProg)</b>".
  @ (uid=%d(getuid()), PATH=%h(getenv("PATH")))</p></li>
  error_finish(nErr);
}