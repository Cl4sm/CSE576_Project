static int is_list_elem(const char *z, int *pLevel){
  int type;
  int depth;
  const char *zStart = z;
  if( isdigit(*z) ){
    z++;
    while( isdigit(*z) ){ z++; }
    type = -1;
  }else if( *z=='*' || *z=='_' ){
    z++;
    type = +1;
  }else{
    *pLevel = 0;
    return 0;
  }
  depth = 0;
  while( *z==':' ){ z++; depth++; }
  while( isspace(*z) && *z!='\n' ){ z++; }
  if( depth==0 || depth>10 || *z==0 || *z=='\n' ){
    *pLevel = 0;
    return 0;
  }
  if( type<0 ){ 
    *pLevel = -depth;
  }else{
    *pLevel = depth;
  }
  return z - zStart;
}