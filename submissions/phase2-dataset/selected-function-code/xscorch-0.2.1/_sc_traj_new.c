static inline sc_trajectory *_sc_traj_new(const sc_config *c, const sc_player *p,
                                          sc_trajectory_type type, int flags,
                                          double centerx, double centery) {
/* sc_traj_new
   Create a new trajectory structure with "reasonable" defaults.  Note
   that "reasonable" is defined by the person who wrote most of this
   file, namely Me, Justin, the programmer of DHOOM.  You would be very
   well justified to question my idea of "reasonable" at this stage.

   The trajectory is initialized with the centerx,centery coordinates
   as its starting position.  It will use the config structure to setup
   initial acceleration; velocity is cleared.  The flags can default
   to SC_TRAJ_DEFAULT.  */

   sc_trajectory *tr;         /* Storage location for trajectory */

   assert(c != NULL);
   assert(p != NULL);

   /* Attempt to allocate the memory */
   tr = (sc_trajectory *)malloc(sizeof(sc_trajectory));
   if(tr == NULL) return(NULL);

   /* Configure the type */
   tr->type = type;

   /* Initialise origin coordinate */
   tr->ctrx = centerx;
   tr->ctry = centery;

   /* Initialise velocity */
   tr->velx = 0;
   tr->vely = 0;

   /* Initialise accelerations */
   tr->accx = (flags & SC_TRAJ_IGNORE_WIND) ? 0 : c->physics->curwind;
   tr->accy = c->physics->gravity;
   tr->oaccx = tr->accx;
   tr->oaccy = tr->accy;

   /* Initialise "current" state */
   _sc_traj_reset(tr);

   /* Set terminus step */
   tr->stopstep = (flags & SC_TRAJ_BRIEF ? SC_TRAJ_TIMEOUT / 3 : SC_TRAJ_TIMEOUT);

   /* Clear out landfall params */
   tr->landfall_x1 = c->fieldwidth;
   tr->landfall_x2 = 0;

   /* Make the player assignment */
   tr->player = p;

   /* Victim setup */
   tr->victim = -1;

   /* Return the new structure */
   return(tr);

}
