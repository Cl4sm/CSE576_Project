
   if(SC_AI_DEBUG_VICTIMS) {
      const sc_player *vp;
      const sc_player *p;
      int dx;
      int dy;

      if(c == NULL || tr == NULL || tr->player == NULL) return;
      p = tr->player;
      if(p->ai == NULL || p->ai->victim == NULL) return;

      /* We hit something; was it near intended victim? */
      vp = p->ai->victim;
      if(vp->index != tr->victim && tr->victim >= 0) {
         printf("trajectory(%d): intended victim %d, hit victim %d\n", 
                p->index, vp->index, tr->victim);
      }
      if(sc_land_calculate_deltas(c->land, &dx, &dy, vp->x, vp->y, tr->curx, tr->cury)) {
         printf("trajectory(%d): was %g pixels away from intended victim %d\n",
                p->index, sqrt(SQR(dx) + SQR(dy)), vp->index);
      }
   }
   
}
