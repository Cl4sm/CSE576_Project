static void add_param_list(char *z, int terminator){
  while( *z ){
    char *zName;
    char *zValue;
    while( isspace(*z) ){ z++; }
    zName = z;
    while( *z && *z!='=' && *z!=terminator ){ z++; }
    if( *z=='=' ){
      *z = 0;
      z++;
      zValue = z;
      while( *z && *z!=terminator ){ z++; }
      if( *z ){
        *z = 0;
        z++;
      }
      dehttpize(zValue);
      cgi_set_parameter_nocopy(zName, zValue);
    }else{
      if( *z ){ *z++ = 0; }
      cgi_set_parameter_nocopy(zName, "");
    }
  }
}