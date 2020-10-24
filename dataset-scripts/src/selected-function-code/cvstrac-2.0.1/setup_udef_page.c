void setup_udef_page(void){
  int idx, i;
  const char *zName;
  const char *zText;

  /* The user must be at least the administrator in order to see
  ** this screen.
  */
  login_check_credentials();
  if( !g.okAdmin ){
    login_needed();
    return;
  }

  /* Write back results if requested.
  */
  idx = atoi(PD("idx","0"));
  zName = P("n");
  zText = P("x");
  if( idx>=1 && idx<=5 && zName && zText ){
    char zEnum[20];
    char *zName2 = trim_string(zName);
    char *zDesc2 = trim_string(PD("d",""));
    bprintf(zEnum,sizeof(zEnum),"extra%d", idx);
    db_execute("BEGIN");
    if( zName2[0] ){
      string_to_enum(zEnum, zText);
      db_execute(
        "REPLACE INTO config(name,value) VALUES('%s_name','%q');",
        zEnum, zName2
      );
      db_execute(
        "REPLACE INTO config(name,value) VALUES('%s_desc','%q');",
        zEnum, zDesc2
      );
    }else{
      db_execute("DELETE FROM config WHERE name='%s_name'", zEnum);
      db_execute("DELETE FROM config WHERE name='%s_desc'", zEnum);
      db_execute("DELETE FROM enums WHERE type='%s'", zEnum);
    }
    db_execute("COMMIT");
    db_config(0,0);
  }

  /* Genenerate the page.
  */
  common_add_nav_item("setup", "Main Setup Menu");
  common_add_help_item("CvstracAdminUserField");
  common_header("Configure User-Defined Fields");
  @ <p>
  @ Five extra columns named "extra1" through "extra5" exist in the 
  @ TICKET table of the database.  These columns can be defined for
  @ application specific use using this configuration page.
  @ </p>
  @
  @ <p>
  @ Each column is controlled by a separate form below.  The column will
  @ be displayed on ticket reports if and only if its has a non-blank
  @ display name.  User's see the column as its display name, not as
  @ "extra1".
  @ </p>
  @
  @ <p>
  @ Allowed values for the column can be specified in the text box.
  @ The same format is used here
  @ as when specifying <a href="setup_enum?e=type">ticket types</a>,
  @ <a href="setup_enum?e=status">ticket states</a> and
  @ <a href="setup_enum?e=subsys">subsystem names</a>.
  @ There is one allowed value per line.  
  @ The token on the left is the value as it is stored in the database.
  @ The text that follows is a human-readable description for the meaning
  @ of the token.  A color name for use in reports may optionally appear
  @ in parentheses after the description.
  @ </p>
  @
  @ <p>
  @ The Allowed Values box may be left blank.
  @ If allowed values are defined for the column, then users will restricted
  @ to the values specified when changing the value of the column.
  @ If no allowed values are defined, then the column can be set to
  @ arbitrary text.
  @ </p>
  @
  @ <p>
  @ The Description box may be left blank. 
  @ If a description is provided, then this field may be entered on the
  @ new ticket page.  If no description is given, this field can be modified 
  @ on the edit ticket page but will not appear on the new ticket page.
  @ </p>
  for(i=0; i<5; i++){
    const char *zOld;
    char *zAllowed;
    const char *zDesc;
    char zEnumName[30];
    bprintf(zEnumName,sizeof(zEnumName),"extra%d_name",i+1);
    zOld = db_config(zEnumName,"");
    zEnumName[6] = 0;
    zAllowed = enum_to_string(zEnumName);
    bprintf(zEnumName,sizeof(zEnumName),"extra%d_desc",i+1);
    zDesc = db_config(zEnumName,"");
    @ <hr>
    @ <h3>Database column "extra%d(i+1)":</h3>
    @ <form action="%s(g.zPath)" method="POST">
    @ <input type="hidden" name="idx" value="%d(i+1)">
    @ Display Name:
    @ <input type="text" name="n" value="%h(zOld)"><br>
    @ Allowed Values: (<i>Name Desc Color</i> - omit for free text)<br>
    @ <textarea cols=60 rows=15 name="x">%h(zAllowed)</textarea><br>
    @ Description: (HTML - Leave blank to omit from new-ticket page)<br>
    @ <textarea cols=60 rows=5 name="d">%h(zDesc)</textarea><br>
    @ <input type="submit" value="Submit">
    @ </form>
  }
  common_footer();
}