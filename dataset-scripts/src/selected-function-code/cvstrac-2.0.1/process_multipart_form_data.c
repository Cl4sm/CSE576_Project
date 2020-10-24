static void process_multipart_form_data(char *z, int len){
  char *zLine;
  int nArg, i;
  char *zBoundry;
  char *zValue;
  char *zName = 0;
  int showBytes = 0;
  char *azArg[50];

  zBoundry = get_line_from_string(&z, &len);
  if( zBoundry==0 ) return;
  while( (zLine = get_line_from_string(&z, &len))!=0 ){
    if( zLine[0]==0 ){
      int nContent = 0;
      zValue = get_bounded_content(&z, &len, zBoundry, &nContent);
      if( zName && zValue ){
        cgi_set_parameter_nocopy(zName, zValue);
        if( showBytes ){
          cgi_set_parameter_nocopy(mprintf("%s:bytes", zName),
               mprintf("%d",nContent));
        }
      }
      zName = 0;
      showBytes = 0;
    }else{
      nArg = tokenize_line(zLine, sizeof(azArg)/sizeof(azArg[0]), azArg);
      for(i=0; i<nArg; i++){
        int c = tolower(azArg[i][0]);
        if( c=='c' && strcasecmp(azArg[i],"content-disposition:")==0 ){
          i++;
        }else if( c=='n' && strcasecmp(azArg[i],"name=")==0 ){
          zName = azArg[++i];
        }else if( c=='f' && strcasecmp(azArg[i],"filename=")==0 ){
          char *z = azArg[++i];
          if( zName && z ){
            cgi_set_parameter_nocopy(mprintf("%s:filename",zName), z);
          }
          showBytes = 1;
        }else if( c=='c' && strcasecmp(azArg[i],"content-type:")==0 ){
          char *z = azArg[++i];
          if( zName && z ){
            cgi_set_parameter_nocopy(mprintf("%s:mimetype",zName), z);
          }
        }
      }
    }
  }        
}