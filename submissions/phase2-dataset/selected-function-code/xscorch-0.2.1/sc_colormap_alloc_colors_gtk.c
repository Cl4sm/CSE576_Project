
   sc_color_gtk *colormap = w->colormap;
   sc_color *colors = w->c->colors;

   /* Initialise gradient subsystem */
   _sc_colormap_gradient_gtk(w, SC_GRAD_GROUND,          0,    0.47, 0,    0,    0.72, 0);
   _sc_colormap_gradient_gtk(w, SC_GRAD_NIGHT_SKY,       0,    0,    0.05, 0,    0,    0.45);
   _sc_colormap_gradient_gtk(w, SC_GRAD_FIRE_SKY,        1.00, 0.75, 0.25, 0.37, 0,    0);
   _sc_colormap_gradient_gtk(w, SC_GRAD_EXPLOSION,       0.80, 0,    0,    0.20, 0,    0);
   _sc_colormap_gradient_gtk(w, SC_GRAD_FUNKY_EXPLOSION, 0.80, 0.40, 0,    0.50, 0.10, 0);
   _sc_colormap_gradient_gtk(w, SC_GRAD_MAGNETIC,        0,    0,    0,    1.00, 0.50, 0.00);
   _sc_colormap_gradient_gtk(w, SC_GRAD_SHIELD,          0,    0,    0,    1.00, 1.00, 1.00);
   _sc_colormap_gradient_gtk(w, SC_GRAD_FORCE,           0,    0,    0,    1.00, 0,    1.00);
   _sc_colormap_gradient_gtk(w, SC_GRAD_FLAMES,          0.40, 0.00, 0.00, 0.80, 0.40, 0.00);

   _sc_colormap_set_gtk(w, &colormap->black, 0.0, 0.0, 0.0);
   _sc_colormap_set_gtk(w, &colormap->gray,  0.5, 0.5, 0.5);
   _sc_colormap_set_gtk(w, &colormap->white, 1.0, 1.0, 1.0);
   _sc_colormap_set_gtk(w, &colormap->yellow,1.0, 1.0, 0.0);
   _sc_colormap_set_gtk(w, &colormap->napalm,1.0, 0.4, 0.1);
   _sc_colormap_set_gtk(w, &colormap->windar,0.7, 0.7, 1.0);

   _sc_colormap_set_gtk(w, &colormap->pcolors[0], 1.0, 0.1, 0.1);
   _sc_colormap_set_gtk(w, &colormap->pcolors[1], 1.0, 1.0, 0.2);
   _sc_colormap_set_gtk(w, &colormap->pcolors[2], 0.5, 0.5, 0.5);
   _sc_colormap_set_gtk(w, &colormap->pcolors[3], 0.2, 1.0, 1.0);
   _sc_colormap_set_gtk(w, &colormap->pcolors[4], 0.2, 0.2, 1.0);
   _sc_colormap_set_gtk(w, &colormap->pcolors[5], 1.0, 0.2, 1.0);
   _sc_colormap_set_gtk(w, &colormap->pcolors[6], 1.0, 1.0, 1.0);
   _sc_colormap_set_gtk(w, &colormap->pcolors[7], 0.2, 1.0, 0.2);
   _sc_colormap_set_gtk(w, &colormap->pcolors[8], 0.7, 0.7, 1.0);
   _sc_colormap_set_gtk(w, &colormap->pcolors[9], 1.0, 0.3, 0.05);

   sc_color_gradient_init(w->c, colors);

}
