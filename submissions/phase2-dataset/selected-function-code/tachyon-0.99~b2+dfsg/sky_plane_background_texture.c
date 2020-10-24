color sky_plane_background_texture(const ray *ry) {
  color col;
  flt IdotG = VDot(&ry->o, &ry->scene->bgtex.gradient);
  flt range = ry->scene->bgtex.gradtopval - ry->scene->bgtex.gradbotval;

  flt val = (IdotG - ry->scene->bgtex.gradbotval) / range;

  if (val < 0.0)
    val = 0.0;

  if (val > 1.0)
    val = 1.0;

  col.r = val * ry->scene->bgtex.backgroundtop.r + 
          (1.0 - val) * ry->scene->bgtex.backgroundbot.r;
  col.g = val * ry->scene->bgtex.backgroundtop.g + 
          (1.0 - val) * ry->scene->bgtex.backgroundbot.g;
  col.b = val * ry->scene->bgtex.backgroundtop.b + 
          (1.0 - val) * ry->scene->bgtex.backgroundbot.b;

  return col;
}
