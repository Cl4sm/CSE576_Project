
   /*int voice;*/

   if(s == NULL) return;
   _sc_sound_reset(s);
   
   _sc_sound_try_load(s, id);
   if(s->module == NULL) return;
      
   Player_Start(s->module);
   /*
   Player_SetPosition(0);
   for(voice = 0; voice <= 99; ++voice) {
      Player_Unmute(voice);
      Voice_SetVolume(voice, 256);
      Voice_SetPanning(voice, PAN_SURROUND);
   }*/
   
   s->playing = 1;
   sc_sound_update(s);

}
