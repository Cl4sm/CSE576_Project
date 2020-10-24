int execute_tool(
  const char *zTool,
  const char *zAction,
  const char *zContent,
  const char **azTSubst
){
  FILE *in;
  char *zCmd;
  const char *azSubst[32];
  int n = 0;
  char zF[200];

  db_add_functions();
  if( !db_exists("SELECT 1 FROM tool,user "
                 "WHERE tool.name='%q' AND user.id='%q' "
                 "      AND cap_and(user.capabilities,tool.perms)!=''",
                 zTool, g.zUser)){
    return -1;
  }

  while(azTSubst[n]){
    azSubst[n] = azTSubst[n];
    n++;
  }

  azSubst[n++] = "RP";
  azSubst[n++] = db_config("cvsroot", "");
  azSubst[n++] = "B";
  azSubst[n++] = g.zBaseURL;
  azSubst[n++] = "P";
  azSubst[n++] = g.zName;
  azSubst[n++] = "U";
  azSubst[n++] = g.zUser;
  azSubst[n++] = "N";
  azSubst[n++] = mprintf("%d",time(0));
  azSubst[n++] = "T";
  azSubst[n++] = quotable_string(zTool);
  azSubst[n++] = "UC";
  azSubst[n++] = db_short_query("SELECT capabilities FROM user "
                                "WHERE id='%q'",g.zUser);
  zF[0] = 0;
  if( zContent ){
    /* content is special. If provided, we need to write it to a temp file
    ** so that it can be given to the program via the %C subst.
    */
    if( !write_to_temp(zContent,zF,sizeof(zF)) ){
      azSubst[n++] = "C";
      azSubst[n++] = zF;
    }
  }

  azSubst[n] = 0;

  zCmd = subst(zAction, azSubst);

  in = popen(zCmd,"r");
  free(zCmd);
  if( in==0 ) {
    if( zF[0] ) unlink(zF);
    return -1;
  }

  n = output_tool(in);

  pclose(in);
  if( zF[0] ) unlink(zF);

  return n;
}