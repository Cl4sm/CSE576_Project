static int git_diff_versions(
  const char *oldVersion,
  const char *newVersion,
  const char *zFile
){
  char *zCmd;
  FILE *in;
  
  zCmd = mprintf("GIT_DIR='%s' git-diff-tree -t -p -r '%s' '%s' 2>/dev/null",
                 db_config("cvsroot",""),
                 quotable_string(oldVersion),
                 quotable_string(newVersion), quotable_string(zFile));
  
  in = popen(zCmd, "r");
  free(zCmd);
  if( in==0 ) return -1;
  
  output_pipe_as_html(in,1);
  pclose(in);
  
  return 0;
}