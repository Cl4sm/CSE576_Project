                                        sc_physics_setup_data_gtk *setup) {

   sc_physics *ph = setup->ph;
   
   ph->airviscosity = setup->airviscosity;
   ph->gravity      = setup->gravity;
   ph->damping      = setup->damping;
   ph->maxwind = setup->maxwind;
   ph->dynamicwind  = setup->dynamicwind;
   ph->suspenddirt  = setup->suspenddirt;
   ph->tanksfall    = setup->tanksfall;
   ph->bordersextend= setup->bordersextend;
   ph->walls        = sc_physics_wall_types()[setup->wallidx];

   #if USE_NETWORK
   if(SC_NETWORK_SERVER(setup->c)) sc_net_server_send_config(setup->c, setup->c->server);
   #endif
   
}
