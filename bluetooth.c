#include "main.h"

/*******************************************************************************
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

NOTICE:
  This file has moved to our main repository. We had too many issues dealing
  with the submodule linking this project to our main one. You can find the new
  and up-to-date version of this file at the following URL:

  > https://github.com/EastRobotics/2616E/blob/master/In%20The%20Zone/src/2616E_lib/bluetooth.c

-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
*******************************************************************************/

void (*callback)(char *);
FILE *uFile;

// Init a typical hc05 bluetooth module
void hc05Init(char uart, bool atMode) {
  FILE *uFile = uart == 1 ? uart1 : uart2;
  usartInit(uFile, atMode ? 38400 : 9600,
            SERIAL_STOPBITS_1 | SERIAL_PARITY_NONE | SERIAL_DATABITS_8);
}

// Returns a string from the bluetooth module
// uart = 1 || 2
char *bluetoothRead(char uart) {
  FILE *uFile = uart == 1 ? uart1 : uart2;
  char buffer[100];
  return fgets(buffer, 50, uFile);
}

void bprint(char uart, const char *message) {
  FILE *uFile = uart == 1 ? uart1 : uart2;
  fprint(message, uFile); // Send a message to bluetooth
}

void blistenTask(void * ignored) {
  while (true) {
    while (fcount(uFile) < 1) {
      delay(500);
    } // Long delay, don't need to be snappy
    char buffer[100];
    callback(fgets(buffer, 50, uFile));
  }
}

void blisten(char uart, void (*_callback)(char *)) {
  uFile = uart == 1 ? uart1 : uart2;
  callback = _callback;
  taskCreate(blistenTask, TASK_DEFAULT_STACK_SIZE, NULL,
             TASK_PRIORITY_DEFAULT);
}

// Sorry, idk how to pass varargs, so no bprintf atm:
// fprintf(uart1, "formatString", ...); // Send formatted message to bluetooth
