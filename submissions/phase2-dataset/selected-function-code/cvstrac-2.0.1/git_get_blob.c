static char *git_get_blob(
  const char *zGitDir,
  const char *zTreeish,
  const char* zPath
){
  FILE *in;
  char zLine[PATH_MAX*2];
  char *zCmd;

  if( zTreeish==0 || zTreeish[0]==0 || zPath==0 || zPath[0]==0 ) return 0;
    
  zCmd = mprintf("GIT_DIR='%s' git-ls-tree -r '%s' '%s'", zGitDir,
                 quotable_string(zTreeish), quotable_string(zPath));
  in = popen(zCmd,"r");
  if( in==0 ){
    err_pipe("Reading tree",zCmd);
    return 0;
  }
  free(zCmd);

  while( !feof(in) && !ferror(in) ){
    char zMode[100], zType[100], zObject[100];

    if( 0==fgets(zLine,sizeof(zLine),in) ) break;

    sscanf(zLine, "%8s %90s %50[0-9a-fA-F]", zMode, zType, zObject);

    if( !strcmp(zType,"blob") ){
      return strdup(zObject);
    }
  }
  return 0;
}