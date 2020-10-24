static int is_markup(const char *z, Markup *pMarkup){
  int i, j;
  int nest = 1;
  if( *z!='{' ) return 0;
  for(i=1; isalpha(z[i]); i++){}
  if( z[i]=='}' ){
    pMarkup->lenTotal = i+1;
    pMarkup->lenType = i-1;
    pMarkup->lenKey = i-1;
    pMarkup->lenArgs = i-1;
    pMarkup->zType = &z[1];
    pMarkup->zKey = &z[1];
    pMarkup->zArgs = &z[1];
    return 1;
  }
  if( z[i]!=':' ) return 0;
  pMarkup->lenType = i-1;
  pMarkup->zType = &z[1];
  i++;
  while( isspace(z[i]) && z[i]!='\n' ){ i++; }
  if( z[i]==0 || z[i]=='\n' ) return 0;
  j = i;
  pMarkup->zKey = &z[i];
  while( z[i] && !isspace(z[i]) ){
    if( z[i]=='}' ) nest--;
    if( z[i]=='{' ) nest++;
    if( nest==0 ) break;
    i++;
  }
  if( z[i]==0 || z[i]=='\n' ) return 0;
  pMarkup->lenKey = i - j;
  if( nest==0 ){
    pMarkup->lenArgs = i - j;
    pMarkup->lenTotal = i+1;
    pMarkup->zArgs = pMarkup->zKey;
    return 1;
  }
  while( isspace(z[i]) && z[i]!='\n' ){ i++; }
  if( z[i]=='\n' || z[i]==0 ) return 0;
  j = i;
  while( z[i] && z[i]!='\n' ){
    if( z[i]=='}' ) nest--;
    if( z[i]=='{' ) nest++;
    if( nest==0 ) break;
    i++;
  }
  if( z[i]!='}' || nest>0 ) return 0;
  pMarkup->zArgs = &z[j];
  pMarkup->lenArgs = i - j;
  pMarkup->lenTotal = i+1;
  return 1;
}