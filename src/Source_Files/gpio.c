/***************************************************************************//**
 * @file
 *   gpio.c
 * @author
 *   Frank McDermott
 * @date
 *   9/18/2022
 * @brief
 *   GPIO source file for driving the output to LED0 and LED1
 ******************************************************************************/

//***********************************************************************************
// Include files
//***********************************************************************************
#include "gpio.h"


//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// static/private variables
//***********************************************************************************
static uint32_t gpio_odd_irq_cb;
static uint32_t gpio_even_irq_cb;


//***********************************************************************************
// Private functions
//***********************************************************************************


//***********************************************************************************
// Global functions
//***********************************************************************************


/***************************************************************************//**
 * @brief
 *   Driver to open the GPIO peripheral
 *
 * @details
 *   Enables the GPIO for use with the buttons and LEDs. Enables GPIO clock,
 *   clears all interrupt flags, enables interrupts and enable the NVIC for
 *   the peripheral.
 ******************************************************************************/
void gpio_open(void){

  // enable clock
  CMU_ClockEnable(cmuClock_GPIO, true);

  // configure LEDs
  GPIO_DriveStrengthSet(LED0_PORT, LED0_DRIVE_STRENGTH);
  GPIO_PinModeSet(LED0_PORT, LED0_PIN, LED0_GPIOMODE, LED0_DEFAULT);
  GPIO_DriveStrengthSet(LED1_PORT, LED1_DRIVE_STRENGTH);
  GPIO_PinModeSet(LED1_PORT, LED1_PIN, LED1_GPIOMODE, LED1_DEFAULT);

  // configure buttons
  GPIO_PinModeSet(BUTTON_0_PORT, BUTTON_0_PIN, BUTTON_0_CONFIG, BUTTON_DEFAULT);
  GPIO_PinModeSet(BUTTON_1_PORT, BUTTON_1_PIN, BUTTON_1_CONFIG, BUTTON_DEFAULT);

  // clear interrupt flags
  GPIO->IFC &= ~(_GPIO_IFC_RESETVALUE);

  // configure button interrupts on falling edge of the clock
  GPIO_ExtIntConfig(BUTTON_0_PORT, BUTTON_0_PIN, BUTTON_0_INT_NUM,
                    BUTTON_0_INT_RISING, BUTTON_0_INT_FALLING, BUTTON_0_INT_ENABLE);
  GPIO_ExtIntConfig(BUTTON_1_PORT, BUTTON_1_PIN, BUTTON_1_INT_NUM,
                    BUTTON_1_INT_RISING, BUTTON_1_INT_FALLING, BUTTON_1_INT_ENABLE);

  // enable NVIC button interrupts
  NVIC_EnableIRQ(GPIO_ODD_IRQn);  // button 1
  NVIC_EnableIRQ(GPIO_EVEN_IRQn); // button 0

  // assign call back values
  gpio_odd_irq_cb = GPIO_ODD_IRQ_CB;   // button 1
  gpio_even_irq_cb = GPIO_EVEN_IRQ_CB; // button 0
}


/***************************************************************************//**
 * @brief
 *   Driver to handle the GPIO Odd Interrupts on BTN1
 *
 * @details
 *   Adds a callback event to the schedule for BTN1 interrupt
 ******************************************************************************/
void GPIO_ODD_IRQHandler(void)
{
  uint32_t int_flag;
  int_flag = (GPIO->IF & GPIO->IEN);

  // clear interrupt flag
  GPIO->IFC = int_flag;

  // schedule call back event
  add_scheduled_event(gpio_odd_irq_cb);

  // assert to ensure flag is cleared
  EFM_ASSERT(!(GPIO->IF & gpio_odd_irq_cb));
}


/***************************************************************************//**
 * @brief
 *   Driver to handle the GPIO Even Interrupts on BTN0
 *
 * @details
 *   Adds a callback event to the schedule for BTN0 interrupt
 ******************************************************************************/
void GPIO_EVEN_IRQHandler(void)
{
  uint32_t int_flag;
  int_flag = (GPIO->IF & GPIO->IEN);

  // clear interrupt flag
  GPIO->IFC = int_flag;

  // schedule call back event
  add_scheduled_event(gpio_even_irq_cb);

  // assert to ensure flag is cleared
  EFM_ASSERT(!(GPIO->IF & gpio_even_irq_cb));
}
