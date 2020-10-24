void setup_enum_page(void){
  char *zText;
  const char *zEnum;
  int nRow;
  const char *zTitle;
  const char *zName;

  /* The user must be at least the administrator in order to see
  ** this screen.
  */
  login_check_credentials();
  if( !g.okAdmin ){
    login_needed();
    return;
  }

  /* What type of enumeration are we entering.
  */
  zEnum = P("e");
  if( zEnum==0 ){ zEnum = "subsys"; }
  if( strcmp(zEnum,"subsys")==0 ){
    zTitle = "Configure Subsystem Names";
    zName = "subsystem";
    nRow = 20;
    common_add_help_item("CvstracAdminSubsystem");
  }else
  if( strcmp(zEnum,"type")==0 ){
    zTitle = "Configure Ticket Types";
    zName = "type";
    nRow = 6;
    common_add_help_item("CvstracAdminTicketType");
  }else
  if( strcmp(zEnum,"status")==0 ){
    zTitle = "Configure Ticket States";
    zName = "status";
    nRow = 10;
    common_add_help_item("CvstracAdminTicketState");
  }else
  {
    common_add_nav_item("setup", "Main Setup Menu");
    common_header("Unknown Enumeration");
    @ <p>URL error:  The "e" query parameter specifies an unknown
    @ enumeration type: "%h(zEnum)".</p>
    @
    @ <p>Press the "Back" link above to return to the setup menu.</p>
    common_footer();
    return;
  }

  /*
  ** The "s" query parameter is a long text string that specifies
  ** the names of all subsystems.  If any subsystem names have been
  ** added or removed, then make appropriate changes to the subsyst
  ** table in the database.
  */
  if( P("x") ){
    db_execute("BEGIN");
    string_to_enum(zEnum, P("x"));
    db_execute("COMMIT");
  }

  /* Genenerate the page.
  */
  common_add_nav_item("setup", "Main Setup Menu");
  common_header(zTitle);
  zText = enum_to_string(zEnum);
  @ <p>
  @ The allowed values of the "%s(zName)" attribute of tickets
  @ are listed below.
  @ You may edit this text and press apply to change the set of
  @ allowed values.
  @ </p>
  @
  @ <p>
  @ The token on the left is the value as it is stored in the database.
  @ The text that follows is a human-readable description for the meaning
  @ of the token.  A color name for use in reports may optionally appear
  @ in parentheses after the description.
  @ </p>
  @
  @ <p>
  @ <form action="%s(g.zPath)" method="POST">
  @ <input type="hidden" name="e" value="%s(zEnum)">
  @ <textarea cols=60 rows=%d(nRow) name="x">%h(zText)</textarea>
  @ <p><input type="submit" value="Submit"></p>
  @ </form>
  common_footer();
}