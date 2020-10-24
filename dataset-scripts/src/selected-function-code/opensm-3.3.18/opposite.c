static inline int opposite(switch_t *s, int axis)
{
	unsigned i, j;
	int negaxis = 1 + (1 ^ (axis - 1));

	if (!s->node->matrix)
		return 0;

	for (i = 0; i < s->node->num_links; i++) {
		if (s->node->axes[i] == axis) {
			for (j = 0; j < s->node->num_links; j++) {
				if (j == i)
					continue;
				if (s->node->matrix[i][j] != 2)
					return negaxis;
			}

			return axis;
		}
	}

	return 0;
}