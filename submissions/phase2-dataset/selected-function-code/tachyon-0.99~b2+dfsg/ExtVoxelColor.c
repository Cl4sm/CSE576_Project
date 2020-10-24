color ExtVoxelColor(flt scalar) {
  color col;

  if (scalar > 1.0) 
    scalar = 1.0;

  if (scalar < 0.0)
    scalar = 0.0;

  if (scalar < 0.5) {
    col.g = 0.0;
  }
  else {
    col.g = (scalar - 0.5) * 2.0;
  }

  col.r = scalar;
  col.b = 1.0 - (scalar / 2.0);

  return col;
} 
