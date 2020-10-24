void login_page(void){
  const char *zUsername, *zPasswd, *zGoto;
  const char *zNew1, *zNew2;
  char *zErrMsg = "";
  char *z;

  login_check_credentials();
  zUsername = P("u");
  zPasswd = P("p");
  zGoto = P("g");
  if( P("out")!=0 ){
    const char *zCookieName = login_cookie_name();
    cgi_set_cookie(zCookieName, "", 0, time(NULL)-(60*60*24));
    db_execute("DELETE FROM cookie WHERE cookie='%q'", P(zCookieName));
    cgi_redirect(PD("nxp","index"));
    return;
  }
  if( !g.isAnon && zPasswd && (zNew1 = P("n1"))!=0 && (zNew2 = P("n2"))!=0 ){
    z = db_short_query("SELECT passwd FROM user WHERE id='%q'", g.zUser);
    if( z==0 || z[0]==0 || strcmp(crypt(zPasswd,z),z)!=0 ){
      sleep(1);
      zErrMsg = 
         @ <p><font color="red">
         @ You entered an incorrect old password while attempting to change
         @ your password.  Your password is unchanged.
         @ </font></p>
      ;
    }else if( strcmp(zNew1,zNew2)!=0 ){
      zErrMsg = 
         @ <p><font color="red">
         @ The two copies of your new passwords do not match.
         @ Your password is unchanged.
         @ </font></p>
      ;
    }else{
      db_execute(
         "UPDATE user SET passwd='%q' WHERE id='%q'",
         crypt(zNew1,zPasswd), g.zUser
      );
      if( g.scm.pxUserWrite ) g.scm.pxUserWrite(0);
      cgi_redirect("index");
      return;
    }
  }
  if( zUsername!=0 && zPasswd!=0 && strcmp(zUsername,"anonymous")!=0 ){
    z = db_short_query("SELECT passwd FROM user WHERE id='%q'", zUsername);
    if( z==0 || z[0]==0 || strcmp(crypt(zPasswd,z),z)!=0 ){
      sleep(1);
      zErrMsg = 
         @ <p><font color="red">
         @ You entered an incorrect username and/or password
         @ </font></p>
      ;
    }else{
      time_t now;
      unsigned char *zDigest;
      const char *zAddr;
      const char *zAgent;
      unsigned char zHash[200];
      unsigned char zRawDigest[16];
      MD5Context ctx;

      time(&now);
      bprintf((char *)zHash,sizeof(zHash),"%d%d%.19s", getpid(), (int)now, zPasswd);
      MD5Init(&ctx);
      MD5Update(&ctx, zHash, strlen((char *)zHash));
      MD5Final(zRawDigest, &ctx);
      zDigest = encode64(zRawDigest, 16);
      zAddr = getenv("REMOTE_ADDR");
      if( zAddr==0 ) zAddr = "0.0.0.0";
      zAgent = getenv("HTTP_USER_AGENT");
      if( zAgent==0 ) zAgent = "Unknown";
      db_execute(
        "BEGIN;"
        "DELETE FROM cookie WHERE expires<=%d;"
        "INSERT INTO cookie(cookie,user,expires,ipaddr,agent)"
        "  VALUES('%q','%q',%d,'%q','%q');"
        "COMMIT;",
        now, zDigest, zUsername, now+3600*24, zAddr, zAgent
      );
      cgi_set_cookie(login_cookie_name(), (char *)zDigest, 0, 0);
      cgi_redirect(PD("nxp","index"));
      return;
    }
    free(z);
  }
  common_standard_menu("login", 0);
  common_add_help_item("CvstracLogin");
  common_header("Login/Logout");
  @ %s(zErrMsg)
  @ <form action="login" method="POST">
  if( P("nxp") ){
    @ <input type="hidden" name="nxp" value="%h(P("nxp"))">
  }
  @ <table align="left" hspace="10">
  @ <tr>
  @   <td align="right">User ID:</td>
  @   <td><input type="text" name="u" value="" size=30></td>
  @ </tr>
  @ <tr>
  @  <td align="right">Password:</td>
  @   <td><input type="password" name="p" value="" size=30></td>
  @ </tr>
  @ <tr>
  @   <td></td>
  @   <td><input type="submit" name="in" value="Login"></td>
  @ </tr>
  @ </table>
  if( g.isAnon ){
    @ <p>To login
  }else{
    @ <p>You are current logged in as <b>%h(g.zUser)</b></p>
    @ <p>To change your login to a different user
  }
  @ enter the user-id and password at the left and press the
  @ "Login" button.  Your user name will be stored in a browser cookie.
  @ You must configure your web browser to accept cookies in order for
  @ the login to take.</p>
  if( db_exists("SELECT id FROM user WHERE id='anonymous'") ){
    @ <p>This server is configured to allow limited access to users
    @ who are not logged in.</p>
  }
  if( !g.isAnon ){
    @ <br clear="both"><hr>
    @ <p>To log off the system (and delete your login cookie)
    @  press the following button:<br>
    @ <input type="submit" name="out" value="Logout"></p>
  }
  @ </form>
  if( !g.isAnon && g.okPassword ){
    @ <br clear="both"><hr>
    @ <p>To change your password, enter your old password and your
    @ new password twice below then press the "Change Password"
    @ button.</p>
    @ <form action="login" method="POST">
    @ <table>
    @ <tr><td align="right">Old Password:</td>
    @ <td><input type="password" name="p" size=30></td></tr>
    @ <tr><td align="right">New Password:</td>
    @ <td><input type="password" name="n1" size=30></td></tr>
    @ <tr><td align="right">Repeat New Password:</td>
    @ <td><input type="password" name="n2" size=30></td></tr>
    @ <tr><td></td>
    @ <td><input type="submit" value="Change Password"></td></tr>
    @ </table>
    @ </form>
  }
  common_footer();
}