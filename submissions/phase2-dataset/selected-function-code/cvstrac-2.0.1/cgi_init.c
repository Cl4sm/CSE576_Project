void cgi_init(void){
  char *z;
  char *zType;
  int len;
  z = getenv("QUERY_STRING");
  if( z ){
    z = mprintf("%s",z);
    add_param_list(z, '&');
  }

  z = getenv("CONTENT_LENGTH");
  len = z ? atoi(z) : 0;
  zType = getenv("CONTENT_TYPE");
  if( len>0 && zType && 
    (strcmp(zType,"application/x-www-form-urlencoded")==0 
      || strncmp(zType,"multipart/form-data",19)==0) ){
    z = malloc( len+1 );
    if( z==0 ) exit(1);
    len = fread(z, 1, len, stdin);
    z[len] = 0;
    if( zType[0]=='a' ){
      add_param_list(z, '&');
    }else{
      process_multipart_form_data(z, len);
    }
  }

  z = getenv("HTTP_COOKIE");
  if( z ){
    z = mprintf("%s",z);
    add_param_list(z, ';');
  }
}