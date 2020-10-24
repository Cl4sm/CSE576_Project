void diff_strings(int nContext,const char *zString1, const char *zString2){
  char zF1[200], zF2[200];

  zF1[0] = zF2[0] = 0;
  if( !write_to_temp(zString1, zF1,sizeof(zF1))
       && !write_to_temp(zString2, zF2,sizeof(zF2))
  ){
    char *zCmd;
    FILE *p;
    char zLine[2000];
    int cnt = 0;
    zCmd = mprintf("diff -C %d -b '%s' '%s' 2>&1", nContext,
                   quotable_string(zF1), quotable_string(zF2));
    if( zCmd ){
      p = popen(zCmd, "r");

      if( p ){
        @ <pre>
        while( fgets(zLine, sizeof(zLine), p) ){
          cnt++;
          if( cnt>3 ) cgi_printf("%h", zLine);
        }
        @ </pre>
        pclose(p);
      }else{
        common_err("Unable to diff temporary files");
      }
      free(zCmd);
    }
  } else {
    common_err("Unable to create a temporary file");
  }
  if( zF1[0] ) unlink(zF1);
  if( zF2[0] ) unlink(zF2);
}