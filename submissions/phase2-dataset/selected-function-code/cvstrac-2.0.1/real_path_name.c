static char *real_path_name(const char *zPath){
  char *zName, *zBase, *zDir;
  char *zReal;
  const char *zRoot;
  int i, j;

  zRoot = db_config("cvsroot", 0);
  if( zRoot==0 ){ return 0; }
  zName = mprintf("%s", zPath);
  for(i=j=0; zName[i]; i++){
    if( zName[i]=='/' ){
      while( zName[i+1]=='/' ){ i++; }
      if( zName[i+1]==0 ) break;
    }
    zName[j++] = zName[i];
  }
  zName[j] = 0;
  zDir = mprintf("%s/%s", zRoot, zName);
  zBase = strrchr(zDir, '/');
  if( zBase==0 ){
    zBase = zDir;
    zDir = strdup(".");
  }else{
    *zBase = 0;
    zBase++;
  }
  zReal = find_repository_file(zDir, zBase);
  free(zName);
  free(zDir);
  return zReal;
}