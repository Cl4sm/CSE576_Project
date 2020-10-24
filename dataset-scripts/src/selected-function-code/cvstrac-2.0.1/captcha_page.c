void captcha_page(void){
  time_t now = time(NULL);
  int q1, q2;

  if( atoi(PD("a","0")) == (atoi(PD("q1","-1")) + atoi(PD("q2","-1"))) ){
    /* User gave the right answer. Set cookie and continue on.
    */
    captcha_set_cookie();
    cgi_redirect(PD("nxp","index"));
    return;
  }

  /* Note that we don't do _any_ credential checks in this page... However,
  ** some flags are needed for sane header generation. For example, we
  ** want a "Login" in the menu rather than "Logout" so isAnon should be
  ** set.
  */
  g.isAnon = 1;

  common_standard_menu(0, 0);
  common_add_help_item("CvstracAdminAbuse");
  common_header("Abbreviated Turing Test");

  /* small numbers */
  srand(now);
  q1 = (rand()%5)+1;
  q2 = (rand()%5)+1;

  @ In order to continue, you must show you're a human. Please answer
  @ the following mathematical skill testing question (and ensure cookies
  @ are enabled):
  @ <p>
  @ <form action="captcha" method="POST">
  @ What is <tt>%d(q1) + %d(q2)</tt>?
  @ <input type="text" name="a" value="" size=4>
  @ <font size="-1">Hint: %d(q1+q2)</font>
  @ <input type="hidden" name="q1" value="%d(q1)">
  @ <input type="hidden" name="q2" value="%d(q2)">
  if( P("nxp") ){
    @ <input type="hidden" name="nxp" value="%h(P("nxp"))">
  }
  @ </p>
  @ <p>
  @ <input type="submit" name="in" value="Submit"></td>
  @ </p>
  @ </form>
  common_footer();
}