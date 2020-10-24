static double _sc_traj_timesteps(const sc_trajectory *tr, double y0) {
/* sc_traj_timesteps
   Estimate the number of timesteps to reach coordinate y0, given the
   y velocity and acceleration defined in the trajectory.  Returns -1
   if y0 is unattainable.  */

   double root;      /* Calculation for sqrt() part */
   double time;      /* Timestep this occurs at */

   assert(tr != NULL);

   /* Calculate value in the inner sqrt */
   root = SQR(tr->vely) - 2 * tr->accy * y0;
   if(root < 0) {
      return(-1);
   }

   /* Calculate the timesteps */
   time = 1 / tr->accy * (tr->vely + sqrt(root));
   return(time);

}
