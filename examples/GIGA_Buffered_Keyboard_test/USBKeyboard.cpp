//================
// USBKeyboard.cpp
// 
//================

#include "USBKeyboard.h"

class USBKbdBuffered;
usbKeyMsg_t keyboard_msg;
usbKeyMsg_t *keyboard_msg_p = &keyboard_msg;

elapsedMillis keypresselapsed;	//Setup Key Press Repeat Start Time Interval

// Init timer TIM15
Portenta_H7_Timer ITimer0(TIM15);

// Create a  USBHostKeyboardEx instance 
USBHostKeyboardEx kybd;

//==============================
// Init USB keyboard system.
//==============================
void USBKbdBuffered::USBKeyboardInit(void) {
  init();
}

// Constructor
USBKbdBuffered::USBKbdBuffered() {
//  init();
}

void USBKbdBuffered::init(void) {
  connect();
  kybd.attachPress(USBKbdBuffered::kbd_key_cb);
  kybd.attachRelease(USBKbdBuffered::kbd_key_release_cb);
  keyboard_msg_p->head = 0;
  keyboard_msg_p->tail = 0;
  keyboard_msg_p->keyRepeatDelay = 800; // Default repeat delay
  keyboard_msg_p->keyRepeatRate = 500; // Default repeat rate 
  // HANGS if called from constructor. OK if called from sketch ???????????????????????????
  ITimer0.attachInterruptInterval((uint32_t)(keyboard_msg_p->keyRepeatRate * 100),checkKeyRepeat);
}

void USBKbdBuffered::connect(void) {
    kybd.connect();
}
	
bool USBKbdBuffered::connected(void) {
	return kybd.connected();
}

// Key Press
void USBKbdBuffered::kbd_key_cb(uint8_t key) {
  keypresselapsed = 0;
  keyboard_msg_p->keypressed = 1;
  keyboard_msg_p->keyCode = key;
  keyboard_msg_p->modifiers = kybd.getModifiers();
  keyboard_msg_p->keyOEM = kybd.getOemKey();
  writeBuff();
}

//Key Release
void USBKbdBuffered::kbd_key_release_cb(uint8_t key) {
	keyboard_msg_p->keypressed = 0;
	keyboard_msg_p->keyOEM = 0;
	keyboard_msg_p->last_keyCode = key;
	keyboard_msg_p->keyCode = 0;
	keyboard_msg_p->modifiers = 0;
}

//===============================
// Write key code to buffer and
// scan code to sbuffer.
//===============================
void USBKbdBuffered::writeBuff(void) {
  uint8_t i = 0;

  i = keyboard_msg_p->head + 1;	//Get current buffer pointer
  if(i >= BUFFER_SIZE) i = 0; // Wrap around if buffer is full
  if (i != keyboard_msg_p->tail) {
    keyboard_msg_p->buffer[i] = keyboard_msg_p->keyCode;
    keyboard_msg_p->sbuffer[i] = keyboard_msg_p->keyOEM;
    keyboard_msg_p->head = i;
  }
}

//=======+++++++++++++===========
// Check if keypress is held long
// enough to do auto repeat.
//===============================
void USBKbdBuffered::checkKeyRepeat(void) {
  if((keyboard_msg_p->keypressed == 1) && (keypresselapsed >
      keyboard_msg_p->keyRepeatDelay)) {
     writeBuff();
  } else {
    return;
  }
}

//=============================
// Get a keypress. Wait for it.
//=============================
uint16_t USBKbdBuffered::USBGetkey(void) {
  while(!USBKeyboard_available());
  return USBKeyboard_read();
}

//==================
// Get modifiers.
//==================
uint8_t USBKbdBuffered::USBGetModifiers(void) {
  return keyboard_msg_p->modifiers;
}

//==================
// Get OEM keypress.
//==================
uint8_t USBKbdBuffered::USBGetOemKey(void) {
  return keyboard_msg_p->keyOEM;
}

//===============================
// Read a keypress from buffer.
//===============================
int USBKbdBuffered::USBKeyboard_read(void) {
  uint8_t i = keyboard_msg_p->tail;
  if(i++ == keyboard_msg_p->head) return 0;
  if(i >= BUFFER_SIZE) i = 0;
  keyboard_msg_p->tail = i;
  return keyboard_msg_p->buffer[i];
}

//===============================
// Check key buffer for keypress.
// Return true if available.
//===============================
int USBKbdBuffered::USBKeyboard_available(void) {
  if((keyboard_msg_p->head != keyboard_msg_p->tail) &&
      keyboard_msg_p->buffer[keyboard_msg_p->head]) {
    return true;
  } else {
    return false; }
}

// TODO: implement
void USBKbdBuffered::kbd_hid_key_press_cb(uint32_t top, uint16_t code) {
  Serial.print("Press: HID Key: Page:0x");
//  print_hid_data(top, code);
}

// TODO: implement
void USBKbdBuffered::kbd_hid_key_release_cb(uint32_t top, uint16_t code) {
  Serial.print("Release: HID Key: Page:0x");
//  print_hid_data(top, code);
}
