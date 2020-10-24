void dmix_read(dirichlet_mix *m, FILE *stream) {
  char word[21];  /* allow for terminating NUL */

  int mixture_num = 0;
  int alpha_num = 0;
  double *weights = NULL;
  dirichlet *components = NULL;
  double x;

  while (fscanf(stream, "%20s", word) == 1) {
    if (strcmp(word, "Mixture=") == 0) {
      const int r = fscanf(stream, "%lf", &x);
      if (r != 1)
	die("%s: error reading Dirichlet mixture file: Mixture= ???\n", prog_name);
      if (x <= 0)
	die("%s: error reading Dirichlet mixture file: 'Mixture=' values should be > 0\n", prog_name);
      ++mixture_num;
      weights = XREALLOC(weights, mixture_num);
      weights[mixture_num-1] = x;
    } else if (strcmp(word, "Alpha=") == 0) {
      int dim = 0;
      double *alpha = NULL;
      fscanf(stream, "%lf", &x);  /* 1st number is sum of alphas: ignore */
      while (fscanf(stream, "%lf", &x) == 1) {
	if (x <= 0)
	  die("%s: error reading Dirichlet mixture file: 'Alpha=' values should be > 0\n");
	++dim;
	alpha = XREALLOC(alpha, dim);
	alpha[dim-1] = x;
      }
      if (dim == 0)
	die("%s: error reading Dirichlet mixture file: zero pseudocounts\n", prog_name);
      ++alpha_num;
      components = XREALLOC(components, alpha_num);
      dirichlet_init(&components[alpha_num-1], dim, alpha);
      free(alpha);
      if (dim != components->dim)
	die("%s: error reading Dirichlet mixture file: unequal number of values on 'Alpha=' lines\n", prog_name);
    }
  }
  if (ferror(stream))
    die("%s: error reading Dirichlet mixture file: %s\n", prog_name, strerror(errno));

  if (mixture_num != alpha_num)
    die("%s: error reading Dirichlet mixture file: unequal number of 'Mixture=' and 'Alpha=' lines\n", prog_name);

  if (alpha_num == 0)
    die("%s: error reading Dirichlet mixture file: zero components\n", prog_name);

  /* check that weights sum to 1? */

  m->comp_num = alpha_num;
  m->weights = weights;
  m->components = components;
  dmix_extra_init(m);
}