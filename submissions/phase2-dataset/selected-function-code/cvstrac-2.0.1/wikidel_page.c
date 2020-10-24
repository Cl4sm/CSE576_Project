void wikidel_page(void){
  const char *pg = P("p");
  const char *zTime = P("t");
  char *zTimeFmt;
  const char *zIP = 0;
  int nBefore, nAfter, nSimilar;
  int tm = 0;
  int isLocked;
  int isHome = 0;
  int isUser = 0;

  login_check_credentials();
  db_add_functions();
  isUser = is_user_page(pg);
  isHome = is_home_page(pg);
  if( !isUser && (pg==0 || is_wiki_name(pg)!=strlen(pg)) ){
    login_needed();
    return;
  }
  if( zTime==0 || (tm = atoi(zTime))==0 ){
    zTime = db_short_query(
       "SELECT max(-invtime) FROM wiki WHERE name='%q'", pg);
    if( zTime==0 || (tm = atoi(zTime))==0 ){
      cgi_redirect("index");
    }
  }
  if( !g.okSetup && !isHome ){
    const char *zUser = db_short_query(
       "SELECT who FROM wiki WHERE name='%q' AND invtime=%d", pg, -tm);
    if( !ok_to_delete_wiki(tm, zUser) ){
      login_needed();
      return;
    }
  }
  zIP = db_short_query(
       "SELECT ipaddr FROM wiki WHERE name='%q' AND invtime=%d", pg, -tm);
  nBefore = atoi( db_short_query(
     "SELECT count(*) FROM wiki WHERE name='%q' AND invtime>%d", pg, -tm));
  nAfter = atoi( db_short_query(
     "SELECT count(*) FROM wiki WHERE name='%q' AND invtime<%d", pg, -tm));
  nSimilar = atoi( db_short_query(
     "SELECT count(*) FROM wiki WHERE invtime>=%d AND invtime<=%d "
               "AND ipaddr='%q'", -3600-tm, 3600-tm, zIP));

  zTimeFmt = db_short_query("SELECT ldate(%d)", tm);
  isLocked = atoi( db_short_query(
     "SELECT locked FROM wiki WHERE name='%q' LIMIT 1", pg));
  common_add_action_item(
     zTime ? mprintf("wiki?p=%t&t=%d", pg, tm) : mprintf("wiki?p=%t", pg),
     "Cancel"     
  );
  common_add_help_item("CvstracWiki");
  common_header("Verify Delete");
  @ <p><big><b>Delete Wiki Page "%h(wiki_expand_name(pg))"?</b></big></p>
  @ <p>All delete actions are irreversible. Make your choice carefully!</p>
  @ <form action="wikidodel" method="POST">
  @ <input type="hidden" name="p" value="%h(pg)">
  if( P("t") ){
    @ <input type="hidden" name="t" value="%d(tm)">
  }
  @ <input type="hidden" name="t2" value="%d(tm)">
  @ <table border=0 cellpadding=5>
  @
  if( !isLocked && (isHome || g.okSetup || nBefore+nAfter==0) ){
    @ <tr><td align="right">
    if( nBefore==0 && nAfter==0 ){
      @ <input type="submit" name="all" value="Yes">
    }else{
      @ <input type="submit" name="all" value="All">
    }
    @ </td><td>
    @ Delete this page with all its history.
    @ </td></tr>
    @
  }
  if( nBefore>0 && nAfter>0 && (isHome || g.okSetup) ){
    @
    @ <tr><td align="right">
    @ <input type="submit" name="after" value="Older">
    @ </td><td>
    @ Delete %d(nBefore+1) historical version(s) from %s(zTimeFmt) and older
    @ but retain the %d(nAfter) most recent version(s) of the page.
    @ </td></tr>
  }
  if( nBefore+nAfter>0 ){
    @
    @ <tr><td align="right">
    @ <input type="submit" name="one" value="One">
    @ </td><td>
    @ Delete a single page from %s(zTimeFmt) 
    @ but retain the %d(nBefore+nAfter) other version(s) of the page.
    @ </td></tr>
  }
  if( zIP && zIP[0] && nSimilar>1 ){
    @
    @ <tr><td align="right">
    @ <input type="submit" name="similar" value="Similar">
    @ <input type="hidden" name="ip" value="%s(zIP)">
    @ </td><td>
    @ Delete %d(nSimilar) changes to this and other wiki pages
    @ from IP address (%s(zIP)) that occur
    @ within one hour of %s(zTimeFmt).
    @ </td></tr>
  }
  @
  @ <tr><td align="right">
  @ <input type="submit" name="cancel" value="Cancel">
  @ </td><td>
  @ Do not delete anything.
  @ </td></tr>
  @ </table>  
  @ </form>
  common_footer();
}