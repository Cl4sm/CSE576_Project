                    vector upvec) {
  vector newupvec;
  vector newviewvec;
  vector newrightvec;

  /* recompute the camera vectors */
  VCross(&upvec, &viewvec, &newrightvec);
  VNorm(&newrightvec);

  VCross(&viewvec, &newrightvec, &newupvec);
  VNorm(&newupvec);

  newviewvec=viewvec;
  VNorm(&newviewvec);

  camera->center=center;
  camera->viewvec=newviewvec;
  camera->rightvec=newrightvec;
  camera->upvec=newupvec;
}
