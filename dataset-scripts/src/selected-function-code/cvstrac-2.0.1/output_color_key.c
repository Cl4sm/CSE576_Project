void output_color_key(const char *zClrKey, int horiz, char *zTabArgs){
  int i, j, k;
  char *zSafeKey, *zToFree;
  while( isspace(*zClrKey) ) zClrKey++;
  if( zClrKey[0]==0 ) return;
  @ <table %s(zTabArgs)>
  if( horiz ){
    @ <tr>
  }
  zToFree = zSafeKey = mprintf("%h", zClrKey);
  while( zSafeKey[0] ){
    while( isspace(*zSafeKey) ) zSafeKey++;
    for(i=0; zSafeKey[i] && !isspace(zSafeKey[i]); i++){}
    for(j=i; isspace(zSafeKey[j]); j++){}
    for(k=j; zSafeKey[k] && zSafeKey[k]!='\n' && zSafeKey[k]!='\r'; k++){}
    if( !horiz ){
      cgi_printf("<tr bgcolor=\"%.*s\"><td>%.*s</td></tr>\n",
        i, zSafeKey, k-j, &zSafeKey[j]);
    }else{
      cgi_printf("<td bgcolor=\"%.*s\">%.*s</td>\n",
        i, zSafeKey, k-j, &zSafeKey[j]);
    }
    zSafeKey += k;
  }
  free(zToFree);
  if( horiz ){
    @ </tr>
  }
  @ </table>
}