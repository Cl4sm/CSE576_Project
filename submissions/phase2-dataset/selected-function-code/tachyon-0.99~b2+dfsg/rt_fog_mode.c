  scenedef * scene = (scenedef *) voidscene;

  switch (mode) {
    case RT_FOG_LINEAR:
      scene->fog.fog_fctn = fog_color_linear;
      break;

    case RT_FOG_EXP:
      scene->fog.fog_fctn = fog_color_exp;
      break;

    case RT_FOG_EXP2:
      scene->fog.fog_fctn = fog_color_exp2;
      break;

    case RT_FOG_NONE: 
    default:
      scene->fog.fog_fctn = NULL;
      break;
  }
}
