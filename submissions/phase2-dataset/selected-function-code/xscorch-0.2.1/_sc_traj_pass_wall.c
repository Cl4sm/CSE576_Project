static sc_trajectory_result _sc_traj_pass_wall(const sc_config *c, sc_trajectory *tr, sc_trajectory_data *t) {
/* sc_traj_pass_wall
   Determine whether we hit a wall with the weapon at (t).  Action depends
   on the wall type.  This function may choose to detonate here.  Returns
   NO_ACTION if the weapon status hasn't changed lately.  */

   assert(c != NULL && tr != NULL && t != NULL);

   switch(c->physics->walls) {
      case SC_WALL_NONE:
      case SC_WALL_RANDOM:
         return(_sc_traj_pass_wall_none(c, tr));

      case SC_WALL_WRAP:
         return(_sc_traj_pass_wall_wrap(c, tr));

      case SC_WALL_CONCRETE:
         return(_sc_traj_pass_wall_concrete(c, tr));

      case SC_WALL_PADDED:
         return(_sc_traj_pass_wall_elastic(c, tr, t, SC_PHYSICS_ELASTIC_PADDED));

      case SC_WALL_RUBBER:
         return(_sc_traj_pass_wall_elastic(c, tr, t, SC_PHYSICS_ELASTIC_RUBBER));

      case SC_WALL_SPRING:
         return(_sc_traj_pass_wall_elastic(c, tr, t, SC_PHYSICS_ELASTIC_SPRING));
   }
   return(SC_TRAJ_CONTINUE);

}
