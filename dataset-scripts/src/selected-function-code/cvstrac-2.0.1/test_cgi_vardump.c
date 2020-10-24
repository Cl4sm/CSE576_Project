void test_cgi_vardump(void){
  int i;
  char zPwd[1000];
  getcwd(zPwd, sizeof(zPwd));
  @ <html>
  @ <head><title>Test Page</title></head>
  @ <body bgcolor="white">
  @ <h1>Test Page</h1>
  cgi_print_all();
  @ <h2>Environment variables:</h2>
  @ <p>
  for(i=0; environ[i]; i++){
    @ %h(environ[i]) <br />
  }
  @ </p>
  @ <h2>Working directory and user id</h2>
  @ <p>
  @ pwd = %s(zPwd)<br />
  @ uid = %d(getpid())<br />
  @ euid = %d(geteuid())<br />
  @ argc = %d(g.argc)<br />
  for(i=0; i<g.argc; i++){
    @ argv[%d(i)] = %s(g.argv[i])<br />
  }
  @ </p>
  @ </body></html>
}