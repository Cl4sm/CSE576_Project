static inline sc_trajectory_result _sc_traj_pass_wall_elastic(const sc_config *c, sc_trajectory *tr,
                                                              sc_trajectory_data *t, double elasticity) {
/* sc_traj_pass_wall_elastic
   Boundary case for elastic walls, with elasticity given (values < 1, wall
   absorbs some velocity, > 1, wall adds more velocity).  */

   assert(c != NULL && tr != NULL && t != NULL);
   assert(elasticity > 0);

   /* Check if we hit sidewalls. */
   if(rint(tr->curx) < 0) {
      tr->curx =  0;
      tr->ctrx =  tr->curx;
      tr->ctry =  tr->cury;
      tr->velx = -sc_traj_get_velocity_x(tr) * elasticity;
      tr->vely =  sc_traj_get_velocity_y(tr);
      tr->stopstep -= tr->timestep;
      tr->timestep  = 0;
      tr->finalstep = 0;
      t->stepx  =  fabs(t->stepx) * elasticity;
      t->stepy *=  elasticity;
   } else if(rint(tr->curx) >= c->fieldwidth) {
      tr->curx =  c->fieldwidth - 1;
      tr->ctrx =  tr->curx;
      tr->ctry =  tr->cury;
      tr->velx = -sc_traj_get_velocity_x(tr) * elasticity;
      tr->vely =  sc_traj_get_velocity_y(tr);
      tr->stopstep -= tr->timestep;
      tr->timestep  = 0;
      tr->finalstep = 0;
      t->stepx  = -fabs(t->stepx) * elasticity;
      t->stepy *=  elasticity;
   }

   /* Check if we hit ground or ceiling. */
   if(rint(tr->cury) < 0) {
      /* Rollers need to feel solid earth under their feet (otherwise infinite loop). */
      if(tr->type == SC_TRAJ_ROLLER)
         return(SC_TRAJ_IMPACT_GROUND);

      tr->cury =  0;
      tr->ctry =  tr->cury;
      tr->ctrx =  tr->curx;
      tr->vely = -sc_traj_get_velocity_y(tr) * elasticity;
      tr->velx =  sc_traj_get_velocity_x(tr);
      tr->stopstep -= tr->timestep;
      tr->timestep  = 0;
      tr->finalstep = 0;
      t->stepy  =  fabs(t->stepy) * elasticity;
      t->stepx *=  elasticity;
   } else if(rint(tr->cury) >= c->fieldheight) {
      tr->cury =  c->fieldheight - 1;
      tr->ctry =  tr->cury;
      tr->ctrx =  tr->curx;
      tr->vely = -sc_traj_get_velocity_y(tr) * elasticity;
      tr->velx =  sc_traj_get_velocity_x(tr);
      tr->stopstep -=  tr->timestep;
      tr->timestep  =  0;
      tr->finalstep =  0;
      t->stepy  = -fabs(t->stepy) * elasticity;
      t->stepx *=  elasticity;
   }

   /* Not much occurred ... */
   return(SC_TRAJ_CONTINUE);

}
