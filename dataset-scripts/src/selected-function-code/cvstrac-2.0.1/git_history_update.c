static int git_history_update(int isReread){
  const char *zRoot;
  char **azResult;
  int cn;
  int nOldSize = 0;
  int i;
  int nNewRevs;

  db_execute("BEGIN");

  /* Get the path to local repository and last revision number we have in db
   * If there's no repository defined, bail and wait until the admin sets one.
  */
  zRoot = db_config("cvsroot","");
  if( zRoot[0]==0 ) return 0;

  nOldSize = atoi(db_config("historysize","0"));

  /* When the historysize is zero, it means we're in for a fresh start or we've
  ** restarted at the beginning. In either case, we go with an empty HEADS
  ** and TAGS tables.
  */
  if( nOldSize==0 ){
    sqlite3 *pDb = db_open();
    char *zErrMsg = 0;
    char *zHead;

    zHead = mprintf("%s/HEAD");
    if( !access(zHead,R_OK) ){
      /* no HEAD in the project directory means it's probably _not_ a GIT
      ** repository.
      */
      int nErr = 0;
      error_init(&nErr);
      @ <b>Error</b>
      @ <li><p>Unable to locate:
      @ <blockquote><pre>
      @ %h(zHead)
      @ </pre></blockquote></p></li>
      @ Are you sure this is a GIT repository?
      error_finish(nErr);
      db_execute("COMMIT;");
      return -1;
    }

    /* If it doesn't succeed, hope it's just a "already exists" error because we
    ** don't seem to have return codes accurate enough to determine if the table
    ** add failed. If the table already exists, it _will_ fail.
    */
    sqlite3_exec(pDb,"CREATE TABLE "
                     "heads(name text primary key, "
                     "      object text,cn,seen,UNIQUE(name));",
                     0, 0, &zErrMsg);

    sqlite3_exec(pDb, "CREATE TABLE "
                      "tags(name text primary key, "
                      "     object text,cn,seen,UNIQUE(name));",
                      0, 0, &zErrMsg);

    /* Make sure they're empty. We're starting fresh */
    db_execute("DELETE FROM heads; DELETE from tags");
  }

  /* git has multiple "heads", each representing a different
  ** branch. Changes may occur in any of them and it's most efficient
  ** just to check each one separately for new commits, _then_ to combine
  ** everything into one merged linear sequence
  */

  db_execute( "CREATE TEMP TABLE ci(vers,date,author,message,prevvers);");

  nNewRevs = git_read_refs(zRoot,"heads");
  nNewRevs += git_read_refs(zRoot,"tags");

  if( nNewRevs==0 ) {
    git_update_refs("heads");
    git_update_refs("tags");
    db_execute("COMMIT");
    return 0;
  }

  /* That filled the ci table, but we dont't actually generate any CHNG
  ** or FILECHNG entries because walking the revision tree from multiple
  ** leaf nodes isn't going to give us a nice ordering. In fact, the most
  ** recent changes would have lower change numbers than the oldest, among
  ** other things.
  **
  ** Now we turn each revision into a list of files and generate the CHNG,
  ** FILE and FILECHNG records
  */

  azResult = db_query("SELECT vers,date,author,message,prevvers "
                      "FROM ci ORDER BY date");
  assert(azResult);
  for(cn=next_cnum(), i=0; azResult[i]; cn++, i+=5){
    git_ingest_commit_chng(zRoot, cn, azResult[i], atoi(azResult[i+1]),
          azResult[i+2], azResult[i+3], azResult[i+4], (nOldSize==0)?1:0);
  }
  db_query_free(azResult);

  /* We couldn't do this before since GIT tags are basically milestones
  ** that point at other CHNG entries and we may not have had all the CHNG
  ** records.  We do heads here too. What this means is that each head is
  ** basically a moving milestone. Not sure how desirable this really is.
  */
  git_update_refs("heads");
  git_update_refs("tags");
  
  /*
  ** Update the "historysize" entry. For GIT, it only matters that it's
  ** non-zero except when we need to re-read the database.
  */
  db_execute("UPDATE config SET value=%d WHERE name='historysize';",
      nOldSize + nNewRevs );
  db_config(0,0);
  
  /* We delayed populating FILE till now on initial scan */
  if( nOldSize==0 ){
    update_file_table_with_lastcn();
  }
  
  /* Commit all changes to the database
  */
  db_execute("COMMIT;");

  return 0;
}