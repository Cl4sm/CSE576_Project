
   int size;
   const sc_weapon_info *wi;
   const sc_accessory_info *si;
   char buf[SC_GTK_STRING_BUFFER];
   sc_window_gtk *w = (sc_window_gtk *)w_;
   int start;
   int spot_start;
   int end_line_one;

   if(w == NULL) return;
   wi = p->selweapon;
   si = p->selshield;
   if(wi == NULL || si == NULL) {
      fprintf(stderr, "warning: player is missing either shields or weapons, status NOT updated!\n");
      return;
   }

   sc_window_redraw_tank(w_, p);

   if(p->aitype == SC_AI_NETWORK) return;

   size = w->c->fieldwidth / sc_console_get_col_width(SC_CONSOLE(w->status));

   _sc_status_reset_state(w, TRUE);
   _sc_status_write_line(w, p, size - strlenn(p->name) - 2, size < SC_TWO_ROW_BOUNDARY ? 1 : 0,
                         p->name, SC_STATUS_PLAYER);

   if(size < SC_TWO_ROW_BOUNDARY) {
      /* We must scale player tank life to display in game life units. */
      sbprintf(buf, sizeof(buf), "Ang: %3d %c  Pwr: %4d  %c  Shld: %3d%%  %3d%c",
               (p->turret >= 90 ? 180 - p->turret : p->turret), p->turret > 90 ? '<' : '>',
               p->power, (p->contacttriggers ? 'T' : ' '),
               p->shield != NULL ? p->shield->life * 100 / p->shield->info->shield : 0,
               si->shield / 100, SC_ACCESSORY_SHIELD_CHAR(si));
      sc_console_write_line(SC_CONSOLE(w->status), 1, 0, buf);
      sbprintf(buf, sizeof(buf), "%2d %-16s   Wind: %4d %c",
               wi->inventories[p->index], wi->name,
               abs((int)(w->c->physics->curwind * 1000 / SC_PHYSICS_WIND_MAX)),
                   (w->c->physics->curwind >= 0 ? '>' : '<'));
      sc_console_write_line(SC_CONSOLE(w->status), 1, 1, buf);
   } else {
      /* Skip the first column */
      start = 1;

      sbprintf(buf, sizeof(buf), "[Ang: %2d %c]",
               p->turret >= 90 ? 180 - p->turret : p->turret, p->turret > 90 ? '<' : '>');
      _sc_status_write_line(w, p, start, 0, buf, SC_STATUS_NORMAL);
      start += 11;

      sbprintf(buf, sizeof(buf), "[Pwr: %4d]", p->power);
      _sc_status_write_line(w, p, start, 0, buf, SC_STATUS_NORMAL);
      start += 11;

      _sc_status_write_line(w, p, start, 0, "[Shld:           Cur:    ]", SC_STATUS_NORMAL);
      if(SC_ACCESSORY_IS_NULL(si)) {
         sbprintf(buf, sizeof(buf), "    None");
      } else {
         sbprintf(buf, sizeof(buf), "%2dx %3d%c", si->inventories[p->index],
                  si->shield / 100, SC_ACCESSORY_SHIELD_CHAR(si));
      }
      spot_start = start + 7;
      sc_active_console_add_spot(SC_ACTIVE_CONSOLE(w->status), spot_start, 0, 8, 1, (gpointer)SC_STATUS_TOGGLE_SHIELD);
      _sc_status_write_line(w, p, spot_start, 0, buf, SC_ACCESSORY_IS_NULL(si) ? SC_STATUS_BOLD : SC_STATUS_NORMAL);
      spot_start += 14;
      sc_active_console_add_spot(SC_ACTIVE_CONSOLE(w->status), spot_start, 0, 4, 1, (gpointer)SC_STATUS_ACTIVATE_SHIELD);
      sbprintf(buf, sizeof(buf), "%3d%%", p->shield != NULL ? p->shield->life * 100 / p->shield->info->shield : 0);
      _sc_status_write_line(w, p, spot_start, 0, buf, p->shield == NULL ? SC_STATUS_BOLD : SC_STATUS_NORMAL);
      start += 26;

      _sc_status_write_line(w, p, start, 0, "[                       ]", SC_STATUS_NORMAL);
      if(SC_WEAPON_IS_NULL(wi)) {
         sbprintf(buf, sizeof(buf), "   ");
      } else {
         sbprintf(buf, sizeof(buf), "%2dx", wi->inventories[p->index]);
      }
      spot_start = start + 1;
      sc_active_console_add_spot(SC_ACTIVE_CONSOLE(w->status), spot_start, 0, 23, 1, (gpointer)SC_STATUS_TOGGLE_WEAPON);
      _sc_status_write_line(w, p, spot_start, 0, buf, SC_STATUS_NORMAL);
      spot_start += 4;
      sbprintf(buf, sizeof(buf), "%-16s ", wi->name);
      _sc_status_write_line(w, p, spot_start, 0, buf, SC_WEAPON_IS_NULL(wi) ? SC_STATUS_BOLD : SC_STATUS_NORMAL);
      spot_start += 18;
      sbprintf(buf, sizeof(buf), "%c", p->contacttriggers ? 'T' : ' ');
      _sc_status_write_line(w, p, spot_start, 0, buf, SC_STATUS_NORMAL);
      start += 25;
      end_line_one = start;

      if(w->c->options.extstatus) {
         start = 1;
         sbprintf(buf, sizeof(buf), "Round %d/%d", w->c->curround + 1, w->c->numrounds);
         _sc_status_write_line(w, p, size - strlenn(buf) - 2, 1, buf, SC_STATUS_NORMAL);

         sbprintf(buf, sizeof(buf), "[Life: %4d]", INT_ROUND_DIV(p->life, p->tank->hardness));
         _sc_status_write_line(w, p, start, 1, buf, SC_STATUS_PLAYER);
         start += 12;

         sbprintf(buf, sizeof(buf), "[Batt: %2d]", sc_player_battery_count(w->c, p));
         spot_start = start + 1;
         sc_active_console_add_spot(SC_ACTIVE_CONSOLE(w->status), spot_start, 1, 8, 1, (gpointer)SC_STATUS_ACTIVATE_BATTERY);
         _sc_status_write_line(w, p, start, 1, buf, SC_STATUS_PLAYER);
         start += 10;

         sbprintf(buf, sizeof(buf), "[Trig: %2d]", sc_player_contact_trigger_count(w->c, p));
         spot_start = start + 1;
         sc_active_console_add_spot(SC_ACTIVE_CONSOLE(w->status), spot_start, 1, 8, 1, (gpointer)SC_STATUS_TOGGLE_TRIGGER);
         _sc_status_write_line(w, p, start, 1, buf, SC_STATUS_PLAYER);
         start += 10;

         sbprintf(buf, sizeof(buf), "[Fuel: %4d]", sc_player_total_fuel(w->c->accessories, p));
         spot_start = start + 1;
         sc_active_console_add_spot(SC_ACTIVE_CONSOLE(w->status), spot_start, 1, 10, 1, (gpointer)SC_STATUS_MOVE_PLAYER);
         _sc_status_write_line(w, p, start, 1, buf, SC_STATUS_PLAYER);
         start += 28;

         sbprintf(buf, sizeof(buf), "[Wind: %3d %c]",
                  abs((int)(w->c->physics->curwind * 1000 / SC_PHYSICS_WIND_MAX)),
                      (w->c->physics->curwind >= 0 ? '>' : '<'));
         _sc_status_write_line(w, p, start, 1, buf, SC_STATUS_NORMAL);
         start += 13;
         assert(end_line_one == start);
      } /* Extended status */
   }

}
