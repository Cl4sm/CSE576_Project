static int svn_diff_chng(int cn, int bRaw){
  const char *zRoot;
  char *zRev;
  char *zCmd;
  char zLine[2000];
  FILE *in;
  
  zRev = db_short_query("SELECT vers FROM filechng WHERE cn=%d", cn);
  if( !zRev || !zRev[0] ) return -1; /* Invalid check-in number */
  
  zRoot = db_config("cvsroot", "");
  zCmd = mprintf("svnlook diff -r '%s' '%s' 2>/dev/null",
    quotable_string(zRev), quotable_string(zRoot));
  free(zRev);
  
  in = popen(zCmd, "r");
  free(zCmd);
  if( in==0 ) return -1;
  
  if( bRaw ){
    while( !feof(in) ){
      int amt = fread(zLine, 1, sizeof(zLine), in);
      if( amt<=0 ) break;
      cgi_append_content(zLine, amt);
    }
  }else{
    output_pipe_as_html(in,1);
  }
  pclose(in);
  
  return 0;
}