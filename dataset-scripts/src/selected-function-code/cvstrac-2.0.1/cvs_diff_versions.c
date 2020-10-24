static int cvs_diff_versions(
  const char *oldVersion,
  const char *newVersion,
  const char *zRelFile
){
  const char *zTemplate;
  char *zCmd;
  FILE *in;
  const char *azSubst[10];
  char *zFile;

  if( zRelFile==0 ) return -1;

  zFile = real_path_name(zRelFile);
  if( zFile==0 ) return -1;

  /* Check file for binary keyword */
  if( has_binary_keyword(zFile) ){
    free(zFile);
    @ <p>
    @ %h(zRelFile) is a binary file
    @ </p>
    return 0; /* Don't attempt to compare binaries, but it's not a failure */
  }

  if( oldVersion[0]==0 ){
    @ %h(zRelFile)  -> %h(newVersion)
  }else{
    @ %h(zRelFile)  %h(oldVersion) -> %h(newVersion)
  }
  cgi_append_content("\n", 1);

  /* Find the command used to compute the file difference.
  */
  azSubst[0] = "F";
  azSubst[1] = zFile;
  if( oldVersion[0]==0 ){
    zTemplate = db_config("filelist",
      "co -q -p'%V' '%F' | diff -c /dev/null - 2>/dev/null");
    azSubst[2] = "V";
    azSubst[3] = newVersion;
    azSubst[4] = "RP";
    azSubst[5] = db_config("cvsroot", "");
    azSubst[6] = 0;
  }else{
    zTemplate = db_config("filediff","rcsdiff -q '-r%V1' '-r%V2' -u '%F' 2>/dev/null");
    azSubst[2] = "V1";
    azSubst[3] = oldVersion;
    azSubst[4] = "V2";
    azSubst[5] = newVersion;
    azSubst[6] = "RP";
    azSubst[7] = db_config("cvsroot", "");
    azSubst[8] = 0;
  }
  zCmd = subst(zTemplate, azSubst);
  free(zFile);
  in = popen(zCmd, "r");
  free(zCmd);
  if( in==0 ) return -1;

  output_pipe_as_html(in,0);
  pclose(in);

  return 0;
}