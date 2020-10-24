char *htmlize(const char *zIn, int n){
  int c;
  int i = 0;
  int count = 0;
  char *zOut;

  if( n<0 ) n = strlen(zIn);
  while( i<n && (c = zIn[i])!=0 ){
    switch( c ){
      case '<':   count += 4;       break;
      case '>':   count += 4;       break;
      case '&':   count += 5;       break;
      case '"':   count += 6;       break;
      default:    count++;          break;
    }
    i++;
  }
  i = 0;
  zOut = malloc( count+1 );
  if( zOut==0 ) return 0;
  while( n-->0 && (c = *zIn)!=0 ){
    switch( c ){
      case '<':   
        zOut[i++] = '&';
        zOut[i++] = 'l';
        zOut[i++] = 't';
        zOut[i++] = ';';
        break;
      case '>':   
        zOut[i++] = '&';
        zOut[i++] = 'g';
        zOut[i++] = 't';
        zOut[i++] = ';';
        break;
      case '&':   
        zOut[i++] = '&';
        zOut[i++] = 'a';
        zOut[i++] = 'm';
        zOut[i++] = 'p';
        zOut[i++] = ';';
        break;
      case '"':   
        zOut[i++] = '&';
        zOut[i++] = 'q';
        zOut[i++] = 'u';
        zOut[i++] = 'o';
        zOut[i++] = 't';
        zOut[i++] = ';';
        break;
      default:
        zOut[i++] = c;
        break;
    }
    zIn++;
  }
  zOut[i] = 0;
  return zOut;
}