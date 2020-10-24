char *is_edit_allowed(const char *zOld, const char *zNew){
  if( g.isAnon ){
    const char *zKeys = db_config("keywords","");
    int nMscore = atoi(db_config("keywords_max_score","0"));
    int nMax = atoi(db_config("max_links_per_edit","0"));
    
    /*
    ** Check for too many "bad words" in the new text. Checking the "diff"
    ** might be better?
    */
    if( nMscore && zKeys[0] ) {
      db_add_functions();
      if( db_exists("SELECT 1 WHERE search('%q','%q')>%d",zKeys,zNew,nMscore)){
        increase_load();
        return "Forbidden keywords!";
      }
    }

    /*
    ** Check to see if the threshold of external links was exceeded.
    */
    if( nMax ){
      int nOld = count_links(zOld);
      int nNew = count_links(zNew);

      /* Note that someone could bypass this by replacing a whole bunch of
      ** links in an existing page. If that starts to happen it might be
      ** necessary to compare the list of links or something.
      **
      ** Some keyword filtering would help a bit, too.
      */
      if( nNew - nOld >= nMax ){
        increase_load();
        return "Too many external links for one edit!";
      }
    }
  }
  return 0;
}