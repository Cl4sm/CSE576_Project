static int tokenize_line(char *z, int mxArg, char **azArg){
  int i = 0;
  while( *z ){
    while( isspace(*z) || *z==';' ){ z++; }
    if( *z=='"' && z[1] ){
      *z = 0;
      z++;
      if( i<mxArg-1 ){ azArg[i++] = z; }
      while( *z && *z!='"' ){ z++; }
      if( *z==0 ) break;
      *z = 0;
      z++;
    }else{
      if( i<mxArg-1 ){ azArg[i++] = z; }
      while( *z && !isspace(*z) && *z!=';' && *z!='"' ){ z++; }
      if( *z && *z!='"' ){
        *z = 0;
        z++;
      }
    }
  }
  azArg[i] = 0;
  return i;
}