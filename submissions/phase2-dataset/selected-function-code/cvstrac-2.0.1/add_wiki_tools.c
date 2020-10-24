static void add_wiki_tools(
  const char *zExcept,
  const char *zPage,
  time_t t1, time_t t2
){
  int i;
  char *zLink;
  char **azTools;
  db_add_functions();
  azTools = db_query("SELECT tool.name FROM tool,user "
                     "WHERE tool.object='wiki' AND user.id='%q' "
                     "      AND cap_and(tool.perms,user.capabilities)!=''",
                     g.zUser);

  for(i=0; azTools[i]; i++){
    if( zExcept && 0==strcmp(zExcept,azTools[i]) ) continue;

    if( t1 && t2 ){
      zLink = mprintf("wikitool?t=%T&p=%T&t1=%d&t2=%d",azTools[i],zPage,t1,t2);
    }else if( t1 ){
      zLink = mprintf("wikitool?t=%T&p=%T&t1=%d",azTools[i],zPage,t1);
    }else{
      zLink = mprintf("wikitool?t=%T&p=%T",azTools[i],zPage);
    }
    common_add_action_item(zLink, azTools[i]);
  }
}