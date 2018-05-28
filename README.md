# foodThermometer
Arduino based kitchen thermometer.\
Made with a PT-100 probe on a MAX31865, 4 buttons, piezo alarm and lcd screen

## Modes
Includes cooking temperatures for beef, porc, poultry and candy making.\
Also includes an alarm with a 2 minute until temperature warning.

# Usage
- Button 1 Previous - Button 3 Next : Changes the value on screen (alarm and calibration mode)
- Button 2 Menu : Changes thermometer modes
- Button 4 Cancel: On default mode without alarm, goes into calibration mode. In any mode, with an alarm, cancels the alarm

Check and modify the screen and pins setup below and change according to your needs.\
Works as is on Arduino Leonardo
