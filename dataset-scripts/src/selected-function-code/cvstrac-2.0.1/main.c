int main(int argc, char **argv){
  int i, j;
  char *zSCM;
  char *zPath;
  char *zDb;
  const char *zLogFile;
  int cmdlineProj;        /* True if project specified on command line */
  void (*xFunc)(void);

  /* Determine the SCM subsystem. Need to do this before anyone messes with
  ** argv.
  */
  i = strlen(argv[0]);
  while( i>0 && argv[0][i-1]!='/' ){ i--; }
  zSCM = mprintf("%s", &argv[0][i]);
  zPath = strstr(zSCM,"trac");
  if( zPath!=0 ) *zPath = 0;

  if( !strcmp(zSCM,"cvs") ){
    init_cvs();
  }else if(!strcmp(zSCM,"svn") ){
    init_svn();
  }else if(!strcmp(zSCM,"git") ){
    init_git();
  }else{
    fprintf(stderr,"%s: unknown SCM '%s'\n", argv[0], zSCM);
    exit(1);
  }
 
  /*
  ** Attempt to put this process in a chroot jail if requested by the
  ** user.  The program must be run as root for this to work.
  */
  if( argc>=5 && strcmp(argv[1],"chroot")==0 ){
    struct passwd *pwinfo;
    pwinfo = getpwnam(argv[3]);
    if( pwinfo==0 ){
      fprintf(stderr,"%s: no such user: %s\n", argv[0], argv[3]);
      exit(1);
    }
    if( chdir(argv[2]) || chroot(argv[2]) ){
      fprintf(stderr, "%s: Unable to change root directory to %s\n",
        argv[0], argv[2]);
      exit(1);
    }
    argv[3] = argv[0];
    argv += 3;
    argc -= 3;
    if( argc>=3 && strcmp(argv[1],"server")==0 ){
      cgi_http_server(atoi(argv[2]));
      argc--;
      argv[1] = argv[0];
      argv++;
      argv[1] = "http";
    }
    setgid(pwinfo->pw_gid);
    setuid(pwinfo->pw_uid);
  }else if( argc>=3 && strcmp(argv[1],"server")==0 ){
    cgi_http_server(atoi(argv[2]));
    argv[1] = argv[0];
    argv++;
    argv[1] = "http";
    argc--;
  }

  /*
  ** Make sure we have the right number of arguments left.
  */
  if( argc<2 || argc>4 ){
    usage(argv[0]);
  }

  /*
  ** For security, do not allow this program to be run as root.
  */
  if( getuid()==0 || getgid()==0 ){
    fprintf(stderr,"%s: execution by the superuser is disallowed\n", argv[0]);
    exit(1);
  }

  /* Change into the project directory. */
  if( argc>=3 && chdir(argv[2]) ){
    fprintf(stderr,"%s: unable to change directories to %s\n", argv[0],argv[2]);
    exit(1);
  }

#if CVSTRAC_I18N
  /* Set the appropriate locale */
  setlocale(LC_ALL, "");
  g.useUTF8 = (strcmp(nl_langinfo(CODESET), "UTF-8") == 0);
#endif

  /* Set up global variable g
  */
  g.argc = argc;
  g.argv = argv;
  if( argc>=4 ){
    /* The project name is specified on the command-line */
    g.zName = argv[3];
    cmdlineProj = 1;
  }else{
    /* No project name on the command line.  Get the project name from
    ** either the URL or the HTTP_HOST parameter of the request.
    */
    i = strlen(argv[0]);
    while( i>0 && argv[0][i-1]!='/' ){ i--; }
    g.zName = mprintf("%s", &argv[0][i]);
    cmdlineProj = 0;
  }

  /* Figure out our behavior based on command line parameters and
  ** the environment.  
  */
  if( strcmp(argv[1],"cgi")==0 /* || getenv("GATEWAY_INTERFACE")!=0 */ ){
    cgi_init();
  }else if( strcmp(argv[1],"http")==0 ){
    cgi_handle_http_request();
  }else if( strcmp(argv[1],"init")==0 ){
    if( getuid()!=geteuid() ){
      fprintf(stderr,"Permission denied\n");
      exit(1);
    }
    db_init();
    exit(0);
  }else if( strcmp(argv[1],"wikiinit")==0 ){
    if( getuid()!=geteuid() ){
      fprintf(stderr,"Permission denied\n");
      exit(1);
    }
    initialize_wiki_pages();
    exit(0);
  }else if( strcmp(argv[1],"update")==0 ){
    check_schema();
    history_update(0);
    exit(0);
  }else if( strcmp(argv[1],"testcgi")==0 ){
    cgi_init();
    test_cgi_vardump();
    cgi_reply();
    exit(0);
  }else{
    usage(argv[0]);
  }

  /* Find the page that the user has requested, construct and deliver that
  ** page.
  */
  zPath = getenv("PATH_INFO");
  if( zPath==0 || zPath[0]==0 ){
    char *zBase, *zUri;
    zUri = getenv("REQUEST_URI");
    if( zUri==0 ) zUri = "/";
    for(i=0; zUri[i] && zUri[i]!='?' && zUri[i]!='#'; i++){}
    for(j=i; j>0 && zUri[j-1]!='/'; j--){}
    if( i==j ){
      cgi_set_status(404,"Not Found");
      @ <h1>Not Found</h1>
      @ <p>Page not found: %h(zUri)</p>
    }else{
      zBase = mprintf("%.*s/index", i-j, &zUri[j]);
      cgi_redirect(zBase);
    }
    cgi_reply();
    return 0;
  }

  /* 
  ** Extract the project name from the front of the path if no project
  ** was specified on the command line.
  */
  if( !cmdlineProj ){
    while(zPath[0]=='/') {zPath++;}    /* eat leading '/' */
    for(i=0; zPath[i] && zPath[i]!='/'; i++){}
    if( i>0 ){
      g.zName = mprintf("%.*s", i, zPath);
      zPath = &zPath[i];    
    }else{
      cgi_set_status(404,"Not Found");
      @ <h1>Not Found</h1>
      @ <p>Page not found: %h(zPath)</p>
      cgi_reply();
      return 0;
    }
  }
  while(zPath[0]=='/') {zPath++;}    /* eat leading '/' */
  g.zPath = zPath;
  for(i=0; zPath[i] && zPath[i]!='/'; i++){}
  if( zPath[i]=='/' ){
    zPath[i] = 0;
    g.zExtra = &zPath[i+1];

    /* CGI parameters get this treatment elsewhere, but places like getfile
    ** will use g.zExtra directly.
    */
    dehttpize(g.zExtra);
  }else{
    g.zExtra = 0;
  }

  g.zBaseURL = get_base_url();

  /* Prevent robots from indexing this site.
  */
  if( strcmp(g.zPath, "robots.txt")==0 ){
    cgi_set_content_type("text/plain");
    @ User-agent: *
    @ Disallow: /
    cgi_reply();
    exit(0);
  }

  /* Make sure the specified project really exists.  Return an error
  ** if it does not.
  */
  zDb = mprintf("%s.db", g.zName);
  if( access(zDb,0) ){
    free(zDb);
    zDb = mprintf("%s.db", g.zPath);
    if( !cmdlineProj && access(zDb,0)==0 ){
      cgi_redirect( mprintf("%s/index", g.zPath) );
    }else{
      cgi_set_status(404,"Not Found");
      @ <h1>Not Found</h1>
      @ <p>Project not found: %h(g.zName)</p>
      @ <p>Page not found: %h(g.zPath)</p>
    }
    cgi_reply();
    return 0;
  }
  free(zDb);

  check_schema();

  /* Ensure the CVSTrac process doesn't live indefinitely. If it takes more
  ** than this long, you're doing something wrong.
  */
  alarm(MX_CHILD_LIFETIME);
  
  /* Make a log file entry for this access.
  */
  zLogFile = db_config("logfile", 0);
  if( zLogFile ){
    cgi_logfile(zLogFile,"*");
  }

  /* Locate the method specified by the path and execute the function
  ** that implements that method.
  */
  if( !find_path(g.zPath, &xFunc) && !find_path("not_found",&xFunc) ){
    char *atn = db_short_query("SELECT atn FROM attachment "
                               "WHERE tn=0 AND fname='%q' "
                               "ORDER BY date DESC LIMIT 1", g.zPath);
    if( atn && *atn ){
      attachment_output(atoi(atn));
      free(atn);

      /* it's not actually constant because the URL can point at different
      ** attachments over time. */
      g.isConst = 0;
    }else{
      cgi_set_status(404,"Not Found");
      @ <h1>Not Found</h1>
      @ <p>Page not found: %h(g.zPath)</p>
    }
  }else{
    xFunc();
  }

  /* Return the result.
  */
  cgi_reply();
  return 0;
}