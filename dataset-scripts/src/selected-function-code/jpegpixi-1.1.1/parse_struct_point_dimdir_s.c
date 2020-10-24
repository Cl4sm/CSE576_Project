static struct point_dimdir_s *
parse_struct_point_dimdir_s (struct point_dimdir_s *const point, const char *str,
                             const unsigned int image_width, const unsigned int image_height)
{
    while (isspace ((unsigned char)*str))
        ++str;

    if (*str == 'h' || *str == 'H')
        point->d = horizontal;
    else if (*str == 'v' || *str == 'V')
        point->d = vertical;
    else
    {
        point->d = twodim;
        if (*str != '2' || isdigit ((unsigned char)str [1]) || str [1] == '.' || str [1] == '%'
            || str [1] == ',' || str [1] == '/' || str [1] == ';')
            goto parse_x;
    }
    ++str;

    while (isspace ((unsigned char)*str))
        ++str;

    if (*str == ':')
        ++str;
    else if (point->d == twodim && (*str == ',' || *str == '/' || *str == ';'))
    {
        ++str;
        point->p.x = 2;
        goto parse_y;
    }
    else
        return 0;

    while (isspace ((unsigned char)*str))
        ++str;

  parse_x:
    switch (parse_number (&str, &point->p.x))
    {
      case -1:
        return 0;
      case 0:
        point->p.x = REL_TO_ABS (point->p.x, image_width);
    }

    while (isspace ((unsigned char)*str))
        ++str;

    if (*str == ',' || *str == '/' || *str == ';')
        ++str;
    else
        return 0;

  parse_y:
    while (isspace ((unsigned char)*str))
        ++str;

    switch (parse_number (&str, &point->p.y))
    {
      case -1:
        return 0;
      case 0:
        point->p.y = REL_TO_ABS (point->p.y, image_height);
    }

    while (isspace ((unsigned char)*str))
        ++str;

    if (*str == ',' || *str == '/' || *str == ';')
        ++str;
    else if (*str == '\0')
    {
        point->p.x_size = 1;
        point->p.y_size = 1;
        return point;
    }
    else
        return 0;

    while (isspace ((unsigned char)*str))
        ++str;

    switch (parse_number (&str, &point->p.x_size))
    {
      case -1:
        return 0;
      case 0:
        point->p.x_size = REL_TO_ABS (point->p.x_size, image_width);
    }
    if (point->p.x_size == 0)
        return 0;

    while (isspace ((unsigned char)*str))
        ++str;

    if (*str == ',' || *str == '/' || *str == ';')
        ++str;
    else if (*str == '\0')
    {
        point->p.y_size = point->p.x_size;
        return point;
    }
    else
        return 0;

    while (isspace ((unsigned char)*str))
        ++str;

    switch (parse_number (&str, &point->p.y_size))
    {
      case -1:
        return 0;
      case 0:
        point->p.y_size = REL_TO_ABS (point->p.y_size, image_height);
    }
    if (point->p.y_size == 0)
        return 0;

    while (isspace ((unsigned char)*str))
        ++str;

    return *str == '\0' ? point : 0;
}