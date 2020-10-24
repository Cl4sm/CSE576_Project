static void column_header(
  const char *zNameNS,
  char zFld,
  const char *zField,
  const char *zColumn
){
  int set = (zFld==zField[0]);
  int desc = P("desc")!=0;
  const char *zDesc = set ? (desc ? "" : "&desc" ) : "";

  /* Clicking same column header 3 times in a row resets any sorting.
  */
  if(set && desc){
    @ <th align="left" bgcolor="%s(BG1)" class="bkgnd1">
    @   <a href="dirview?d=%T(zNameNS)">%h(zColumn)</a></th>
    return;
  }
  if(set){
    @ <th align="left" bgcolor="%s(BG1)" class="bkgnd1"><a
  }else{
    @ <th align="left"><a
  }
  @     href="dirview?d=%T(zNameNS)&o=%s(zField)%s(zDesc)">%h(zColumn)</a></th>
}