void search_page(void){
  int srchTkt = PD("t","0")[0]!='0';
  int srchCkin = PD("c","0")[0]!='0';
  int srchWiki = PD("w","0")[0]!='0';
  int srchFile = PD("f","0")[0]!='0';
  int nPage = atoi(PD("p","0"));
  const char *zPattern;
  char **azResult = 0;
  char *zPage;
  sqlite3 *db;
  char zSql[10000];

  login_check_credentials();
  zPattern = P("s");
  if( zPattern && zPattern[0] ){
    zPattern = sqlite3_mprintf("%q",zPattern);
  }
  if( srchTkt+srchCkin+srchWiki+srchFile==0 ){
    srchTkt = srchCkin = srchWiki = srchFile = 1;
  }
  if( !g.okRead ) srchTkt = 0;
  if( !g.okCheckout ) srchFile = srchCkin = 0;
  if( !g.okRdWiki ) srchWiki = 0;
  if( srchTkt==0 && srchCkin==0 && srchWiki==0 && srchFile==0 ){
    login_needed();
    return;
  }
  if( zPattern && zPattern[0] && strlen(zPattern)<100 ){
    int i = 0;
    char *zConnect = " (";

    db = db_open();
    db_add_functions();
    sqlite3_create_function(db, "highlight", -1, SQLITE_ANY, 0,
                            highlightFunc, 0, 0);

    appendf(zSql,&i,sizeof(zSql),"SELECT type, score, obj, title, body FROM");
    if( srchTkt ){
      appendf(zSql,&i,sizeof(zSql), "%s SELECT "
         "1 AS type, "
         "search('%s',title,description,remarks) as score, "
         "tn AS obj, "
         "highlight('%s',title) as title, "
         "highlight('%s',description,remarks) as body FROM ticket "
         "WHERE score>0",
         zConnect, zPattern, zPattern, zPattern);
      zConnect = " UNION ALL";
    }
    if( srchCkin ){
      appendf(zSql,&i,sizeof(zSql),"%s SELECT "
         "2 AS type, "
         "search('%s',message) as score, "
         "cn AS obj, "
         "NULL as title, " /* ignored */
         "highlight('%s',message) as body FROM chng "
         "WHERE score>0",
         zConnect, zPattern, zPattern);
      zConnect = " UNION ALL";
    }
    if( srchWiki ){
      appendf(zSql,&i,sizeof(zSql),"%s SELECT "
         "3 AS type, "
         "search('%s',name, text) as score, "
         "name AS obj, "
         "highlight('%s',name) as title, "
         "highlight('%s',text) as body "
         "FROM (select name as nm, min(invtime) as tm FROM wiki GROUP BY name) "
         "    as x, wiki "
         "WHERE score>0 AND x.nm=wiki.name AND x.tm=wiki.invtime",
         zConnect, zPattern, zPattern, zPattern);
      zConnect = " UNION ALL";
    }
    if( srchFile ){
      appendf(zSql,&i,sizeof(zSql),"%s SELECT "
         "CASE WHEN isdir THEN 6 ELSE 4 END as type, "
         "search('%s', dir || '/' || base) as score, "
         "dir || '/' || base AS obj, "
         "dir || '/' || base as title, "
         "highlight('%s',dir || '/' || base) as body FROM file "
         "WHERE score>0",
         zConnect, zPattern, zPattern);
      zConnect = " UNION ALL";

      appendf(zSql,&i,sizeof(zSql),"%s SELECT "
         "5 AS type, "
         "search('%s', fname, description) as score, "
         "atn AS obj, "
         "fname as title, "
         "highlight('%s',fname, description) as body FROM attachment "
         "WHERE score>0 AND tn!=0",
         zConnect, zPattern, zPattern);
      zConnect = " UNION ALL";
    }
    appendf(zSql,&i,sizeof(zSql), ") ORDER BY score DESC LIMIT 30 OFFSET %d;",
            nPage*30);
    azResult = db_query("%s",zSql);
  }
  common_standard_menu("search", 0);
  common_add_help_item("CvstracSearch");
  common_header("Search");
  @ <form action="search" method="GET">
  @ Search:
  @ <input type="text" size=40 name="s" value="%h(PD("s",""))">
  @ <input type="submit" value="Go">
  if( g.okRead + g.okCheckout + g.okRdWiki>1 ){
    char *z;
    @ <br>Look in:
    if( g.okRead ){
      z = srchTkt ? " checked " : "";
      @ <label for="t"><input type="checkbox" name="t" id="t" value="1"%s(z)>
      @    Tickets</label>
    }
    if( g.okCheckout ){
      z = srchCkin ? " checked " : "";
      @ <label for="c"><input type="checkbox" name="c" id="c" value="1"%s(z)>
      @    Checkins</label>
    }
    if( g.okRdWiki ){
      z = srchWiki ? " checked " : "";
      @ <label for="w"><input type="checkbox" name="w" id="w" value="1"%s(z)>
      @    Wiki Pages</label>
    }
    if( g.okCheckout ){
      z = srchFile ? " checked " : "";
      @ <label for="f"><input type="checkbox" name="f" id="f" value="1"%s(z)>
      @    Filenames</label>
    }
  }
  @ </form>
  @ <div>
  if( azResult && azResult[0] ){
    int i, n;
    @ <hr>
    for(i=n=0; azResult[i]; i+=5, n++){
      @ <p>
      switch( atoi(azResult[i]) ){
        case 1: /* ticket */
          @ Ticket
          output_ticket(atoi(azResult[i+2]),0);
          @ : %s(azResult[i+3])
          break;
        case 2: /* check-in */
          @ Check-in
          output_chng(atoi(azResult[i+2]));
          break;
        case 3: /* wiki */
          @ Wiki Page <a href="wiki?p=%s(azResult[i+2])">%s(azResult[i+3])</a>
          break;
        case 4: /* file */
          @ File <a href="rlog?f=%t(azResult[i+2])">%h(azResult[i+3])</a>
          break;
        case 5: /* attachment */
          zPage = db_short_query("SELECT tn FROM attachment WHERE atn=%d",
                                 atoi(azResult[i+2]));
          @ Attachment
          @   <a href="attach_get/%s(azResult[i+2])/%t(azResult[i+3])">\
          @   %s(azResult[i+3])</a> to
          if( is_integer(zPage) ){
            output_ticket(atoi(zPage),0);
          }else{
            @ wiki page <a href="wiki?p=%s(zPage)">%s(zPage)</a>
          }
          break;
        case 6:{ /* directory */
          const char* zDir = azResult[i+2];
          if( zDir[0]=='/' ) zDir ++;
          @ Directory <a href="dir?d=%t(zDir)">%h(zDir)</a>
          break;
        }
      }
      @ <br>%s(azResult[i+4])</p>
    }
    if( nPage || n>=30 ){
      @ <hr><p align="center">
      /* provide a history */
      cgi_printf("Page: ");
      for( i=0; i<nPage; i++ ){
        cgi_printf("<a href=\"search?s=%T&t=%d&c=%d&w=%d&f=%d&p=%d\">%d</a> ",
            zPattern, srchTkt, srchCkin, srchWiki, srchFile, i, i+1);
      }
      cgi_printf("%d&nbsp;&nbsp;", nPage+1);
      if( n>=30 ){
        cgi_printf("<a href=\"search?s=%T&t=%d&c=%d&w=%d&f=%d&p=%d\">More</a>",
            zPattern, srchTkt, srchCkin, srchWiki, srchFile, nPage+1);
      }
      @ </p>
    }
  }else if( zPattern && zPattern[0] ){
    @ No results for <strong>%h(zPattern)</strong>
  }
  @ </div>
  common_footer();
}