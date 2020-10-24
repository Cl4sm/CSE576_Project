void cgi_handle_http_request(void){
  char *z, *zToken;
  int i;
  struct sockaddr_in remoteName;
  size_t size = sizeof(struct sockaddr_in);
  char zLine[2000];     /* A single line of input. */

  fullHttpReply = 1;
  if( fgets(zLine, sizeof(zLine), stdin)==0 ){
    malformed_request();
  }
  zToken = extract_token(zLine, &z);
  if( zToken==0 ){
    malformed_request();
  }
  if( strcmp(zToken,"GET")!=0 && strcmp(zToken,"POST")!=0
      && strcmp(zToken,"HEAD")!=0 ){
    malformed_request();
  }
  putenv("GATEWAY_INTERFACE=CGI/1.0");
  putenv(mprintf("REQUEST_METHOD=%s",zToken));
  zToken = extract_token(z, &z);
  if( zToken==0 ){
    malformed_request();
  }
  putenv(mprintf("REQUEST_URI=%s", zToken));
  for(i=0; zToken[i] && zToken[i]!='?'; i++){}
  if( zToken[i] ) zToken[i++] = 0;
  putenv(mprintf("PATH_INFO=%s", zToken));
  putenv(mprintf("QUERY_STRING=%s", &zToken[i]));
  if( getpeername(fileno(stdin), (struct sockaddr*)&remoteName, &size)>=0 ){
    putenv(mprintf("REMOTE_ADDR=%s", inet_ntoa(remoteName.sin_addr)));
  }
 
  /* Get all the optional fields that follow the first line.
  */
  while( fgets(zLine,sizeof(zLine),stdin) ){
    char *zFieldName;
    char *zVal;

    zFieldName = extract_token(zLine,&zVal);
    if( zFieldName==0 || *zFieldName==0 ) break;
    while( isspace(*zVal) ){ zVal++; }
    i = strlen(zVal);
    while( i>0 && isspace(zVal[i-1]) ){ i--; }
    zVal[i] = 0;
    for(i=0; zFieldName[i]; i++){ zFieldName[i] = tolower(zFieldName[i]); }
    if( strcmp(zFieldName,"user-agent:")==0 ){
      putenv(mprintf("HTTP_USER_AGENT=%s", zVal));
    }else if( strcmp(zFieldName,"content-length:")==0 ){
      putenv(mprintf("CONTENT_LENGTH=%s", zVal));
    }else if( strcmp(zFieldName,"referer:")==0 ){
      putenv(mprintf("HTTP_REFERER=%s", zVal));
    }else if( strcmp(zFieldName,"host:")==0 ){
      putenv(mprintf("HTTP_HOST=%s", zVal));
    }else if( strcmp(zFieldName,"content-type:")==0 ){
      putenv(mprintf("CONTENT_TYPE=%s", zVal));
    }else if( strcmp(zFieldName,"cookie:")==0 ){
      putenv(mprintf("HTTP_COOKIE=%s", zVal));
    }else if( strcmp(zFieldName,"if-none-match:")==0 ){
      putenv(mprintf("HTTP_IF_NONE_MATCH=%s", zVal));
    }else if( strcmp(zFieldName,"if-modified-since:")==0 ){
      putenv(mprintf("HTTP_IF_MODIFIED_SINCE=%s", zVal));
    }
  }

  cgi_init();
}