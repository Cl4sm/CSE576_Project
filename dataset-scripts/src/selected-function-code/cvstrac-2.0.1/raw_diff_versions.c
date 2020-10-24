static void raw_diff_versions(
  const char *oldVersion,
  const char *newVersion,
  const char *zRelFile
){
  const char *zTemplate;
  char *zCmd;
  FILE *in;
  const char *azSubst[16];
  const char *zRoot = db_config("cvsroot", 0);

  char *zFile = find_repository_file(zRoot, zRelFile);
  if( zFile==0 ) return;

  /* Check file for binary keyword. cvs diff doesn't handle binaries
  ** either, so no big loss.
  */
  if( has_binary_keyword(zFile) ){
    free(zFile);
    return;
  }

  /* Find the command used to compute the file difference.
  */
  azSubst[0] = "F";
  azSubst[1] = zFile;
  azSubst[2] = "R";
  azSubst[3] = zRelFile;
  if( is_dead_revision(zRelFile,oldVersion) ){
    zTemplate = "co -q -kk -p'%V' '%F' | diff -u /dev/null - -L'%R' 2>/dev/null";
    azSubst[4] = "V";
    azSubst[5] = newVersion;
    azSubst[6] = 0;
  }else if( is_dead_revision(zRelFile,newVersion) ){
    zTemplate = "co -q -kk -p'%V' '%F' | diff -u - /dev/null -L'%R' 2>/dev/null";
    azSubst[4] = "V";
    azSubst[5] = oldVersion;
    azSubst[6] = 0;
  }else{
    zTemplate = "rcsdiff -q -kk '-r%V1' '-r%V2' -u '%F' 2>/dev/null";
    azSubst[4] = "V1";
    azSubst[5] = oldVersion;
    azSubst[6] = "V2";
    azSubst[7] = newVersion;
    azSubst[8] = 0;
  }
  zCmd = subst(zTemplate, azSubst);

  /* patch doesn't need to guess filenames if we give it an index line.
  ** Some extra cvs-like information doesn't hurt, either.
  */
  cgi_printf("Index: %s\n", zRelFile);
  cgi_printf("RCS File: %s\n", zFile );
  cgi_printf("%s\n", zCmd );

  free(zFile);
  in = popen(zCmd, "r");
  free(zCmd);
  if( in==0 ) return;

  while( !feof(in) && !ferror(in) ){
    char zBuf[1024];
    size_t n = fread( zBuf,1,sizeof(zBuf),in );
    if( n > 0 ){
      cgi_append_content(zBuf,n);
    }
  }
  pclose(in);
}