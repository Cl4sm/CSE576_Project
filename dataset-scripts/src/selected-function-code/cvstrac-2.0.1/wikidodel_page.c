void wikidodel_page(void){
  const char *pg = P("p");
  const char *t = P("t");
  const char *t2 = P("t2");
  char *zLast;
  const char *zRestrict;
  int isHome = 0;
  int isUser = 0;

  login_check_credentials();
  isHome = is_home_page(pg);
  isUser = is_user_page(pg);
  if( t2==0 || (!isUser && (pg==0 || is_wiki_name(pg)!=strlen(pg))) ){
    login_needed();
    return;
  }
  if( P("cancel") ){
    if( t==0 ){
      cgi_redirect(mprintf("wiki?p=%t",pg));
    }else{
      cgi_redirect(mprintf("wiki?p=%t&t=%t",pg,t));
    }
    return;
  }
  db_add_functions();
  if( isHome || g.okSetup ){
    /* The Setup user can delete anything. A user can always delete their
    ** home page.
    */
    zRestrict = "";
  }else if( g.okDelete ){
    /* Make sure users with Delete privilege but without Setup privilege
    ** can only delete wiki added by anonymous within the past 24 hours.
    */
    zRestrict = " AND (who='anonymous' OR who=user()) AND invtime<86400-now()";
  }else if( g.isAnon ){
    /* Anonymous user without Delete privilege cannot delete anything */
    login_needed();
    return;
  }else{
    /* What is left is registered users without Delete privilege.  They
    ** can delete the things that they themselves have added within 24
    ** hours. */
    zRestrict = " AND who=user() AND invtime<=86400-now()";
  }
  if( P("all") ){
    db_execute(
      "BEGIN;"
      "DELETE FROM wiki WHERE name='%q'%s;"
      "DELETE FROM attachment WHERE tn='%q' AND "
          "(SELECT count(*) FROM wiki WHERE name='%q')==0;"
      "COMMIT",
      pg, zRestrict, pg, pg
    );
    cgi_redirect("wiki?p=WikiIndex");
    return;
  }
  if( P("similar") ){
    db_execute("DELETE FROM wiki WHERE invtime>=%d AND invtime<=%d "
               "AND ipaddr='%q'%s",
              -3600-atoi(t2), 3600-atoi(t2), P("ip"), zRestrict);
  }else if( P("one") ){
    db_execute("DELETE FROM wiki WHERE name='%q' AND invtime=%d%s",
                pg, -atoi(t2), zRestrict);
  }else if( P("after") && (isHome || g.okSetup) ){
    db_execute("DELETE FROM wiki WHERE name='%q' AND invtime>=%d",pg,-atoi(t2));
  }
  zLast = db_short_query("SELECT max(-invtime) FROM wiki WHERE name='%q'",pg);
  if( zLast ){
    cgi_redirect(mprintf("wiki?p=%t&t=%t",pg,zLast));
  }else{
    cgi_redirect(mprintf("wiki?p=%t",pg));
  }
}