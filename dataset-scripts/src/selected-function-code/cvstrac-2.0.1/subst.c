char *subst(const char *zIn, const char **azSubst){
  int i, k;
  char *zOut;
  int nByte = 1;

  /* Sanitize the substitutions
  */
  for(i=0; azSubst[i]; i+=2){
    azSubst[i+1] = quotable_string(azSubst[i+1]);
  }

  /* Figure out how must space is required to hold the result.
  */
  nByte = 1;  /* For the null terminator */
  for(i=0; zIn[i]; i++){
    if( zIn[i]=='%' ){
      int c = zIn[++i];
      if( c=='%' ){
        nByte++;
      }else{
        int j, len = 0;
        for(j=0; azSubst[j]; j+=2){
          if( azSubst[j][0]!=c ) continue;
          len = strlen(azSubst[j]);
          if( strncmp(&zIn[i], azSubst[j], len)!=0 ) continue;
          break;
        }
        if( azSubst[j]==0 ){
          nByte += 2;
        }else{
          nByte += strlen(azSubst[j+1]);
          i += len - 1;
        }
      }
    }else{
      nByte++;
    }
  }

  /* Allocate a string to hold the result
  */
  zOut = malloc( nByte );
  if( zOut==0 ) exit(1);

  /* Do the substituion
  */
  for(i=k=0; zIn[i]; i++){
    if( zIn[i]=='%' ){
      int c = zIn[++i];
      if( c=='%' ){
        zOut[k++] = '%';
      }else{
        int j, len = 0;
        for(j=0; azSubst[j]; j+=2){
          if( azSubst[j][0]!=c ) continue;
          len = strlen(azSubst[j]);
          if( strncmp(&zIn[i], azSubst[j], len)!=0 ) continue;
          break;
        }
        if( azSubst[j]==0 ){
          zOut[k++] = '%';
          zOut[k++] = c;
        }else{
          strcpy(&zOut[k], azSubst[j+1]);
          k += strlen(&zOut[k]);
          i += len - 1;
        }
      }
    }else{
      zOut[k++] = zIn[i];
    }
  }
  zOut[k] = 0;
  assert( k==nByte-1 );
  return zOut;
}