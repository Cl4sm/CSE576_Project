void * rt_init_scanlinereceives(scenedef * scene) {
#ifdef MPI
  int i, addr;
  pardata * p;

  p = (pardata *) rt_allocate_reqbuf(scene->vres);

  p->curmsg = 0;
  p->count = 0;
  p->haveinited = 1;

  if (scene->imgbufformat == RT_IMAGE_BUFFER_RGB24) {
    /* 24-bit RGB packed pixel format */
    unsigned char *imgbuf = (unsigned char *) scene->img;

    if (p->mynode == 0) {
      for (i=0; i<scene->vres; i++) {
        if (i % p->nodes != p->mynode) {
          addr = i * scene->hres * 3;
          MPI_Recv_init(&imgbuf[addr], scene->hres * 3, MPI_BYTE, 
                    i % p->nodes, i+1, MPI_COMM_WORLD, &p->requests[p->count]);
          p->count++;
        }
      }
    } else {
      for (i=0; i<scene->vres; i++) {
        if (i % p->nodes == p->mynode) {
          addr = i * scene->hres * 3;
          MPI_Send_init(&imgbuf[addr], scene->hres * 3, MPI_BYTE, 
                    0, i+1, MPI_COMM_WORLD, &p->requests[p->count]);
          p->count++;
        }
      }
    }
  } else if (scene->imgbufformat == RT_IMAGE_BUFFER_RGB96F) {
    /* 96-bit float RGB packed pixel format */
    float *imgbuf = (float *) scene->img;

    if (p->mynode == 0) {
      for (i=0; i<scene->vres; i++) {
        if (i % p->nodes != p->mynode) {
          addr = i * scene->hres * 3;
          MPI_Recv_init(&imgbuf[addr], scene->hres * 3, MPI_FLOAT, 
                    i % p->nodes, i+1, MPI_COMM_WORLD, &p->requests[p->count]);
          p->count++;
        }
      }
    } else {
      for (i=0; i<scene->vres; i++) {
        if (i % p->nodes == p->mynode) {
          addr = i * scene->hres * 3;
          MPI_Send_init(&imgbuf[addr], scene->hres * 3, MPI_FLOAT,
                    0, i+1, MPI_COMM_WORLD, &p->requests[p->count]);
          p->count++;
        }
      }
    }
  }
 
  return p;
#else
  return NULL;
#endif
}
