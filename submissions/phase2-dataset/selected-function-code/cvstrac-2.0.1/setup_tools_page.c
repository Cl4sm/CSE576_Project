void setup_tools_page(void){
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
  common_add_action_item("setup_toolsedit", "Add Tool");
  common_header("External Tools");

  az = db_query("SELECT name, description FROM tool ORDER BY name;");
  if( az && az[0] ){
    @ <p><big><b>External Tools</b></big></p>
    @ <dl>
    for(j=0; az[j]; j+=2){
      @ <dt><a href="setup_toolsedit?t=%h(az[j])">%h(az[j])</a></dt>
      if( az[j+1] && az[j+1][0] ){
        /* this tool has a description, output it.
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