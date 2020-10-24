void wikiedit_page(void){
  const char *pg = P("p");
  const char *text = P("x");
  char **az;
  int isLocked;
  char *zErrMsg = 0;
  int isHome = 0;

  login_check_credentials();
  throttle(1,1);
  isHome = is_home_page(pg);
  if( !isHome && (pg==0 || is_wiki_name(pg)!=strlen(pg)) ){
    pg = "WikiIndex";
  }
  az = db_query(
    "SELECT invtime, locked, who, ipaddr, text "
    "FROM wiki WHERE name='%q' LIMIT 1", pg
  );
  isLocked = az[0] ? atoi(az[1]) : 0;
  if( !g.okAdmin && !isHome && (!g.okWiki || isLocked) ){
    cgi_redirect(mprintf("wiki?p=%t", pg));
  }
  if( g.okAdmin && az[0] && P("lock")!=0 ){
    isLocked = !isLocked;
    db_execute("UPDATE wiki SET locked=%d WHERE name='%q'", isLocked, pg);
    if( text && strcmp(remove_blank_lines(text),remove_blank_lines(az[4]))==0 ){
      cgi_redirect(mprintf("wiki?p=%t",pg));
      return;
    }
  }
  if( P("submit")!=0 && text!=0 ){
    time_t now = time(0);
    char *zOld = db_short_query("SELECT text FROM wiki "
                                "WHERE name='%q' AND invtime>=%d LIMIT 1",
                                pg, now);
    zErrMsg = is_edit_allowed(zOld,text);
    if( 0==zErrMsg ){
      const char *zIp = getenv("REMOTE_ADDR");
      if( zIp==0 ){ zIp = ""; }
      db_execute(
        "INSERT INTO wiki(name,invtime,locked,who,ipaddr,text) "
        "VALUES('%q',%d,%d,'%q','%q','%q')",
        pg, -(int)now, isLocked, g.zUser, zIp, remove_blank_lines(text)
      );
      cgi_redirect(mprintf("wiki?p=%t", pg));
      return;
    }
  }
  if( text==0 ) text = az[0] ? az[4] : "";
  text = remove_blank_lines(text);
  common_add_help_item("CvstracWiki");
  common_add_action_item( mprintf("wiki?p=%t",pg), "Cancel");
  common_header("Edit Wiki %h", pg);

  @ <p><big><b>Edit: "%h(wiki_expand_name(pg))"</b></big></p>
  @ <form action="wikiedit" method="POST">
  @ <input type="hidden" name="p" value="%h(pg)">

  if( zErrMsg ){
    @ <blockquote>
    @ <font color="red">%h(zErrMsg)</font>
    @ </blockquote>
  }

  @ Make changes to the document text below.  
  if( P("preview") ){
    @ See <a href="wikihints">Formatting Hints</a>.
  }else{
    @ See <a href="#formatting">Formatting Hints</a>.
  }
  @ <br><textarea cols=80 rows=30 name="x" wrap="physical">
  if( text ){
    @ %h(text)
  }
  @ </textarea><br>

  if( g.okAdmin ){
    if( isLocked ){
      @ <input type="submit" name="lock" value="Unlock Page">
      @ This page is currently locked, meaning only administrators
      @ can edit it.<br>
    }else{
      @ <input type="submit" name="lock" value="Lock Page">
      @ This page is currently unlocked.  Anyone can edit it.<br>
    }
  }
  if( P("preview") ){
    @ <input type="submit" name="submit" value="Submit Changes As Shown">
  }
  @ <input type="submit" name="preview" value="Preview Your Changes">
  @ </form>

  if( P("preview") ){
    @ <p>The following is what the page will look like:</p>
    @ <p><table border=2 cellpadding=5 width="100%%"><tr><td>
    output_wiki(text,"",pg);
    @ </td></tr></table></p><br>
  }

  attachment_html(pg,
    "<hr><h3>Attachments:</h3>\n<blockquote>",
    "</blockquote>"
  );
  if( !P("preview") ){
    @ <a name="formatting">
    @ <hr>
    @ <h3>Formatting Hints</h3>
    append_formatting_hints();
  }
  common_footer(); 
}