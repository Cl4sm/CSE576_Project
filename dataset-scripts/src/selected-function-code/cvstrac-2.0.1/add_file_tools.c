static void add_file_tools(
  const char *zExcept,
  const char *zFile,
  const char *zVers1,
  const char *zVers2
){
  int i;
  char *zLink;
  char **azTools;
  db_add_functions();
  azTools = db_query("SELECT tool.name FROM tool,user "
                     "WHERE tool.object='file' AND user.id='%q' "
                     "      AND cap_and(tool.perms,user.capabilities)!=''",
                     g.zUser);

  for(i=0; azTools[i]; i++){
    if( zExcept && 0==strcmp(zExcept,azTools[i]) ) continue;

    zLink = mprintf("filetool?t=%T&f=%T%s%T%s%T",
                          azTools[i], zFile,
                          zVers1?"&v1=":"", zVers1?zVers1:"",
                          zVers2?"&v2=":"", zVers2?zVers2:"");
    common_add_action_item(zLink, azTools[i]);
  }
}