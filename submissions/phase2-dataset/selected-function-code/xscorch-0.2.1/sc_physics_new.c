sc_physics *sc_physics_new(void) {
/* sc_physics_new
   Create a new physics parametre structure.  */

   sc_physics *ph;         /* New structure */

   /* Allocate memory for the parametre structure */
   ph = (sc_physics *)malloc(sizeof(sc_physics));
   if(ph == NULL) return(NULL);

   /* Initialise the various parametres */
   ph->airviscosity = 0;
   ph->gravity = SC_PHYSICS_GRAVITY_DEF;
   ph->damping = SC_TRAJ_TUNNEL_DAMPING;
   ph->maxwind = SC_PHYSICS_WIND_DEF;
   ph->suspenddirt = 0;
   ph->tanksfall = 100;
   ph->dynamicwind = true;
   ph->bordersextend = SC_PHYSICS_BORDERS_DEF;
   ph->walls = SC_WALL_WRAP;

   /* Return the new structure */
   return(ph);

}
