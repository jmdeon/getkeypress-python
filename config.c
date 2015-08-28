#include <stdlib.h>

#include "config.h"

void Config_cleanup(Config *config) {
	free(config->deviceFile);
	config->deviceFile = NULL;
}
