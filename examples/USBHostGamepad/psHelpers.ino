uint16_t xc, yc; 
uint8_t isTouch;
int16_t xc_old, yc_old;

// PS3 angles
void getAnglesPS3(float * angles) {

      float accXval, accYval, accZval;

      // Data for the Kionix KXPC4 used in the DualShock 3
      const float zeroG = 511.5f; // 1.65/3.3*1023 (1,65V)
      accXval = -((float)((gamepad.getAxis(50 - 9) << 8) | gamepad.getAxis(51 - 9)) - zeroG);
      accYval = -((float)((gamepad.getAxis(52 - 9) << 8) | gamepad.getAxis(53 - 9)) - zeroG);
      accZval = -((float)((gamepad.getAxis(54 - 9) << 8) | gamepad.getAxis(55 - 9)) - zeroG);
      //printf("\tax: %f, ay: %f, az: %f\n", accXval, accYval, accZval);
      angles[0] =  ((atan2f(accYval, accZval) + PI) * RAD_TO_DEG);
      angles[1]  = ((atan2f(accXval, accZval) + PI) * RAD_TO_DEG);

}

//ps4
void printAngles(){

  getCoords(xc, yc, isTouch);
  printf("Touch: %d, %d, %d, %d\n", psAxis[33], isTouch, xc, yc);
}

void getCoords(uint16_t &xc, uint16_t &yc, uint8_t &isTouch){
  // Trackpad touch 1: id, active, x, y
  xc = ((psAxis[37] & 0x0f) << 8) | psAxis[36];
  yc = psAxis[38] << 4 | ((psAxis[37] & 0xf0) >> 4),

  isTouch = psAxis[35] >> 7;
  if(xc != xc_old || yc != yc_old){
    //printf("Touch: %d, %d, %d, %d\n", psAxis[33], isTouch, xc, yc);
    xc_old = xc;
    yc_old = yc;
  }
}