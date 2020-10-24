static int git_ingest_commit_tree(const char *zGitDir, const char *zCommit){
  int i;
  char **azParents = 0;
  char zCur[50];
  int nNew = 0;

  strncpy(zCur,zCommit,sizeof(zCur));
  
  while( zCur[0]!=0 ){
    if( db_exists("SELECT 1 FROM filechng WHERE vers='%s' "
                  "UNION ALL "
                  "SELECT 1 FROM ci WHERE vers='%s'", zCur, zCur)){
      /* Seen this already, or it's already one of the commits we're going
      ** to ingest.
      */
      break;
    }
    
    {
      /* Read the commit in a different scope so all the large static
      ** buffers aren't holding stack space when we recurse.
      */
      char zComment[10000];
      char zAuthor[100];
      int nDate = 0;

      azParents = git_read_commit(zGitDir,zCur,zAuthor,&nDate,
                                  zComment,sizeof(zComment));
      if( nDate==0 ) break;

      db_execute("INSERT INTO ci(vers,date,author,message,prevvers) "
                 "VALUES('%s',%d,'%q','%q','%s');",
                 zCur, nDate, zAuthor, zComment, azParents ? azParents[0] : "");

      nNew ++;
    }

    /* we'll want to break out if we're at a root object */
    zCur[0] = 0;
    if( azParents && azParents[0] ){
      /* If there's more than one parent, recurse on the extras. Otherwise,
      ** just update our "current" counter. This minimizes actual recursions
      ** so multi-parent commits don't end up blowing our stack.
      */
      for(i=1; azParents[i]; i++){
        nNew += git_ingest_commit_tree(zGitDir,azParents[i]);
      }

      strncpy(zCur,azParents[0],sizeof(zCur));

      db_query_free(azParents);
    }
  }

  return nNew;
}