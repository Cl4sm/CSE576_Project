void setup_markup_page(void){
  int j;
  char **az;

  /* The user must be the setup user in order to see
  ** this screen.
  */
  login_check_credentials();
  if( !g.okSetup ){
    cgi_redirect("setup");
    return;
  }

  common_add_nav_item("setup", "Main Setup Menu");
  common_add_action_item("setup_markupedit", "Add Markup");
  common_add_help_item("CvstracAdminMarkup");
  common_header("Custom Wiki Markup");

  az = db_query("SELECT markup, description FROM markup ORDER BY markup;");
  if( az && az[0] ){
    @ <p><big><b>Custom Markup Rules</b></big></p>
    @ <dl>
    for(j=0; az[j]; j+=2){
      @ <dt><a href="setup_markupedit?m=%h(az[j])">%h(az[j])</a></dt>
      if( az[j+1] && az[j+1][0] ){
        /* this markup has a description, output it.
        */
        @ <dd>
        output_formatted(az[j+1],NULL);
        @ </dd>
      }else{
        @ <dd>(no description)</dd>
      }
    }
    @ </dl>
  }

  common_footer();
}