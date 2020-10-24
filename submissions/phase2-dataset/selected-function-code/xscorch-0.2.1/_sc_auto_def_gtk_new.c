/* _sc_auto_def_gtk_new
   Set up a new auto_def_gtk */

   sc_auto_def_gtk *adg;

   adg = (sc_auto_def_gtk *)malloc(sizeof(sc_auto_def_gtk));
   if(adg == NULL) return(NULL);

   /* sc_auto_def_set items */
   adg->auto_guidance       = NULL;
   adg->auto_shield         = NULL;
   adg->chute_height        = 0;
   adg->triggers            = false;

   /* sc_auto_def_gtk items */
   adg->console_main        = NULL;
   adg->console_guidance    = NULL;
   adg->console_shield      = NULL;
   adg->console_parachute   = NULL;
   adg->console_triggers    = NULL;
   adg->console_activate    = NULL;
   adg->set_guidance.length = 0;
   adg->set_guidance.items  = NULL;
   adg->set_shield.length   = 0;
   adg->set_shield.items    = NULL;
   adg->trigger_toggle      = NULL;
   adg->w                   = NULL;
   adg->p                   = NULL;

   adg->ready               = false;

   return(adg);

}
