void cgi_optionmenu(int in, const char *zP, const char *zD, ...){
  va_list ap;
  char *zName, *zVal;
  int dfltSeen = 0;
  cgi_printf("%*s<select size=1 name=\"%s\">\n", in, "", zP);
  va_start(ap, zD);
  while( (zName = va_arg(ap, char*))!=0 && (zVal = va_arg(ap, char*))!=0 ){
    if( strcmp(zVal,zD)==0 ){ dfltSeen = 1; break; }
  }
  va_end(ap);
  if( !dfltSeen ){
    if( zD[0] ){
      cgi_printf("%*s<option value=\"%h\" selected>%h</option>\n",
        in+2, "", zD, zD);
    }else{
      cgi_printf("%*s<option value=\"\" selected>&nbsp;</option>\n", in+2, "");
    }
  }
  va_start(ap, zD);
  while( (zName = va_arg(ap, char*))!=0 && (zVal = va_arg(ap, char*))!=0 ){
    if( zName[0] ){
      cgi_printf("%*s<option value=\"%h\"%s>%h</option>\n",
        in+2, "",
        zVal,
        strcmp(zVal, zD) ? "" : " selected",
        zName
      );
    }else{
      cgi_printf("%*s<option value=\"\"%s>&nbsp;</option>\n",
        in+2, "",
        strcmp(zVal, zD) ? "" : " selected"
      );
    }
  }
  va_end(ap);
  cgi_printf("%*s</select>\n", in, "");
}