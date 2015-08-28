#include <stdio.h>
#include <stdlib.h> // exit
#include <string.h>

#include "getdevicefile.h"
#include "util.h"

static char *getKeyboardDeviceFileName(void);

/**
 * Detects and returns the name of the keyboard device file. This function uses
 * the fact that all device information is shown in /proc/bus/input/devices and
 * the keyboard device file should always have an EV of 120013
 *
 * @return the name of the keyboard device file
 */
static char *getKeyboardDeviceFileName() {
   static const char *command =
      "grep -E 'Handlers|EV' /proc/bus/input/devices |"
      "grep -B1 120013 |"
      "grep -Eo event[0-9]+ |"
      "tr '\\n' '\\0'";

   FILE *pipe = popen(command, "r");
   if (pipe == NULL) {
      LOG_ERROR("Could not determine keyboard device file");
   }

   char result[20] = "/dev/input/";
   char temp[9];
   fgets(temp, 9, pipe);

   pclose(pipe);
   return strdup(strcat(result, temp));
}

void getDeviceFile(Config *config) {
   config->deviceFile = getKeyboardDeviceFileName();
}

