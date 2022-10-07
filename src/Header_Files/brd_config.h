#ifndef BRD_CONFIG_HG
#define BRD_CONFIG_HG

//***********************************************************************************
// Include files
//***********************************************************************************
/* System include statements */


/* Silicon Labs include statements */
#include "em_cmu.h"
#include "em_gpio.h"


//***********************************************************************************
// defined files
//***********************************************************************************

// button configuration
#define BUTTON_0_PORT           gpioPortF       // PF6; User's Guide: 6.1
#define BUTTON_0_PIN            06u             // user's Guide: 6.1
#define BUTTON_0_CONFIG         gpioModeInput   // input enabled; filter if DOUT is set
#define BUTTON_1_PORT           gpioPortF       // PF7; User's Guide: 6.1
#define BUTTON_1_PIN            07u             // user's Guide: 6.1
#define BUTTON_1_CONFIG         gpioModeInput   // input enabled; filter if DOUT is set.
#define BUTTON_DEFAULT          true            // input filter enabled

// button interrupt configuration
#define BUTTON_0_INT_NUM        BUTTON_0_PIN    // pin for button interrupt
#define BUTTON_0_INT_RISING     false           // false: don't trigger interrupt on rising edge
#define BUTTON_0_INT_FALLING    true            // true: trigger interrupt on falling edge
#define BUTTON_0_INT_ENABLE     true            // enable interrupt on button 0
#define BUTTON_1_INT_NUM        BUTTON_1_PIN    // pin for button interrupt
#define BUTTON_1_INT_RISING     false           // false: don't trigger interrupt on rising edge
#define BUTTON_1_INT_FALLING    true            // true: trigger interrupt on falling edge
#define BUTTON_1_INT_ENABLE     true            // enable interrupt on button 1

// GPIO pin setup
#define STRONG_DRIVE

// LED 0 pin is
#define LED0_PORT         gpioPortF         // Port F
#define LED0_PIN          04u               // Pin 4
#define LED0_DEFAULT      false             // Default false (0) = off, true (1) = on
#define LED0_GPIOMODE     gpioModePushPull  // Push -pull mode

// LED 1 pin is
#define LED1_PORT         gpioPortF         // Port F
#define LED1_PIN          05u               // Pin 5
#define LED1_DEFAULT      false             // Default false (0) = off, true (1) = on
#define LED1_GPIOMODE     gpioModePushPull  // Push-pull mode

#ifdef STRONG_DRIVE
  #define LED0_DRIVE_STRENGTH   gpioDriveStrengthStrongAlternateStrong
  #define LED1_DRIVE_STRENGTH   gpioDriveStrengthStrongAlternateStrong
#else
  #define LED0_DRIVE_STRENGTH   gpioDriveStrengthWeakAlternateWeak
  #define LED1_DRIVE_STRENGTH   gpioDriveStrengthWeakAlternateWeak
#endif


// System Clock setup
#define MCU_HFXO_FREQ     cmuHFRCOFreq_32M0Hz   // Configure HFRC to 32MHz


// LETIMER PWM Configuration
#define   PWM_ROUTE_0     28  // PWM route location value
#define   PWM_ROUTE_1     29  // PWM route location value

//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************

#endif
