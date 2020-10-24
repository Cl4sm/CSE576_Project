{
  /*** WARNING:::  SIZE MUST BE A POWER OF _2_ + 1 ***/
  
  unsigned char *data;
  int spacing = *size = (1 << SC_FRACTAL_DEFAULT_DEPTH);
  
  /*SC_PROFILE_BEGIN("fr")*/

  while(requestedsize > *size + 1) 
    *size = (*size << 1);

  (*size) |= 1;

  data = (unsigned char *)malloc((*size) * (*size));

  if(data == NULL) 
    return(NULL);
  
  _sc_fractal_seeds(data, *size);
  
  while(spacing > 1) 
  {
    _sc_fractal_average(data, *size, spacing);
    spacing >>= 1;
  }
  
  /*SC_PROFILE_END*/
  
  return(data);
}
