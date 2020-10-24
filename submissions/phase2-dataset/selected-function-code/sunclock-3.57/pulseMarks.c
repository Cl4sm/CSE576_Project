void
pulseMarks(Context)
struct Sundata * Context;
{
int     done = 0;
        
        if (!Context->wintype) return; 
        if (Context->flags.colorlevel == FULLCOLORS) {
           if (!Context->xim) return;
        } else {
           if (!Context->mappix) return;
        }
        if (Context->mark1.city && Context->mark1.flags<0) {
           if (Context->mark1.pulse) {
             drawObject(Context, Context->mark1.save_lon, 
                   Context->mark1.save_lat, -1, 0, NULL);
             done = 1;
           }
           Context->mark1.save_lat = Context->mark1.city->lat;
           Context->mark1.save_lon = Context->mark1.city->lon;
           if (Context->mark1.city == &Context->pos1) {
              done = 1;
              drawObject(Context, Context->mark1.save_lon, 
                                Context->mark1.save_lat, -1, 0, NULL);
              Context->mark1.pulse = 1;
           } else
              Context->mark1.pulse = 0;
           Context->mark1.flags = 1;
        }
        else
        if (Context->mark1.flags>0) {
           if (Context->mark1.city|| Context->mark1.pulse) {
              drawObject(Context, Context->mark1.save_lon, 
                                Context->mark1.save_lat, -1, 0, NULL);
              Context->mark1.pulse = 1-Context->mark1.pulse;
              done = 1;
           }
           if (Context->mark1.city == NULL) Context->mark1.flags = 0;
        }

        if (Context->mark2.city && Context->mark2.flags<0) {
           if (Context->mark2.pulse) {
             drawObject(Context, Context->mark2.save_lon, 
                               Context->mark2.save_lat, -1, 0, NULL);
             done = 1;
           }
           Context->mark2.save_lat = Context->mark2.city->lat;
           Context->mark2.save_lon = Context->mark2.city->lon;
           if (Context->mark2.city == &Context->pos2) {
              drawObject(Context, Context->mark2.save_lon, 
                       Context->mark2.save_lat, -1, 0, NULL);
              done = 1;
              Context->mark2.pulse = 1;
           } else
              Context->mark2.pulse = 0;
           Context->mark2.flags = 1;
        }
        else
        if (Context->mark2.flags>0) {
           if (Context->mark2.city || Context->mark2.pulse) {
              drawObject(Context, Context->mark2.save_lon, 
                                Context->mark2.save_lat, -1, 0, NULL);
              Context->mark2.pulse = 1 - Context->mark2.pulse;
              done = 1;
           }
           if (Context->mark2.city == NULL) Context->mark2.flags = 0;
        }
        if (done) {
           Context->flags.update = 2;
           showMapImage(Context);
        }
}
