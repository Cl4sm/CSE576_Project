int output_tool(FILE *in){
  /* Output the result of the command.  If the first non-whitespace
  ** character is "<" then assume the command is giving us HTML.  In
  ** that case, do no translation.  If the first non-whitespace character
  ** is anything other than "<" then assume the output is plain text.
  ** Convert this text into HTML.
  */
  char zLine[2000];
  char *zFormat = 0;
  int i, n=0;
  while( fgets(zLine, sizeof(zLine), in) ){
    n += strlen(zLine);
    for(i=0; isspace(zLine[i]); i++){}
    if( zLine[i]==0 ) continue;
    if( zLine[i]=='<' ){
      zFormat = "%s";
    }else{
      @ <pre>
      zFormat = "%h";
    }
    cgi_printf(zFormat, zLine);
    break;
  }
  while( fgets(zLine, sizeof(zLine), in) ){
    n += strlen(zLine);
    cgi_printf(zFormat, zLine);
  }
  if( zFormat && zFormat[1]=='h' ){
    @ </pre>
  }
  return n;
}