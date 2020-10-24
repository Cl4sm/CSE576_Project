int output_trim_message(char *zMsg, int mnChar, int mxChar){
  int i, j, k, n;
  int brkpt = 0;    /* First paragraph break after zMsg[mnChar] */

  if( zMsg==0 ) return 0;
  for(i=0; isspace(zMsg[i]); i++){}
  i += is_list_elem(&zMsg[i], &k);
  for(j=0; zMsg[i]; i++){
    int c = zMsg[i];
    if( c=='\n' ){
      if( j>mnChar && is_blank_line(&zMsg[i+1]) && brkpt==0 ){
        brkpt = j;
      }
      c = ' ';
      if( (n = is_list_elem(&zMsg[i+1],&k))>0 ) {
        zMsg[i+1] = '+';
        memset(&zMsg[i+2],' ',n-1);
      }
    }
    if( isspace(c) ){
      if( j>=mxChar ){
        zMsg[j] = 0;
        if( brkpt>0 ) zMsg[brkpt] = 0;
        return 1;
      }
      if( j>0 && !isspace(zMsg[j-1]) ){
        zMsg[j++] = ' ';
      }
    }else{
      zMsg[j++] = c;
    }
  }
  zMsg[j] = 0;
  return 0;
}