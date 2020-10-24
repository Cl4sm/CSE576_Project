  /* -XY face */
  rt_tri(scene, tex, rt_vector(min.x, min.y, min.z),
                     rt_vector(min.x, max.y, min.z), 
                     rt_vector(max.x, max.y, min.z));
  rt_tri(scene, tex, rt_vector(min.x, min.y, min.z),
                     rt_vector(max.x, max.y, min.z), 
                     rt_vector(max.x, min.y, min.z));

  /* +XY face */
  rt_tri(scene, tex, rt_vector(min.x, min.y, max.z),
                     rt_vector(max.x, max.y, max.z),
                     rt_vector(min.x, max.y, max.z)); 
  rt_tri(scene, tex, rt_vector(min.x, min.y, max.z),
                     rt_vector(max.x, min.y, max.z),
                     rt_vector(max.x, max.y, max.z)); 

  /* -YZ face */
  rt_tri(scene, tex, rt_vector(min.x, min.y, min.z),
                     rt_vector(min.x, max.y, max.z),
                     rt_vector(min.x, min.y, max.z)); 
  rt_tri(scene, tex, rt_vector(min.x, min.y, min.z),
                     rt_vector(min.x, max.y, min.z),
                     rt_vector(min.x, max.y, max.z)); 

  /* +YZ face */
  rt_tri(scene, tex, rt_vector(max.x, min.y, min.z),
                     rt_vector(max.x, min.y, max.z),
                     rt_vector(max.x, max.y, max.z));
  rt_tri(scene, tex, rt_vector(max.x, min.y, min.z),
                     rt_vector(max.x, max.y, max.z),
                     rt_vector(max.x, max.y, min.z));

  /* -XZ face */
  rt_tri(scene, tex, rt_vector(min.x, min.y, min.z),
                     rt_vector(min.x, min.y, max.z), 
                     rt_vector(max.x, min.y, max.z));
  rt_tri(scene, tex, rt_vector(min.x, min.y, min.z),
                     rt_vector(max.x, min.y, max.z), 
                     rt_vector(max.x, min.y, min.z));

  /* +XZ face */
  rt_tri(scene, tex, rt_vector(min.x, max.y, min.z),
                     rt_vector(max.x, max.y, max.z),
                     rt_vector(min.x, max.y, max.z)); 
  rt_tri(scene, tex, rt_vector(min.x, max.y, min.z),
                     rt_vector(max.x, max.y, min.z),
                     rt_vector(max.x, max.y, max.z)); 
}
