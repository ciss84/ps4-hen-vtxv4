#ifndef CONFIG_H_
#define CONFIG_H_

#include "config_struct.h"

#define DEFAULT_CONFIG_VERSION 3

void upload_ini(const char *path);
int init_config(struct configuration *config);

#endif
