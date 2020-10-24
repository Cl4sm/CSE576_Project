static void row_content(
  const char *zName,
  const char *zSortUrl,
  int nCol,
  char **az
){
  if( (nCol%2)==0 ){
    @ <tr bgcolor="%s(BG4)" class="bkgnd4">
  }else{
    @ <tr>
  }
  
  if( atoi(az[0])==1 ){
    @ <td colspan="3">
    @ <a href="dirview?d=%T(zName)%T(az[1])&%s(zSortUrl)">
    common_icon("dir");
    @ </a>&nbsp;<a href="dirview?d=%T(zName)%T(az[1])&%s(zSortUrl)">%h(az[1])/</a></td>
    @ <td valign="middle" width="10%%">%h(file_age_to_text(atoi(az[5])))</td>
    @ <td></td>
  }else{
    @ <td valign="middle" width="30%%">
    @ <a href="rlog?f=%T(zName)%T(az[1])">
    if( atoi(az[3])==2 ){
      common_icon("del");
    }else{
      common_icon("file");
    }
    @ </a>&nbsp;<a href="rlog?f=%T(zName)%T(az[1])">%h(az[1])</a></td>
    @ <td valign="middle" width="5%%">
    @ <a href="fileview?f=%T(zName)%T(az[1])&v=%T(az[2])">
    @ %h(printable_vers(az[2]))</a></td> 
    @ <td valign="middle" width="8%%">%z(format_user(az[4]))</td>
    @ <td valign="middle" width="8%%">%h(file_age_to_text( atoi(az[5]) ))</td>
    @ <td valign="middle">
    
    if( output_trim_message(az[6], MN_CKIN_MSG, MX_CKIN_MSG) ){
      output_formatted(az[6], 0);
      @ &nbsp;[...]
    }else{
      output_formatted(az[6], 0);
    }
    @ </td>
  }
  @</tr>
}