static void rt_sheightfield(SceneHandle scene, void * tex, apivector ctr, int m, int n, 
                    flt * field, flt wx, flt wy) {
  apivector * vertices;
  apivector * normals;
  apivector offset;
  flt xinc, yinc;
  int x, y, addr; 
   
  vertices = (apivector *) malloc(m*n*sizeof(apivector));
  normals = (apivector *) malloc(m*n*sizeof(apivector));

  offset.x = ctr.x - (wx / 2.0);
  offset.y = ctr.z - (wy / 2.0);
  offset.z = ctr.y;

  xinc = wx / ((flt) m-1);
  yinc = wy / ((flt) n-1);

  /* build vertex list */
  for (y=0; y<n; y++) { 
    for (x=0; x<m; x++) {
      addr = y*m + x;
      vertices[addr] = rt_vector(
        x * xinc + offset.x,
        field[addr] + offset.z,
        y * yinc + offset.y);
    }
  }

  /* build normals from vertex list */
  for (x=1; x<m; x++) {
    normals[x] = normals[(n - 1)*m + x] = rt_vector(0.0, 1.0, 0.0);
  }
  for (y=1; y<n; y++) {
    normals[y*m] = normals[y*m + (m-1)] = rt_vector(0.0, 1.0, 0.0);
  }
  for (y=1; y<(n-1); y++) {
    for (x=1; x<(m-1); x++) {
      addr = y*m + x;

      normals[addr] = rt_vector(
        -(field[addr + 1] - field[addr - 1]) / (2.0 * xinc), 
        1.0, 
        -(field[addr + m] - field[addr - m]) / (2.0 * yinc));

      VNorm(&normals[addr]);
    }
  }    

  /* generate actual triangles */
  for (y=0; y<(n-1); y++) {
    for (x=0; x<(m-1); x++) {
      addr = y*m + x;

      rt_stri(scene, tex, vertices[addr], vertices[addr + 1 + m], vertices[addr + 1],
                   normals[addr], normals[addr + 1 + m], normals[addr + 1]);
      rt_stri(scene, tex, vertices[addr], vertices[addr + m], vertices[addr + 1 + m],
                   normals[addr], normals[addr + m], normals[addr + 1 + m]);
    }
  }

  free(normals);
  free(vertices);
} /* end of smoothed heightfield */
