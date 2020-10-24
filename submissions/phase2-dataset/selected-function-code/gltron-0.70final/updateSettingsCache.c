  /* cache lua settings that don't change during play */
  gSettingsCache.use_stencil = getSettingi("use_stencil");
  gSettingsCache.show_scores = getSettingi("show_scores");
  gSettingsCache.show_ai_status = getSettingi("show_ai_status");
  gSettingsCache.ai_level = getSettingi("ai_level");
  gSettingsCache.show_fps = getSettingi("show_fps");
  gSettingsCache.show_console = getSettingi("show_console");
  gSettingsCache.softwareRendering = getSettingi("softwareRendering");
  gSettingsCache.line_spacing = getSettingi("line_spacing");
  gSettingsCache.alpha_trails = getSettingi("alpha_trails");
  gSettingsCache.antialias_lines = getSettingi("antialias_lines");
  gSettingsCache.turn_cycle = getSettingi("turn_cycle"); 
  gSettingsCache.light_cycles = getSettingi("light_cycles"); 
  gSettingsCache.lod = getSettingi("lod"); 
  gSettingsCache.fov = getSettingi("fov"); 

  gSettingsCache.show_floor_texture = getVideoSettingi("show_floor_texture");
  gSettingsCache.show_skybox = getVideoSettingi("show_skybox"); 
  gSettingsCache.show_wall = getVideoSettingi("show_wall");
  gSettingsCache.stretch_textures = getVideoSettingi("stretch_textures"); 
  gSettingsCache.show_decals = getVideoSettingi("show_decals");

  gSettingsCache.show_impact = getSettingi("show_impact");
  gSettingsCache.show_glow = getSettingi("show_glow"); 
  gSettingsCache.show_recognizer = getSettingi("show_recognizer");

  gSettingsCache.fast_finish = getSettingi("fast_finish");
  gSettingsCache.fov = getSettingf("fov");
  gSettingsCache.znear = getSettingf("znear");
  gSettingsCache.camType = getSettingi("camType");
  gSettingsCache.playEffects = getSettingi("playEffects");
  gSettingsCache.playMusic = getSettingi("playMusic");
	gSettingsCache.map_ratio_w = getSettingf("map_ratio_w");
	gSettingsCache.map_ratio_h = getSettingf("map_ratio_h");

	scripting_GetGlobal("clear_color", NULL);
  scripting_GetFloatArrayResult(gSettingsCache.clear_color, 4);
}
