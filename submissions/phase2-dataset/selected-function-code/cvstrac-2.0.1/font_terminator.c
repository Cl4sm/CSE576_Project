static int font_terminator(const char *z, int c, int n){
  int seenNL = 0;
  int cnt = 0;
  if( isspace(*z) || *z==0 || *z==c ) return 0;
  z++;
  while( *z ){
    if( *z==c && !isspace(z[-1]) ){
      cnt++;
      if( cnt==n && is_eow(&z[1],0) ){
        return 1;
      }
    }else{
      cnt = 0;
      if( *z=='\n' ){
        if( seenNL ) return 0;
        seenNL = 1;
      }else if( !isspace(*z) ){
        seenNL = 0;
      }
    }
    z++;
  }
  return 0;
}