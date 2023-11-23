//=============================
// USBKeyboard.h
// By: Warren Watson 2017-2023
//=============================
#ifndef USBKEYBOARD_H__
#define USBKEYBOARD_H__

#include <Arduino_USBHostMbed5.h>
#include "USBHostKeyboardEx.h"
#include <LibPrintf.h>
#include "elapsedMillis.h"
#include "Portenta_H7_TimerInterrupt.h"

#define BUFFER_SIZE 64

//==============================
// Typedef USB keyboard struct.
//==============================
typedef struct usbKeyMsg_struct  usbKeyMsg_t;

//==============================
// USB keyboard struct.
//==============================
struct usbKeyMsg_struct {
  volatile int buffer[BUFFER_SIZE];
  volatile int sbuffer[BUFFER_SIZE];
  volatile uint8_t head = 0;
  volatile uint8_t tail = 0;
  uint8_t keypressed;
  uint8_t keyCode;
  uint8_t modifiers;
  uint8_t keyOEM;
  uint16_t KeyHID;
  uint16_t last_keyCode;
  uint32_t keyRepeatDelay;
  uint32_t keyRepeatRate;
};

class USBKbdBuffered : public USBHostKeyboardEx {
public:
  
  USBKbdBuffered();

  void init();
  bool connected(void);
  void connect(void);
  void USBKeyboardInit(void);
  uint16_t USBGetkey(void);
  uint8_t USBGetModifiers(void);
  uint8_t USBGetOemKey(void);
  int USBKeyboard_read(void);
  int USBKeyboard_available(void);
  void kbd_hid_key_press_cb(uint32_t top, uint16_t code);
  void kbd_hid_key_release_cb(uint32_t top, uint16_t code);

protected:

private:

  static void kbd_key_cb(uint8_t key);
  static void kbd_key_release_cb(uint8_t key);
  static void writeBuff(void);
  static void checkKeyRepeat(void);

};
#endif
