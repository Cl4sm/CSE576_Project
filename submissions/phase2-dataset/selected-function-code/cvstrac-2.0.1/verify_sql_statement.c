char *verify_sql_statement(char *zSql){
  int i;

  /* First make sure the SQL is a single query command by verifying that
  ** the first token is "SELECT" and that there are no unquoted semicolons.
  */
  for(i=0; isspace(zSql[i]); i++){}
  if( strncasecmp(&zSql[i],"select",6)!=0 ){
    return mprintf("The SQL must be a SELECT statement");
  }
  for(i=0; zSql[i]; i++){
    if( zSql[i]==';' ){
      int bad;
      int c = zSql[i+1];
      zSql[i+1] = 0;
      bad = sqlite3_complete(zSql);
      zSql[i+1] = c;
      if( bad ){
        /* A complete statement basically means that an unquoted semi-colon
        ** was found. We don't actually check what's after that.
        */
        return mprintf("Semi-colon detected! "
                       "Only a single SQL statement is allowed");
      }
    }
  }
  return 0;  
}