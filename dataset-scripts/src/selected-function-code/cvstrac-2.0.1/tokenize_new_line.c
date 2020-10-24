static char **tokenize_new_line(const char *zString){
  int nTok = 0;
  char **azToks;
  int i, j, k;

  for(i=nTok=0; zString[i]; i++){
    if( !isspace(zString[i]) ){
      nTok++;
      while( zString[i+1] && !isspace(zString[i+1]) ) i++;
    }
  }
  azToks = malloc( sizeof(char *)*(nTok+1) );
  if( azToks==0 ) return NULL;
  for(i=j=0; j<nTok && zString[i]; i++){
    if( !isspace(zString[i]) ){
      k = i+1;
      while( zString[k] && !isspace(zString[k]) ) k++;
      azToks[j++] = mprintf("%.*s",k-i,&zString[i]);
      i = k;
    }
  }
  azToks[j] = 0;

  /* sort the result list. This makes it easy to catch duplicates.
  */
  qsort(azToks, j, sizeof(char*), tok_compare);

  return azToks;
}