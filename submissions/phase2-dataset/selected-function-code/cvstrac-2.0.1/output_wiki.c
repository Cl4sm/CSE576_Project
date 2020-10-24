void output_wiki(
  const char *zText,          /* The text to be formatted */
  const char *zLinkSuffix,    /* Suffix added to hyperlinks to Wiki */
  const char *zPageId         /* Name of current page */
){
  int i, j, k;
  int aList[20];         /* See adjust_list_nesting for details */
  int inPRE = 0;
  int inB = 0;
  int inI = 0;
  int inT = 0;
  int inTab = 0;
  int v;
  int wordStart = 1;     /* At the start of a word */
  int lineStart = 1;     /* At the start of a line */
  int paraStart = 1;     /* At the start of a paragraph */
  const char *zEndB;     /* Text used to end a run of bold */
  char **azAttach;       /* Attachments to zPageId */
  const char *zExtLink = "<font color=\"#a0a0a0\">&curren;</font>";
  static int once = 1;
  static int nTicket, nCommit;
  if( once ){
    nTicket = atoi(db_short_query("SELECT max(tn) FROM ticket"));
    nCommit = atoi(db_short_query("SELECT max(cn) FROM chng"));
    once = 0;
  }

  i = 0;
  aList[0] = 0;
  azAttach = 0;
  zEndB = "";
  while( zText[i] ){
    char *z;
    int n;
    Markup sMarkup;
    int c = zText[i];

    /* Text between <html>...</html> is interpreted as HTML.
    */
    if( c=='<' && (n = is_html(&zText[i]))>0 ){
      put_htmlized_text(&zText, i);
      zText += 6;
      cgi_printf("<div>");
      output_restricted_html(zText, n-13);
      cgi_printf("</div>");
      zText += n - 6;
      i = 0;
      continue;
    }

    /* Markup may consist of special strings contained in curly braces.
    ** Examples:  "{linebreak}"  or "{quote: *:}"
    */
    if( c=='{' && is_markup(&zText[i], &sMarkup) ){
      /*
      ** Markup of the form "{linebreak}" forces a line break.
      */
      if( sMarkup.lenType==9 && strncmp(sMarkup.zType,"linebreak",9)==0 ){
        put_htmlized_text(&zText, i);
        zText += sMarkup.lenTotal;
        i = 0;
        cgi_printf("<br>\n");
        wordStart = lineStart = paraStart = 0;
        continue;
      }

      /*
      ** Markup of the form "{clear}" moves down past any left or right
      ** aligned images.
      */
      if( sMarkup.lenType==5 && strncmp(sMarkup.zType,"clear",5)==0 ){
        put_htmlized_text(&zText, i);
        zText += sMarkup.lenTotal;
        i = 0;
        cgi_printf("<br clear=\"both\">\n");
        wordStart = lineStart = paraStart = 0;
        continue;
      }

      /*
      ** Markup of the form "{quote: ABC}" writes out the text ABC exactly
      ** as it appears.  This can be used to escape special meanings 
      ** associated with ABC.
      */
      if( sMarkup.lenType==5 && strncmp(sMarkup.zType,"quote",5)==0 ){
        put_htmlized_text(&zText, i);
        if( sMarkup.zKey==sMarkup.zArgs ){
          n = sMarkup.lenKey;
        }else{
          n = &sMarkup.zArgs[sMarkup.lenArgs] - sMarkup.zKey;
        }
        put_htmlized_text(&sMarkup.zKey, n);
        zText += sMarkup.lenTotal;
        i = 0;
        wordStart = lineStart = paraStart = 0;
        continue;
      }

      /*
      ** Markup of the form "{wiki: NAME TEXT}" creates a hyperlink
      ** to wiki page. The hyperlink appears on the screen as TEXT.
      */
      if( sMarkup.lenType==4 && strncmp(sMarkup.zType,"wiki",4)==0 ){
        int exists = 1;
        put_htmlized_text(&zText, i);
        if( g.okRdWiki ) {
          char *zPage = mprintf("%.*s", sMarkup.lenKey, sMarkup.zKey);
          exists = db_exists("SELECT 1 FROM wiki WHERE name='%q'", zPage);
          if( !exists ) cgi_printf("<em>");
          cgi_printf("<a href=\"wiki?p=%t%s\">", zPage, zLinkSuffix);
          free(zPage);
        }
        put_htmlized_text(&sMarkup.zArgs, sMarkup.lenArgs);
        if( g.okRdWiki ) {
          cgi_printf("</a>");
          if( !exists ) cgi_printf("</em>");
        }
        zText += sMarkup.lenTotal;
        i = 0;
        wordStart = lineStart = paraStart = 0;
        continue;
      }

      /*
      ** Markup of the form "{link: TO TEXT}" creates a hyperlink to TO.
      ** The hyperlink appears on the screen as TEXT.  TO can be a any URL,
      ** including a relative URL such as "chngview?cn=123".
      */
      if( sMarkup.lenType==4 && strncmp(sMarkup.zType,"link",4)==0 ){
        put_htmlized_text(&zText, i);
        if( is_url(sMarkup.zKey)>0 ){
          cgi_printf("%s<a href=\"%.*s\"%s>",
                     zExtLink, sMarkup.lenKey, sMarkup.zKey,
                     g.noFollow ? " rel=\"nofollow\"" : "");
        }else{
          char *zLink = format_link("%.*s", sMarkup.lenKey, sMarkup.zKey);
          cgi_printf("<a href=\"%z\">", zLink);
        }
        put_htmlized_text(&sMarkup.zArgs, sMarkup.lenArgs);
        cgi_printf("</a>");
        zText += sMarkup.lenTotal;
        i = 0;
        wordStart = lineStart = paraStart = 0;
        continue;
      }

      /*
      ** Markup of the form "{image: URL ALT}" creates an in-line image to
      ** URL with ALT as the alternate text.  URL can be relative (for example
      ** the URL of an attachment.
      **
      ** If the URL is the name of an attachment, then automatically
      ** convert it to the correct URL for that attachment.
      */
      if( (sMarkup.lenType==5 && strncmp(sMarkup.zType,"image",5)==0)
       || (sMarkup.lenType==9 && strncmp(sMarkup.zType,"leftimage",9)==0)
       || (sMarkup.lenType==10 && strncmp(sMarkup.zType,"rightimage",10)==0)
      ){
        char *zUrl = 0;
        const char *zAlign;
        char *zAlt = htmlize(sMarkup.zArgs, sMarkup.lenArgs);
        if( azAttach==0 && zPageId!=0 ){
          azAttach = (char **)
                     db_query("SELECT fname, atn FROM attachment "
                              "WHERE tn='%q'", zPageId);
        }
        if( azAttach ){
          int ix;
          for(ix=0; azAttach[ix]; ix+=2){
            if( strncmp(azAttach[ix],sMarkup.zKey,sMarkup.lenKey)==0 ){
              free(zUrl);
              zUrl = format_link("attach_get/%s/%h",
                                 azAttach[ix+1], azAttach[ix]);
              break;
            }
          }
        }
        if( zUrl==0 ){
          zUrl = htmlize(sMarkup.zKey, sMarkup.lenKey);
        }
        put_htmlized_text(&zText, i);
        switch( sMarkup.zType[0] ){
          case 'l': case 'L':   zAlign = " align=\"left\"";  break;
          case 'r': case 'R':   zAlign = " align=\"right\""; break;
          default:              zAlign = "";                 break;
        }
        cgi_printf("<img src=\"%s\" alt=\"%s\"%s>", zUrl, zAlt, zAlign);
        free(zUrl);
        free(zAlt);
        zText += sMarkup.lenTotal;
        i = 0;
        wordStart = lineStart = paraStart = 0;
        continue;
      }

      /*
      ** Markup of the form "{report: RN}" embeds a report into the output.
      */
      if( (sMarkup.lenType==6 && strncmp(sMarkup.zType,"report",6)==0)
          || (sMarkup.lenType==11 && strncmp(sMarkup.zType,"rightreport",11)==0)
          || (sMarkup.lenType==10 && strncmp(sMarkup.zType,"leftreport",10)==0)
      ){
        char *zCaption = mprintf("%.*s", sMarkup.lenArgs, sMarkup.zArgs);
        char *zAlign = 0;
        if( sMarkup.lenType==11 ){
          zAlign = "align=\"right\"";
        }else if( sMarkup.lenType==10 ){
          zAlign = "align=\"left\"";
        }
        put_htmlized_text(&zText, i);
        embed_view( atoi(sMarkup.zKey),
                    (sMarkup.zArgs==sMarkup.zKey) ? "" : zCaption,
                    zAlign );
        free(zCaption);
        zText += sMarkup.lenTotal;
        i = 0;
        wordStart = lineStart = paraStart = 0;
        continue;
      }

      /* Markup of the form "{markups}" outputs the list of custom markups
      ** formats with descriptions.
      */
      if( sMarkup.lenType==7 && strncmp(sMarkup.zType,"markups",7)==0 ){
        char **azMarkup;
        put_htmlized_text(&zText, i);

        azMarkup = db_query(
              "SELECT markup, description FROM markup ORDER BY markup;");
        if( azMarkup && azMarkup[0] ){
          @ <p><big><b>Custom Markup Rules</b></big></p>
          @ <p>The following are custom markup rules implemented
          @ on this server.</p>
          for(j=0; azMarkup[j]; j+=2){
            if( azMarkup[j+1] && azMarkup[j+1][0] ){
              /* this markup has a description, output it.
              */
              @ <p>
              output_formatted(azMarkup[j+1],NULL);
              @ </p>
            }else{
              @ <p>{%h(azMarkup[j])} (no description)</p>
            }
          }
        }

        zText += sMarkup.lenTotal;
        i = 0;
        wordStart = lineStart = paraStart = 0;
        continue;
      }

      /* It could be custom markup. There are two kinds of custom markups
      ** available. The first is a simple format string such
      ** "key=%k args=%a" where %k is replaced by the markup key and %a
      ** by any following arguments. More flexibility would probably be
      ** nice, but that's how the existing markup logic works. The second
      ** form of markup is an external executable which gets passed the
      ** key and args on the command line and any output is dumped right
      ** into the output stream.
      */
      if( sMarkup.zType && sMarkup.lenType ) {
        /* sMarkup.zType is a pointer into the text buffer, not a NUL
        ** terminated token. This is actually the case with everything
        ** in sMarkup. Note that the markup type is already checked to
        ** be only chars that pass isalpha() so we can avoid "%.*q".
        */
        char **azMarkup = db_query(
          "SELECT type,formatter FROM markup WHERE markup='%.*s';",
          sMarkup.lenType, sMarkup.zType);

        if( azMarkup && azMarkup[0] && azMarkup[1] ){
          /* We've found a custom formatter for this type */

          int bl = sMarkup.lenTotal;
          int cl = 0;
          int type = atoi(azMarkup[0]);
          char *zOutput;

          put_htmlized_text(&zText, i);

          /* handle blocks. This basically means we scan ahead to find
          ** "end<markup>. bl becomes the total length of the block
          ** and cl is everything up the the {end<markup>}. If we can't
          ** find a match, bl becomes zero and we end up just outputting
          ** the raw markup tag.
          */
          if( type==2 || type==3 ){
            char *zEnd = mprintf("{end%.*s}", sMarkup.lenType, sMarkup.zType);
            int el = strlen(zEnd);
            while( zText[bl] && strncmp(&zText[bl],zEnd,el)){ bl++; }
            if( zText[bl]!=0 ){
              /* found a matching end tag. Note that bl includes the
              ** length of the initial markup which is not part of the
              ** actual content. Fix that. bl doesn't include the length
              ** of the end markup tag. Fix that too.
              */
              cl = bl - sMarkup.lenTotal;
              bl += el;
            } else {
              /* that didn't work, restore to original value.
              */
              bl = sMarkup.lenTotal;
            }
            free(zEnd);
          }

          /* Substitutions are basically the same for all types of
          ** formatters, except that quotes are stripped from arguments
          ** to programs.
          */
          zOutput = markup_substitution( (type==1 || type==3),
            azMarkup[1], &sMarkup, &zText[sMarkup.lenTotal], cl );
          if( bl && zOutput ){
            if( type == 0 || type == 2 ){
              output_restricted_html(zOutput, strlen(zOutput));
            }else if( type==1 || (type==3 && cl==0) ){
              pipe_block(zOutput, "", 0);
            }else if( type==3 ){
              pipe_block(zOutput, &zText[sMarkup.lenTotal], cl);
            }

            free(zOutput);
          }

          zText += bl;
          i = 0;
          wordStart = lineStart = paraStart = 0;
          continue;
        }
      }
    }

    if( paraStart ){
      put_htmlized_text(&zText, i);

      /* Blank lines at the beginning of a paragraph are ignored.
      */
      if( isspace(c) && (j = is_blank_line(&zText[i]))>0 ){
        zText += j;
        continue;
      }

      /* If the first line of a paragraph begins with a tab or with two
      ** or more spaces, then that paragraph is printed verbatim.
      */
      if( c=='\t' || (c==' ' && (zText[i+1]==' ' || zText[i+1]=='\t')) ){
        if( !inPRE ){
          if( inB ){ cgi_printf(zEndB); inB=0; }
          if( inI ){ cgi_printf("</i>"); inI=0; }
          if( inT ){ cgi_printf("</tt>"); inT=0; }
          if( inTab ){ cgi_printf("</table>"); inTab=0; }
          adjust_list_nesting(aList, 0);
          cgi_printf("<pre>\n");
          inPRE = 1;
        }
      }
    } /* end if( paraStart ) */

    if( lineStart ){
      /* Blank lines in the middle of text cause a paragraph break
      */
      if( isspace(c) && (j = is_blank_line(&zText[i]))>0 ){
        put_htmlized_text(&zText, i);
        zText += j;
        if( inB ){ cgi_printf(zEndB); inB=0; }
        if( inI ){ cgi_printf("</i>"); inI=0; }
        if( inT ){ cgi_printf("</tt>"); inT=0; }
        if( inTab ){ cgi_printf("</table>"); inTab=0; }
        if( inPRE ){ cgi_printf("</pre>\n"); inPRE = 0; }
        is_list_elem(zText, &k);
        if( abs(k)<aList[0] ) adjust_list_nesting(aList, k);
        if( zText[0]!=0 ){ cgi_printf("\n<p>"); }
        wordStart = lineStart = paraStart = 1;
        i = 0;
        continue;
      }
    } /* end if( lineStart ) */

    if( lineStart && !inPRE ){
      /* If we are not in verbatim text and a line begins with "*:", then
      ** generate a bullet.  Or if the line begins with "NNN:" where NNN
      ** is a number, generate an enumeration item.
      */
      if( (j = is_list_elem(&zText[i], &k))>0 ){
        put_htmlized_text(&zText, i);
        adjust_list_nesting(aList, k);
        if( inTab ){ cgi_printf("</table>"); inTab=0; }
        if( zText[0]!='_' ) cgi_printf("<li>");
        zText += j;
        i = 0;
        wordStart = 1;
        lineStart = paraStart = 0;
        continue;
      }

      /* Four or more "-" characters on at the beginning of a line that
      ** contains no other text results in a horizontal rule.
      */
      if( (c=='-' || c=='=') && (j = is_horizontal_rule(&zText[i]))>0 ){
        put_htmlized_text(&zText, i);
        adjust_list_nesting(aList, 0);
        if( inTab ){ cgi_printf("</table>"); inTab=0; }
        cgi_printf("<hr>\n");
        zText += j;
        if( *zText ) zText++;
        i = 0;
        lineStart = wordStart = 1;
        paraStart = 1;
        continue;
      }

      /* '|' at the start of a line may be a table
      */
      if( c=='|' && (j = is_table_row(&zText[i]))>0 ){
        put_htmlized_text(&zText, i);
        adjust_list_nesting(aList, 0);
        if( !inTab ){
          cgi_printf("<table border=\"1\" cellspacing=\"0\">\n");
          inTab = 1;
        }
        output_table_row(zText,j);
        zText += j;
        i = 0;
        wordStart = 1;
        lineStart = paraStart = 0;
        continue;
      }
    } /* end if( lineStart && !inPre ) */

    if( wordStart && !inPRE ){
      /* A wiki name at the beginning of a word which is not in verbatim
      ** text generates a hyperlink to that wiki page.
      ** 
      ** Special case: If the name is in CamelCase but ends with a "_", then
      ** suppress the "_" and do not generate the hyperlink.  This allows
      ** CamelCase words that are not wiki page names to appear in text.
      */
      if( g.okRdWiki && isupper(c) && (j = is_wiki_name(&zText[i]))>0 ){
        int exists = db_exists("SELECT 1 FROM wiki WHERE name='%.*s'",
                               j, &zText[i]);
        put_htmlized_text(&zText, i);
        if( !exists ) cgi_printf("<em>");
        cgi_printf("<a href=\"%z\">%.*s</a>",
            format_link("wiki?p=%.*s%s", j, zText, zLinkSuffix),
            j, zText);
        if( !exists ) cgi_printf("</em>");
        zText += j;
        i = 0;
        wordStart = lineStart = paraStart = 0;
        continue;
      }

      if( g.okCheckout && c=='/' && (j = is_repository_file(&zText[i]))>0 ){
        char *zFile;
        put_htmlized_text(&zText, i);
        zFile = mprintf("%.*s", j-1, zText+1);
        cgi_printf("<a href=\"%z\">/%h</a>",
            format_link("rlog?f=%T", zFile), zFile);
        free(zFile);
        zText += j;
        i = 0;
        wordStart = lineStart = paraStart = 0;
        continue;
      }

      /* A "_" at the beginning of a word puts us into an italic font.
      */
      if( c=='_' && !inB && !inI && !inT && font_terminator(&zText[i+1],c,1) ){
        put_htmlized_text(&zText, i);
        i = 0;
        zText++;
        cgi_printf("<i>");
        inI = 1;
        continue;
      }

      /* A "=" at the beginning of a word puts us into an fixed font.
      */
      if( c=='=' && !inB && !inI && !inT && font_terminator(&zText[i+1],c,1) ){
        put_htmlized_text(&zText, i);
        i = 0;
        zText++;
        cgi_printf("<tt>");
        inT = 1;
        continue;
      }

      /* A "*" at the beginning of a word puts us into a bold font.
      */
      if( c=='*' && !inB && !inI && !inT && (j = count_stars(&zText[i]))>=1
              && j<=3 && font_terminator(&zText[i+j],c,j) ){
        const char *zBeginB = "";
        put_htmlized_text(&zText, i);
        i = 0;
        zText += j;
        switch( j ){
          case 1: zBeginB = "<b>";           zEndB = "</b>";             break;
          case 2: zBeginB = "<big><b>";      zEndB = "</b></big>";       break;
          case 3: zBeginB = "<big><big><b>"; zEndB = "</b></big></big>"; break;
        }
        cgi_printf(zBeginB);
        inB = j;
        continue;
      }


      /* Words that begin with "http:" or "https:" or "ftp:" or "mailto:"
      ** become hyperlinks.
      */
      if( (c=='h' || c=='f' || c=='m') && (j=is_url(&zText[i]))>0 ){
        put_htmlized_text(&zText, i);
        z = htmlize(zText, j);
        if( is_image(z, strlen(z)) ){
          cgi_printf("<img src=\"%s\" alt=\"%s\"%s>", z, z,
                     g.noFollow ? " rel=\"nofollow\"" : "");
        }else{
          cgi_printf("%s<a href=\"%s\"%s>%s</a>",
                     zExtLink, z,
                     g.noFollow ? " rel=\"nofollow\"" : "", z);
        }
        free(z);
        zText += j;
        i = 0;
        wordStart = lineStart = paraStart = 0;
        continue;
      }

      /* If the user has read permission on tickets and a word is of the
      ** form "#NNN" where NNN is a sequence of digits, then generate a
      ** hyperlink to ticket number NNN.
      */
      if( c=='#' && g.okRead && (j = ndigit(&zText[i+1]))>0 
                 && is_eow(&zText[i+1+j],0)
                 && (v = atoi(&zText[i+1]))>0 && v<=nTicket ){
        put_htmlized_text(&zText, i);
        output_ticket(v,0);
        zText += j;
        if( *zText ) zText++;
        i = 0;
        wordStart = lineStart = paraStart = 0;
        continue;
      }

      /* If the user has checkout permissions and a word is of the form
      ** "[NNN]" where NNN is a checkin number, then generate a hyperlink
      ** to check-in NNN.
      */
      if( c=='[' && g.okRead && (j = ndigit(&zText[i+1]))>0
                 && is_eow(&zText[i+j+2],0)
                 && (v = atoi(&zText[i+1]))>0 && v<=nCommit 
                 && zText[i+j+1]==']' ){
        put_htmlized_text(&zText, i);
        output_chng(v);
        zText += j+1;
        if( *zText ) zText++;
        i  = 0;
        wordStart = lineStart = paraStart = 0;
        continue;
      }
    } /* end if( wordStart && !inPre ) */

    /* A "*", "=", or a "_" at the end of a word takes us out of bold,
    ** fixed or italic mode.
    */
    if( inB && c=='*' && !isspace(zText[i-1]) && zText[i-1]!='*' &&
            (j = count_stars(&zText[i]))==inB && is_eow(&zText[i+j],0) ){
      inB = 0;
      put_htmlized_text(&zText, i);
      i = 0;
      zText += j;
      cgi_printf(zEndB);
      continue;
    }
    if( inT && c=='=' && !isspace(zText[i-1]) && is_eow(&zText[i+1],0) ){
      put_htmlized_text(&zText, i);
      i = 0;
      zText++;
      inT = 0;
      cgi_printf("</tt>");
      continue;
    }
    if( inI && c=='_' && !isspace(zText[i-1]) && is_eow(&zText[i+1],0) ){
      put_htmlized_text(&zText, i);
      i = 0;
      zText++;
      inI = 0;
      cgi_printf("</i>");
      continue;
    }
    if( wordStart ){
      wordStart = isspace(c) || c=='(' || c=='"';
    }else{
      wordStart = isspace(c);
    }
    lineStart = c=='\n';
    paraStart = 0;
    i++;
  }
  if( zText[0] ) cgi_printf("%h", zText);
  if( inB ) cgi_printf("%s\n",zEndB);
  if( inT ) cgi_printf("</tt>\n");
  if( inI ) cgi_printf("</i>\n");
  if( inTab ){ cgi_printf("</table>"); inTab=0; }
  adjust_list_nesting(aList, 0);
  if( inPRE ) cgi_printf("</pre>\n");
}