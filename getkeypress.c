#include <Python.h>
#include <fcntl.h>   // open
#include <stdint.h>
#include <linux/input.h>

#include "key_util.h"
#include "util.h"
#include "getdevicefile.h"
#include "config.h"

#define KEY_RELEASE 0
#define KEY_PRESS 1

typedef struct input_event input_event;

static void rootCheck(void);
static int openKeyboardDeviceFile(char *deviceFile);
static PyObject * getkeypress(PyObject * self, PyObject *args);

/**
 * Exit with return code -1 if user does not have root privileges
 */
static void rootCheck() {
   if (geteuid() != 0) {
      printf("Must run as root\n");
      exit(-1);
   }
}

/**
 * Opens the keyboard device file
 *
 * @param  deviceFile   the path to the keyboard device file
 * @return              the file descriptor on success, error code on failure
 */
static int openKeyboardDeviceFile(char *deviceFile) {
   int kbd_fd = open(deviceFile, O_RDONLY);
   if (kbd_fd == -1) {
      LOG_ERROR("%s", strerror(errno));
      exit(-1);
   }

   return kbd_fd;
}

/**
 * Gets a raw key press from user and converts it to a string.
 *
 * @param  self         Pointer to self.
 *         args         The arguments passed from python program, must
 *                      be parsed by python's parse tuple function.
 * @return              PyObject version of string.
 */
static PyObject * getkeypress(PyObject * self, PyObject *args) {
   char * returnString = "";

   rootCheck();

   Config config;
   getDeviceFile(&config);

   int kbd_fd = openKeyboardDeviceFile(config.deviceFile);
   assert(kbd_fd > 0);

   // Daemonize process. Don't change working directory but redirect standard
   // inputs and outputs to /dev/null
//   if (daemon(1, 0) == -1) {
//      LOG_ERROR("%s", strerror(errno));
//      exit(-1);
//   }

   uint8_t shift_pressed = 0;
   input_event event;
   while (read(kbd_fd, &event, sizeof(input_event)) > 0) {
      if (event.type == EV_KEY) {
         if (event.value == KEY_PRESS) {
            char *name = getKeyText(event.code, shift_pressed);
            if (isShift(event.code)) {
               shift_pressed++;
            }
            else if (strcmp(name, UNKNOWN_KEY) != 0) {
               //LOG("%s", name);
               returnString = strdup(name);
               break;

            }
         } else if (event.value == KEY_RELEASE) {
            if (isShift(event.code)) {
               shift_pressed--;
            }
         }
      }
      assert(shift_pressed >= 0 && shift_pressed <= 2);
   }

   Config_cleanup(&config);
   close(kbd_fd);
   return Py_BuildValue("s", returnString);
}


/*
 * Python module information
 *
 * */

static PyMethodDef KeyPressedMethods[] = {
  {"getkeypress",  getkeypress, METH_VARARGS,
    "Get a single key pressed in the form of a string."},
  {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
initgetkeypress(void)
{
  (void) Py_InitModule("getkeypress", KeyPressedMethods);
}
/*
 * End python module information
 *
 * */
