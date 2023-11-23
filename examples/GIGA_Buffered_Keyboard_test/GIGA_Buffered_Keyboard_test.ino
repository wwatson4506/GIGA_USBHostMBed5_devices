/*
  USBHost Buffered Keyboard test

  The circuit:
   - Arduino GIGA

  This example code is in the public domain.
*/

#include "USBKeyboard.h"

REDIRECT_STDOUT_TO(Serial)

USBKbdBuffered kbd;

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 5000) {}

  delay(100); // ??????????
  printf("%cStarting Keyboard test...\n",12);

  // Initialize buffered keyboard.
  kbd.USBKeyboardInit();
  // See if it connected.
  if(kbd.connected()) {
    printf("keyboard connected and initialized...\n");
  } else {
	printf("keyboard NOT connected...\n");
    printf("\nAttach USB keyboard if not attached\n\n");
    while(!kbd.connected()) kbd.connect();
    kbd.USBKeyboardInit();
    printf("keyboard connected and initialized...\n");
  }
  printf("End of Setup\n\n");
  printf("Press a key or any combinations of keys.\n");
  printf("Holding a key or keys down will auto repeat\n");
  printf("after a small delay.\n");
  printf("Un-commenting 'delay(100)' in 'loop()' and holding\n");
  printf("a key down for a second or two and then releasing\n");
  printf("it demonstrates key buffering. It will keep printing\n");
  printf("key presses until the buffer is empty (64 bytes).\n");
}

uint8_t m = 0;
uint8_t o = 0;
uint8_t a = 0;

void loop() {
  a = kbd.USBGetkey();
  m = kbd.USBGetModifiers();
  o = kbd.USBGetOemKey();
  printf("Scancode: 0x%2.2x  ",o);
  printf("Modifier: 0x%2.2x  ",m);
  printf("Ascii code: 0x%2.2x  ",a);
  printf("Ascii: %c\n",a);
//delay(100);
}

