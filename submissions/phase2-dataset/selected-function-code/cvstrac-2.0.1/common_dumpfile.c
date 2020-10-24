int common_dumpfile(
  const char *zCmd,
  const char *zVersion,
  const char *zFile,
  int bRaw
){
  FILE *in;
  char zLine[2000];

  if( !g.okCheckout ) return -1;
  if( bRaw ){
    in = popen(zCmd, "r");
    if( in==0 ) return -1;

    while( !feof(in) ){
      int amt = fread(zLine, 1, sizeof(zLine), in);
      if( amt<=0 ) break;
      cgi_append_content(zLine, amt);
    }
    pclose(in);
  }else{
    const char* zFilter = get_filefilter(zVersion,zFile);
    char *zMyCmd = mprintf("%s %s", zCmd, zFilter ? zFilter : "");
    if( zMyCmd==0 ) return -1;

    in = popen(zMyCmd, "r");
    free(zMyCmd);
    if( in==0 ) return -1;

    if( zFilter ){
      output_pipe_as_html(in,0);
    }else{
      @ <pre>
      while( fgets(zLine, sizeof(zLine), in) ){
        cgi_printf("%h",zLine);
      }
      @ </pre>
    }
    pclose(in);
  }

  return 0;
}