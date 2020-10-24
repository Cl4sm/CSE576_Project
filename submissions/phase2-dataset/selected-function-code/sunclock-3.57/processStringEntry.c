processStringEntry(keysym, entry)
KeySym keysym;
TextEntry *entry;
{
int i, j;
           i = strlen(entry->string);

           switch(keysym) {
             case XK_Left:
               if (entry->caret>0) --entry->caret;
               break;
             case XK_Right:
               if (entry->caret<i) ++entry->caret;
               break;
             case XK_Home:
               entry->caret = 0;
               break;
             case XK_End:
               entry->caret = strlen(entry->string);
               break;
             case XK_BackSpace:
             case XK_Delete:
               if (entry->caret>0) {
                  --entry->caret;
                  for (j=entry->caret; j<i;j++)
                     entry->string[j] = entry->string[j+1];
               }
               break;
             default:
               if (control_key) {
                  if (keysym==XK_space) {
                     keysym = 31;
                     goto specialspace;
                  }
                  if (keysym==XK_a) entry->caret = 0;
                  if (keysym==XK_b && entry->caret>0) --entry->caret;
                  if (keysym==XK_e) entry->caret = i;
                  if (keysym==XK_f && entry->caret<i) ++entry->caret;
                  if (keysym==XK_d) {
                     for (j=entry->caret; j<i;j++)
                        entry->string[j] = entry->string[j+1];
                  }
                  if (keysym==XK_k) {
                     entry->oldcaret = entry->caret;
                     entry->oldlength = i;
                     entry->oldchar = entry->string[entry->caret];
                     entry->string[entry->caret] = '\0';
                  }
                  if (keysym==XK_y && entry->caret==entry->oldcaret) {
                     entry->string[entry->oldcaret] = entry->oldchar;
                     entry->string[entry->oldlength] = '\0';
                     entry->oldcaret = -1;
                  }
                  break;
               }
           specialspace:
               if (keysym<31) break;
	       if (keysym>=XK_KP_Multiply && keysym<=XK_KP_9) 
                   keysym = keysym - XK_KP_0 + '0';
               if (keysym>255) break;
               if (i<entry->maxlength) {
                  for (j=i; j>entry->caret; j--)
                     entry->string[j] = entry->string[j-1];
                  entry->string[entry->caret] = (char) keysym;
                  entry->string[i+1] = '\0';
                  ++entry->caret;
               }
               break;
           }
}
