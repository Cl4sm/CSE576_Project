static char *EncodeHttp(const char *zIn, int n, int encodeSlash){
  int c;
  int i = 0;
  int count = 0;
  char *zOut;
  int other;
# define IsSafeChar(X)  \
     (isalnum(X) || (X)=='.' || (X)=='$' || (X)=='-' || (X)=='_' || (X)==other)

  if( zIn==0 ) return 0;
  if( n<0 ) n = strlen(zIn);
  other = encodeSlash ? 'a' : '/';
  while( i<n && (c = zIn[i])!=0 ){
    if( IsSafeChar(c) || c==' ' ){
      count++;
    }else{
      count += 3;
    }
    i++;
  }
  i = 0;
  zOut = malloc( count+1 );
  if( zOut==0 ) return 0;
  while( n-->0 && (c = *zIn)!=0 ){
    if( IsSafeChar(c) ){
      zOut[i++] = c;
    }else if( c==' ' ){
      zOut[i++] = '+';
    }else{
      zOut[i++] = '%';
      zOut[i++] = "0123456789ABCDEF"[(c>>4)&0xf];
      zOut[i++] = "0123456789ABCDEF"[c&0xf];
    }
    zIn++;
  }
  zOut[i] = 0;
  return zOut;
}