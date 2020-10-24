  scenedef * scene = (scenedef *) voidscene;

  /* Main shader used for whole scene */
  switch (mode) {
    case RT_SHADER_LOWEST:
      scene->shader = (color (*)(void *)) lowest_shader;
      break;
    case RT_SHADER_LOW:
      scene->shader = (color (*)(void *)) low_shader;
      break;
    case RT_SHADER_MEDIUM:
      scene->shader = (color (*)(void *)) medium_shader;
      break;
    case RT_SHADER_HIGH:
      scene->shader = (color (*)(void *)) full_shader;
      break;
    case RT_SHADER_FULL:
      scene->shader = (color (*)(void *)) full_shader;
      break;
    case RT_SHADER_AUTO:
    default:
      scene->shader = NULL;
      break;
  }
}
