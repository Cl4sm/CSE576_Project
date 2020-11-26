#ifndef CNFH

#define mac_addr __SIZE_TYPE__

#define CFG_CLASS_STR 1
#define CFG_CLASS_NUM 2

#define is_numeric(X) (X->type & CFG_CLASS_NUM)

#ifndef CNFDBG
#define cnfdbg(format, ...) fake_cnfdbg(format,##__VA_ARGS__)
#endif
#ifdef BSD
#include <stdio.h>
int getline(char **line, size_t *len, FILE *fp);
#endif

struct cnf_config_class {
	char ident;
	char type;
	char delimiter;
	int (*sanity) (void *data);
};

struct cnf_config_meta {
	char *name;
	char *classes;
	int ident_arg;
	int (*done) (void *data);
};

struct cnf_config_list {
	struct cnf_config_meta *meta;
	void *data;
	struct cnf_config_list *next;
};

struct cnf_config {
	char *delimiter;
	int num_meta;
	int num_classes;
	struct cnf_config_meta *metas;
	struct cnf_config_class *classes;
};

int cnf_parse_line(char *line, struct cnf_config *cfg);
void cnf_parse(char *fname, struct cnf_config *cfg, struct cnf_config_list **cfgl);
void cnf_free_list(struct cnf_config_list *item, struct cnf_config *cfg);


#endif
