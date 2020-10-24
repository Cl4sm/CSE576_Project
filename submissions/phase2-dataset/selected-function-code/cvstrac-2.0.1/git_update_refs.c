static void git_update_refs(const char* zType){
  int i;
  char **azRefs;

  /* assumes a logical ordering for names... There's no other way to really do
  ** this, unfortunately.
  */
  azRefs = db_query("SELECT name,object,seen,cn FROM %s ORDER by name", zType);

  for( i=0; azRefs[i]; i+=4 ){
    const char *zName = azRefs[i];
    const char *zObject = azRefs[i+1];
    time_t nSeen = atoi(azRefs[i+2]);
    int cn = atoi(azRefs[i+3]);
    int chngcn = 0;
    char **azChng;
    char *zCn;

    /* Find the CHNG record for the commit. This isn't as nice as we'd like
    ** because CHNG records, being designed for CVS, don't actually store
    ** version numbers and, unlike Subversion, a cn doesn't directly map
    ** to a revision number. So we need to grab the cn from whatever FILECHNG
    ** record corresponds. Note that we _can_ do this in a single query, but
    ** it's quite, quite slow. Probably faster in SQLite 3...
    */

    zCn = db_short_query(
        "SELECT cn FROM filechng WHERE vers='%s' LIMIT 1", zObject);

    if( zCn==0 ) continue;
    chngcn = atoi(zCn);
    free(zCn);
    if( chngcn==0 ) continue;

    azChng = db_query( "SELECT date, user FROM chng WHERE cn=%d", chngcn );

    /* a FILECHNG without a corresponding CHNG? I think not... */
    assert(azChng);

    if( cn==0 ) cn = next_cnum();

    /* Create/update a milestone. The message text basically contains
    ** some information about the type of ref, the name, the commit object,
    ** and, most importantly, a reference to the actual CHNG which, at
    ** display time, should turn into a hyperlink.  Note that in practice,
    ** the milestone will appear next to the commit in the timeline. But
    ** it serves as the only way to document that the commit itself is
    ** somehow special. At some point we should be able to add some concept
    ** of tag browsing.
    */
    db_execute(
      "REPLACE INTO chng(cn,date,branch,milestone,user,directory,message) "
      "VALUES(%d,%d,'',2,'%q','','%q (%s, commit [%d], object %q)')",
      cn, atoi(azChng[0]), azChng[1], zName, zType, chngcn, zObject
    );

    db_execute(
      "REPLACE INTO %s(name,cn,object,seen) VALUES('%q',%d,'%s',%d)",
      zType, zName, cn, zObject, nSeen
    );
    db_query_free( azChng );
  }
  db_query_free(azRefs);
}