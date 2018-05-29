# foodThermometer
Arduino based kitchen thermometer.\
Made with a PT-100 probe on a MAX31865, 4 buttons, piezo alarm and lcd screen\
Remembers last alarm temperature between restarts.

## Modes
Includes cooking temperatures for beef, porc, poultry and candy making.\
Also includes an alarm with a 2 minute until temperature warning.

# Usage
- Button 1 Previous - Button 3 Next : Changes the value on screen (alarm and calibration mode)
- Button 2 Menu : Changes thermometer modes
- Button 4 Cancel: 
  - If alarm disabled
    - Default mode: goes into calibration mode. 
    - Other modes: enable alarm
  - If alarm enabled : Cancels the alarm
  - While setting alarm temperature: Reset alarm temperature to 0

Check and modify the screen and pins setup below and change according to your needs.\
Works as is on Arduino Leonardo
