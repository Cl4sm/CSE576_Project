
   char buf[SC_GTK_STRING_BUFFER];
   sc_player *p;
   int deltamoney;
   int i;
   int j;

   for(i = 0; i < w->c->numplayers; ++i) {
      j = 3;
      p = w->c->players[i];
      deltamoney = p->money - p->oldmoney;

      sbprintf(buf, sizeof(buf), "%2d %-12s %-16s  %3d  %3d  %3d  ", 
               p->index + 1, sc_ai_name(p->aitype), p->name, 
               p->numwins, p->kills, p->suicides);
      sc_console_write_line(SC_CONSOLE(console), j, i + 2, buf);
      j += strblenn(buf, SC_GTK_STRING_BUFFER);

      if(p->killedby >= 0) {
         sbprintf(buf, sizeof(buf), "%2d  ", p->killedby + 1);
      } else {
         strcopyb(buf, "    ", sizeof(buf));
      }
      sc_console_write_line(SC_CONSOLE(console), j, i + 2, buf);
      sc_console_highlight_attach(SC_CONSOLE(console), &w->colormap->pcolors[p->index], NULL, FALSE, 3, i + 2, 2, 1);
      j += strblenn(buf, SC_GTK_STRING_BUFFER);
      
      sbprintf(buf, sizeof(buf), "$%-9d  %c$%d", p->money, (deltamoney < 0 ? '-' : ' '), abs(deltamoney));
      sc_console_write_line(SC_CONSOLE(console), j, i + 2, buf);
   }
   
}
