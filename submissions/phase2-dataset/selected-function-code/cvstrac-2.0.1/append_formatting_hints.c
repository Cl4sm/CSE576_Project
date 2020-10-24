void append_formatting_hints(void){
  char **az;
  int j;
  @ <ul>
  @ <li><p>
  @ Blank lines divide paragraphs.
  @ </p></li>
  @
  @ <li><p>
  @ If a paragraph is indented by a tab or by two or more spaces,
  @ it is displayed verbatim -- in a constant-width font with all
  @ spacing and line breaks preserved.
  @ </p></li>
  @
  @ <li><p>
  @ Surround phrases by underscores, asterisks or equals 
  @ for italic, bold or fixed text.
  @ (Ex: "<tt>_italic text_, *bold text*, =fixed text=</tt>")
  @ Use two or three asterisks for bold text in a larger font.
  @ </p></li>
  @
  @ <li><p>
  if( g.okRead ){
    @ Text like "<tt>#123</tt>" becomes a hyperlink to ticket #123.
  }
  if( g.okCheckout ){
    @ Text like "<tt>[456]</tt>" becomes a hyperlink to
    @ check-in [456].
  }
  if( g.okRdWiki ){
    @ An absolute URL, a wiki page name becomes a hyperlink.
    @ Also markup of the form "<tt>{wiki: <i>title text</i>}</tt>"
    @ becomes a hyperlink to the wiki document of <i>title</i>.
  } else {
    @ An absolute URL becomes a hyperlink.
  }
  @ Or use markup of the form:
  @ "<tt>{link: <i>url text</i>}</tt>".
  @ </p></li>
  @
  @ <li><p>
  @ A path to a file in the repository becomes a link to its rlog page:
  @ "<tt>/path/to/format.c</tt>".
  @ </p></li>
  @
  @ <li><p>
  @ The characters "<tt>*:</tt>" or "<tt>1:</tt>" at the beginning of a line
  @ produce a bullet or enumeration list.
  @ Use additional colons for nested lists.
  @ </p></li>
  @
  @ <li><p>
  @ Create a table by wrapping cells with "<tt>|</tt>", starting at the
  @ beginning of a line. Each cell is separated with a "<tt>|</tt>" and
  @ each line should end with a "<tt>|</tt>".
  @ </p></li>
  @
  @ <li><p>
  @ Use "<tt>_:</tt>" at the beginning of a paragraph to indent that
  @ paragraph.  Multiple colons indent more.
  @ </p></li>
  @
  @ <li><p>
  @ Four or more "-" or "=" characters on a line by themselves generate a
  @ horizontal rule (the &lt;hr&gt; markup of HTML).
  @ </p></li>
  @
  @ <li><p>
  @ Create a line-break using "<tt>{linebreak}</tt>".
  @ </p></li>
  @
  @ <li><p>
  @ Use "<tt>{quote: <i>text</i>}</tt>" to display <i>text</i>.
  @ </p></li>
  @
  @ <li><p>
  @ Insert in-line images using "<tt>{image: <i>url</i>}</tt>".
  @ The <i>url</i> can be the filename of an attachment.
  @ </p></li>
  @
  @ <li><p>
  @ Insert in-line reports using "<tt>{report: <i>rn</i>}</tt>". The <i>rn</i>
  @ is the report number (which isn't necessarily the same as the numbers on
  @ the <a href="reportlist">report list</a>).
  @ </p></li>
  @
  @ <li><p>
  @ Text between "<tt>&lt;html&gt;...&lt;/html&gt;</tt>" is interpreted as HTML.
  @ </p></li>
  @

  /* output custom markups.
  */
  az = db_query("SELECT markup, description FROM markup;");
  if( az && az[0] ){
    for(j=0; az[j]; j+=2){
      if( az[j+1] && az[j+1][0] ){
        /* this markup has a description, output it.
        */
        @ <li><p>
        output_formatted(az[j+1],NULL);
        @ </p></li>
      }else{
        @ <li><p>{%h(az[j])} (no description)</p></li>
      }
    }
  }
  @ </ul>
}