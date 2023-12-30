/*
 * pins.h
 *
 *  Created on: Dec 29, 2023
 *      Author: jjlange
 */

#ifndef PINS_H_
#define PINS_H_

#define LED_1_PORT                2
#define LED_1_PORT_ADDR
#define LED_1_PIN                 4

#define LED_2_PORT                4
#define LED_2_PIN                 5

#define LED_3_PORT                4
#define LED_3_PIN                 2

#define LED_4_PORT                0
#define LED_4_PIN                 0

#define MODE_BTN_PORT             1
#define MODE_BTN_PIN              0

//These should be configured as analog...
#define PS2_5V_SENSE_PORT         1
#define PS2_5V_SENSE_PIN          3

#define ADB_5V_SENSE_PORT         0
#define ADB_5V_SENSE_PIN          1

#define ENABLE_PS2_PULLUP_PORT    0
#define ENABLE_PS2_PULLUP_PIN     4

#define ENABLE_ADB_PULLUP_PORT    3
#define ENABLE_ADB_PULLUP_PIN     6

#define ENABLE_PS2_POWER_OUT_PORT 4
#define ENABLE_PS2_POWER_OUT_PIN  4

#define ENABLE_ADB_POWER_OUT_PORT 3
#define ENABLE_ADB_POWER_OUT_PIN  7

#define PS2_KBD_DATA_PORT         3
#define PS2_KBD_DATA_PIN          1

#define PS2_KBD_CLK_PORT          3
#define PS2_KBD_CLK_PIN           2

#define PS2_MOUSE_DATA_PORT       3
#define PS2_MOUSE_DATA_PIN        4

#define PS2_MOUSE_CLK_PORT        3
#define PS2_MOUSE_CLK_PIN         3

#define ADB_POWERON_PORT          3
#define ADB_POWERON_PIN           5

#define ADB_OUT1_PORT             2
#define ADB_OUT1_PIN              7

#define ADB_OUT2_PORT             2
#define ADB_OUT2_PIN              1

#define ADB_IN1_PORT              2
#define ADB_IN1_PIN               6

#define ADB_IN2_PORT              2
#define ADB_IN2_PIN               5

#define ADB_IN3_PORT              2
#define ADB_IN3_PIN               2

#define EXTRA_KBD1_PORT           0
#define EXTRA_KBD1_PIN            6

#define EXTRA_KBD2_PORT           0
#define EXTRA_KBD2_PIN            7

#define EXTRA_MOUSE1_PORT         4
#define EXTRA_MOUSE1_PIN          1

#define EXTRA_MOUSE2_PORT         4
#define EXTRA_MOUSE2_PIN          0

#define EXTRA_MOUSE3_PORT         2
#define EXTRA_MOUSE3_PIN          0

#define EXTRA_MOUSE4_PORT         4
#define EXTRA_MOUSE4_PIN          3

#define EXTRA_MOUSE5_PORT         0
#define EXTRA_MOUSE5_PIN          5

#define EXTRA_MOUSE6_PORT         3
#define EXTRA_MOUSE6_PIN          0

#endif /* PINS_H_ */
