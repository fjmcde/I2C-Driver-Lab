/***************************************************************************//**
 * @file
 *   app.c
 * @author
 *   Frank McDermott
 * @date
 *   10/04/2022
 * @brief
 *   Driver to configure application-specific peripherals
 ******************************************************************************/

//***********************************************************************************
// Include files
//***********************************************************************************
#include "app.h"


//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// Static / Private Variables
//***********************************************************************************


//***********************************************************************************
// Private functions
//***********************************************************************************

static void app_letimer_pwm_open(float period, float act_period, uint32_t out0_route, uint32_t out1_route);

//***********************************************************************************
// Global functions
//***********************************************************************************

/***************************************************************************//**
 * @brief
 *   Sets up the application-specific peripherals, schedulers, and timers
 *
 * @details
 *   Opens the CMU, GPIO, interrupt scheduler, and application specific LETIMER
 *   in PWM mode, then starts the LETIMER to begin counting.
 ******************************************************************************/
void app_peripheral_setup(void){
  cmu_open();
  gpio_open();
  scheduler_open();
  sleep_open();
  app_letimer_pwm_open(PWM_PER, PWM_ACT_PER, PWM_ROUTE_0, PWM_ROUTE_1);
  letimer_start(LETIMER0, true);
}



/***************************************************************************//**
 * @brief
 *   Configure LETIMER for PWM mode.
 *
 * @details
 *   Driver which instantiates and opens the LETIMER in PWM mode for the application
 *
 * @param[in] period
 *   Sets the period (in seconds) for the clock
 *
 * @param[in] act_period
 *   Sets the active period (in seconds) for the clock
 *
 * @param[in] out0_route
 *    out0 route to gpio port/pin
 *
 * @param[in] out1_route
 *    out1 route to gpio port/pin
 *
 ******************************************************************************/
void app_letimer_pwm_open(float period, float act_period, uint32_t out0_route, uint32_t out1_route)
{
  // instantiate an APP_LETIMER_PWM_TypeDef struct
  APP_LETIMER_PWM_TypeDef letimer_pwm;

  // configure struct
  letimer_pwm.debugRun = false;
  letimer_pwm.enable = false;
  letimer_pwm.out_pin_route0 = out0_route;
  letimer_pwm.out_pin_route1 = out1_route;
  letimer_pwm.out_pin_0_en = true;
  letimer_pwm.out_pin_1_en = false;
  letimer_pwm.period = period;
  letimer_pwm.active_period = act_period;
  letimer_pwm.comp0_irq_enable = false;
  letimer_pwm.comp0_cb = LETIMER0_COMP0_CB;
  letimer_pwm.comp1_irq_enable = true;
  letimer_pwm.comp1_cb = LETIMER0_COMP1_CB;
  letimer_pwm.uf_irq_enable = true;
  letimer_pwm.uf_cb = LETIMER0_UF_CB;

  // open letimer for PWM mode
  letimer_pwm_open(LETIMER0, &letimer_pwm);
}


//***********************************************************************************
// app scheduler service routines
//***********************************************************************************

/***************************************************************************//**
 * @brief
 *   Handles the scheduling of the letimer0 underflow call back
 *
 * @details
 *   Removes the underflow call back event from the scheduler then
 *   asserts that the event has been cleared
 ******************************************************************************/
void scheduled_letimer0_uf_cb(void)
{
  remove_scheduled_event(LETIMER0_UF_CB);
  EFM_ASSERT(!(get_scheduled_events() & LETIMER0_UF_CB));
}

/***************************************************************************//**
 * @brief
 *   Handles the scheduling of the letimer0 comp0 call back
 *
 * @note
 *   Currently disabled, as the comp0 interrupt is disabled. The Assert
 *   statement is for debugging. If the assert is reached the configuration
 *   is incorrect.
 *
 * @details
 *   Removes the comp0 call back event from the scheduler then asserts that
 *   the event has been cleared
 ******************************************************************************/
void scheduled_letimer0_comp0_cb(void)
{
  // the LETIMER0 comp0 interrupt is disabled; so if we get here we know that
  // we configured the LETIMER0 incorrectly. FOR DEBUGGING PURPOSES ONLY.
  // REMOVE ASSERT IF INTERRUPT ENABLED.
  EFM_ASSERT(false);

  // added so that we can handle the callback once enabled
  // uncomment next two lines when enabled
  //remove_scheduled_event(LETIMER0_COMP0_CB);
  //EFM_ASSERT(!(get_scheduled_events() & LETIMER0_COMP0_CB));
}


/***************************************************************************//**
 * @brief
 *   Handles the scheduling of the letimer0 comp1 call back
 *
 * @details
 *   Removes the comp1 call back event from the scheduler then
 *   asserts that the event has been cleared
 ******************************************************************************/
void scheduled_letimer0_comp1_cb(void)
{
  remove_scheduled_event(LETIMER0_COMP1_CB);
  EFM_ASSERT(!(get_scheduled_events() & LETIMER0_COMP1_CB));
}


/***************************************************************************//**
 * @brief
 *   Handles the scheduling of the GPIO Odd IRQ (BTN1) call back
 *
 * @details
 *  Removes the triggering event from the scheduler and unblocked the
 *  current energy mode. If the current blocked EM is less than EM4,
 *  increment the current energy mode. Otherwise return EM0 (overflow)
 ******************************************************************************/
void scheduled_gpio_odd_irq_cb(void)
{
  // remove scheduled event from scheduler
  remove_scheduled_event(GPIO_ODD_IRQ_CB);

  // assert to ensure removed
  EFM_ASSERT(!(get_scheduled_events() & GPIO_ODD_IRQ_CB));

  // local variable to store the current blocked energy mode
  uint32_t current_block_em = current_block_energy_mode();

  // unblock current energy mode
  sleep_unblock_mode(current_block_em);

  if(current_block_em < EM4)
  {
    sleep_block_mode(++current_block_em);
  }
  else
  {
      sleep_block_mode(EM0);
  }
}


/***************************************************************************//**
 * @brief
 *   Handles the scheduling of the GPIO Even IRQ (BTN1) call back
 *
 * @details
 *  Removes the triggering event from the scheduler. If the EM is zero,
 *  block EM4 (underflow) otherwise  decrement the current block energy mode.
 ******************************************************************************/
void scheduled_gpio_even_irq_cb(void)
{
  // remove event from scheduler
  remove_scheduled_event(GPIO_EVEN_IRQ_CB);

  // assert to ensure removed
  EFM_ASSERT(!(get_scheduled_events() & GPIO_EVEN_IRQ_CB));

  // local variable to store the current blocked energy mode
  uint32_t current_block_em = current_block_energy_mode();

  // unblock current energy mode
  sleep_unblock_mode(current_block_em);

  if(current_block_em > EM0)
  {
      sleep_block_mode(--current_block_em);
  }
  else
  {
      sleep_block_mode(EM4);
  }
}
