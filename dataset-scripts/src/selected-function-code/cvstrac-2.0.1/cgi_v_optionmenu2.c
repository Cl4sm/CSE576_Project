void cgi_v_optionmenu2(
  int in,              /* Indent by this amount */
  const char *zP,      /* The query parameter name */
  const char *zD,      /* Default value */
  const char **az      /* NULL-terminated list of allowed values */
){
  const char *zVal;
  int i;
  cgi_printf("%*s<select size=1 name=\"%s\">\n", in, "", zP);
  for(i=0; az[i]; i+=2){
    if( strcmp(az[i],zD)==0 ) break;
  }
  if( az[i]==0 ){
    if( zD[0]==0 ){
      cgi_printf("%*s<option value=\"\" selected>&nbsp;</option>\n",
       in+2, "");
    }else{
      cgi_printf("%*s<option value=\"%h\" selected>%h</option>\n",
       in+2, "", zD, zD);
    }
  }
  while( (zVal = *(az++))!=0  ){
    const char *zName = *(az++);
    if( zName[0] ){
      cgi_printf("%*s<option value=\"%h\"%s>%h</option>\n",
        in+2, "",
        zVal,
        strcmp(zVal, zD) ? "" : " selected",
        zName
      );
    }else{
      cgi_printf("%*s<option value=\"%h\"%s>&nbsp;</option>\n",
        in+2, "",
        zVal,
        strcmp(zVal, zD) ? "" : " selected"
      );
    }
  }
  cgi_printf("%*s</select>\n", in, "");
}