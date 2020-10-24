void setup_page(void){
  /* The user must be at least the administrator in order to see
  ** this screen.
  */
  login_check_credentials();
  if( !g.okAdmin ){
    login_needed();
    return;
  }

  common_standard_menu("setup", 0);
  common_add_help_item("CvstracAdmin");
  common_header("Setup Menu");
  @ <table cellpadding="10">
  if( g.okSetup ){
    menu_entry(mprintf("%s&nbsp;Repository",g.scm.zName), "setup_repository",
      "Identify the repository to which this server is linked.");
    if( g.scm.pxUserWrite ){
      menu_entry("User&nbsp;Database", "setup_user", 
        mprintf("Control how CVSTrac interacts with the %h user "
                "and password database", g.scm.zName));
    }
    menu_entry("Log&nbsp;File", "setup_log",
      "Turn the access log file on and off.");
    menu_entry("Attachments", "setup_attach",
      "Set the maximum allowable size for attachments.");
    menu_entry("Abuse Control", "setup_throttle",
      "Options to control bandwidth abuse and wiki spam.");
  }
  menu_entry("Ticket&nbsp;Types", "setup_enum?e=type",
    "Enumerate the different types of tickets that can be entered into "
    "the system.");
  menu_entry("Ticket&nbsp;States", "setup_enum?e=status",
    "Configure the allowed values for the \"status\" attribute of tickets.");
  menu_entry("New&nbsp;Tickets&nbsp;Defaults", "setup_newtkt",
    "Specify the default values assigned to various ticket attributes when "
    "a new ticket is created.");
  menu_entry("Subsystem&nbsp;Names", "setup_enum?e=subsys",
    "List the names of subsystems that can be used in the \"subsystem\" "
    "attribute of tickets.");
  menu_entry("User-Defined&nbsp;Fields", "setup_udef",
    "Create user-defined database columns in the TICKET table");
  if( g.okSetup ){
    menu_entry("Diff&nbsp;and&nbsp;Filter&nbsp;Programs", "setup_diff",
      "Specify commands or scripts used to compute the difference between "
      "two versions of a file and pretty print files.");
    menu_entry("External&nbsp;Tools", "setup_tools",
      "Manage tools for processing CVSTrac objects." );
    menu_entry("Change&nbsp;Notification", "setup_chng",
      "Define an external program to run whenever a ticket is created "
      "or modified.");
    menu_entry("Customize&nbsp;Style", "setup_style",
      "Headers, footers, stylesheets, other web page elements.");
    menu_entry("User Interface", "setup_interface",
      "Control the user interface functionality." );
    menu_entry("Wiki Markup", "setup_markup",
      "Manage custom Wiki markups" );
    menu_entry("Backup&nbsp;&amp;&nbsp;Restore", "setup_backup",
      "Make a backup copy of the database or restore the database from a "
      "backup copy.");
    menu_entry("Timeline&nbsp;&amp;&nbsp;RSS", "setup_timeline", 
      "Set timeline cookie lifetime and RSS \"Time To Live\".");
  }
  @ </table>
  common_footer();
}