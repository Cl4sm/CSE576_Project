void browse_rlog(void){
  char *zDir, *z;
  int showMilestones;
  const char *zFile;

  login_check_credentials();
  if( !g.okCheckout ){ login_needed(); return; }
  throttle(1,0);
  common_standard_menu(0, "search?f=1");
  showMilestones = atoi(PD("sms","0"));
  history_update(0);
  zFile = PD("f","");
  /* Make sure we always have '/' in zFile, otherwise link to parent
  ** directory won't work for file in repository root.
  */
  if( strrchr(zFile, '/') ){
    zDir = mprintf("%T?d=%T", default_browse_url(), zFile);
  }else{
    zDir = mprintf("%T?d=/%T", default_browse_url(), zFile);
  }
  z = strrchr(zDir, '/' );
  if( z ){ *z = 0;}
  common_add_action_item(zDir, "Directory");
  add_file_tools(0,zFile,0,0);
  common_add_help_item("CvstracFileHistory");
  revision_history(zFile, showMilestones);
  common_footer();
}