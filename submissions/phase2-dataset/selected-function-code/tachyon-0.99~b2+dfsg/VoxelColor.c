color VoxelColor(flt scalar) {
  color col;

  if (scalar > 1.0) 
    scalar = 1.0;

  if (scalar < 0.0)
    scalar = 0.0;

  if (scalar < 0.25) {
    col.r = scalar * 4.0;
    col.g = 0.0;
    col.b = 0.0;
  }
  else {
    if (scalar < 0.75) {
      col.r = 1.0;
      col.g = (scalar - 0.25) * 2.0;
      col.b = 0.0;
    }
    else {
      col.r = 1.0;
      col.g = 1.0;
      col.b = (scalar - 0.75) * 4.0;
    }
  }

  return col;
} 
