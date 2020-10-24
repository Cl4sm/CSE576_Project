static void add_chng_tools(
  const char *zExcept,
  int cn,
  int ms
){
  int i;
  char *zLink;
  char **azTools;
  db_add_functions();
  azTools = db_query("SELECT tool.name FROM tool,user "
                     "WHERE tool.object='%s' AND user.id='%q' "
                     "      AND cap_and(tool.perms,user.capabilities)!=''",
                     ms?"ms":"chng", g.zUser);

  for(i=0; azTools[i]; i++){
    if( zExcept && 0==strcmp(zExcept,azTools[i]) ) continue;

    zLink = mprintf("chngtool?t=%T&cn=%d", azTools[i], cn);
    common_add_action_item(zLink, azTools[i]);
  }
}