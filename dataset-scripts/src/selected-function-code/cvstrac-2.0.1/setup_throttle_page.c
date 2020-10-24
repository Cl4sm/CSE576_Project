void setup_throttle_page(void){
  int mxHit = atoi(db_config("throttle","0"));
  int nf = atoi(db_config("nofollow_link","0"));
  int cp = atoi(db_config("enable_captcha","0"));
  int lnk = atoi(db_config("max_links_per_edit","0"));
  int mscore = atoi(db_config("keywords_max_score","0"));
  const char *zKeys = db_config("keywords","");

  /* The user must be the setup user in order to see
  ** this screen.
  */
  login_check_credentials();
  if( !g.okSetup ){
    cgi_redirect("setup");
    return;
  }
  

  if( P("sz") && atoi(P("sz"))!=mxHit ){
    mxHit = atoi(P("sz"));
    db_execute("REPLACE INTO config VALUES('throttle',%d)", mxHit);
    db_config(0, 0);
  }

  if( P("nf") && atoi(P("nf"))!=nf ){
    nf = atoi(P("nf"));
    db_execute("REPLACE INTO config VALUES('nofollow_link',%d)", nf);
    db_config(0, 0);
  }
 
  if( P("cp") && atoi(P("cp"))!=cp ){
    cp = atoi(P("cp"));
    db_execute("REPLACE INTO config VALUES('enable_captcha',%d)", cp);
    db_config(0, 0);
  }
 
  if( P("lnk") && atoi(P("lnk"))!=lnk ){
    lnk = atoi(P("lnk"));
    db_execute("REPLACE INTO config VALUES('max_links_per_edit',%d)", lnk);
    db_config(0, 0);
  }

  if( P("mscore") && atoi(P("mscore"))!=mscore ){
    mscore = atoi(P("mscore"));
    db_execute("REPLACE INTO config VALUES('keywords_max_score',%d)", mscore);
    db_config(0, 0);
  }

  if( P("keys") && strcmp(zKeys,PD("keys","")) ){
    zKeys = P("keys");
    db_execute("REPLACE INTO config VALUES('keywords','%q')", zKeys);
    db_config(0, 0);
  }

  common_add_nav_item("setup", "Main Setup Menu");
  common_add_help_item("CvstracAdminAbuse");
  common_header("Abuse Controls");
  @ <h2>Set Maximum Anonymous Hits Per Hour</h2>
  @ <p>
  @ Enter the limit on the number of anonymous accesses from the same
  @ IP address that can occur within one hour.  Enter zero to disable
  @ the limiter.
  @ </p>
  @
  @ <p>
  @ <form action="%s(g.zPath)" method="POST">
  @ Maximum hits per hour: 
  @ <input type="text" name="sz" value="%d(mxHit)" size=5>
  @ <input type="submit" value="Set">
  @ </form>
  @ </p>
  @
  @ <p>
  @ The limiter works by maintain a table in the database (the ACCESS_LOAD
  @ table) that records the time of last access and a "load" for each
  @ IP address.  The load reduces exponentially with a half-life of
  @ one hour.  Each new access increases the load by 1.0.  When the
  @ load exceeds the threshold above, the load automatically doubles and
  @ the client is bounced to the <a href="captcha">captcha</a>
  @ page. After this redirection happens a
  @ few times, the user is denied access until the load decreases
  @ below the threshold. If the user passes the
  @ <a href="captcha">captcha</a> test, a cookie is set.
  @ </p>
  @
  @ <p>
  @ When the limiter is enabled, the <a href="captcha">captcha</a>
  @ page is also used to screen users before they try to do anything
  @ that might change the database (create a <a href="tktnew">new ticket</a>,
  @ <a href="wikiedit?p=WikiIndex">change a wiki page</a>, etc). This
  @ feature is intended to block automated wiki spam.
  @
  @ <p>
  @ Any attempt to access the page named "stopper" (reachable from
  @ <a href="honeypot">honeypot</a>) automatically increases
  @ the load to twice the threshold.  There are hyperlinks to the
  @ honeypot on every page.  The idea is to trick spiders into visiting
  @ this honeypot so that they can have their access terminated quickly.
  @ </p>
  @
  @ <p>
  @ The limiter and the honeypot only work for users that are not
  @ logged in - anonymous users.  Users who are logged in can visit
  @ any page (including the honeypot) as often as they want and will
  @ never be denied access. The limiter (but not the honeypot) is also
  @ disabled for any user with a valid <a href="captcha">captcha</a>
  @ cookie.
  @ </p>
  @
  @ <p>A summary of the <a href="info_throttle">Access Log</a> is available
  @ separately.</p>

  @ <hr>
  @ <h2>Captcha</h2>
  @ <form action="%s(g.zPath)" method="POST">
  @ <p>
  @ By turning on this option, anonymous users will be required to pass a
  @ simple <a href="http://en.wikipedia.org/wiki/Captcha">captcha</a>
  @ test before being allowed to change content (tickets, wiki, etc). Passing
  @ the test will set a cookie on the browser. Too many failures to pass
  @ the test will trigger the throttler and lock the users IP address out.
  @ Note that the rate limiter has to be enabled (non-zero) for this option
  @ to be available.
  @ </p>
  @ <p>
  @ <label for="cp"><input type="checkbox" name="cp" id="cp"
  @    %s(cp?" checked":"") %s(mxHit?"":"disabled") value="1">
  @ Turn on captcha for content changes.</label>
  @ </p>
  @ <input type="submit" value="Set">
  @ </form>
  @ <hr>

  @ <h2>External Links</h2>
  @ <form action="%s(g.zPath)" method="POST">
  @ <p>
  @ By turning on this option, all links to external sites are tagged as
  @ "nofollow". This provides a hint to search engines to ignore such links
  @ and reduces the value of wiki spam. However, this may be of limited use
  @ since wiki spammers aren't always smart enough to notice that they're
  @ wasting their time.
  @ </p>
  @ <p>
  @ <label for="nf"><input type="checkbox" name="nf" id="nf"
  @    %s(nf?" checked":"") value="1">
  @ Don't allow search engines to follow external links.</label>
  @ </p>
  @ <input type="submit" value="Set">
  @ </form>
  @ 
  @ <form action="%s(g.zPath)" method="POST">
  @ <p>
  @ Wiki spam generally works by inserting large numbers of links in a
  @ single page edit. A simple way to prevent this is to simply impose a
  @ maximum number of new external links in a single wiki edit.
  @ A value of zero will disable this option.
  @ </p>
  @ <p>
  @ Maximum external links per Wiki edit:
  @ <input type="text" name="lnk" value="%d(lnk)" size=5>
  @ </p>
  @ <input type="submit" value="Set">
  @ </form>
  @ <hr>
  @ <h2>Keyword Filtering</h2>
  @ <form action="%s(g.zPath)" method="POST">
  @ <p>
  @ Enter a space-separated list of keywords. All wiki edits will be
  @ checked against this list and, if the maximum score is exceeded, 
  @ the change will be denied. The scoring algorithm uses the standard
  @ CVSTrac text <strong>search()</strong> function (where each matched
  @ keyword scores from 6 to 10 points). Repeating a keyword in the
  @ list will cause it to score higher.
  @ </p>
  @ <p>
  cgi_text("mscore", 0, 0, 0, 0, 5, 8, 1, mprintf("%d",mscore),
           "Maximum keyword score");
  @ </p>
  @ <p>
  @ <h3>Forbidden Keywords</h3>
  @ <textarea name="keys" rows="8" cols="80" wrap="virtual">
  @ %h(zKeys)
  @ </textarea>
  @ </p>
  @ <input type="submit" value="Set">
  @ </form>

  common_footer();
}