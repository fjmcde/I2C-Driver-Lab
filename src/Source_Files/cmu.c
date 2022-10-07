/***************************************************************************//**
 * @file
 *   cmu.c
 * @author
 *   Frank McDermott
 * @date
 *   9/18/2022
 * @brief
 *   Driver for enabling the required oscillators and routing the
 *   clock tree to the LETIMER0.
 ******************************************************************************/

//***********************************************************************************
// Include files
//***********************************************************************************
#include "cmu.h"

//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// Private variables
//***********************************************************************************


//***********************************************************************************
// Private functions
//***********************************************************************************


//***********************************************************************************
// Global functions
//***********************************************************************************


/***************************************************************************//**
 * @brief
 *   Routes clock tree to LETIMER0 and enables the proper oscillators
 *
 * @details
 *   Enabled:
 *   - Low-energy clock divided down from HFCLK (CORELE)
 *
 *   Disabled:
 *   - High-frequency peripheral clock (HFPER)
 *   - Low Frequency Resistor Capacitor Oscillator (LFRCO)
 *   - Low Frequency Crystal Oscillator (LFXO)
 *
 *   Selected:
 *   - Ultra low-frequency RC oscillator (ULFRCO)
 *
 *   Routed:
 *   - Low-frequency A clock (LFA) to LETIMER
 *
 * @note
 *   No requirement to enable the ULFRCO oscillator.
 *   It is always enabled in EM0-4H
 *
 ******************************************************************************/
void cmu_open(void){

    // Disable High-frequency peripheral clock
    CMU_ClockEnable(cmuClock_HFPER, false);

    // By default, Low Frequency Resistor Capacitor Oscillator, LFRCO, is enabled,
    // Disable the LFRCO oscillator
    CMU_OscillatorEnable(cmuOsc_LFRCO, false, false);

    // Disable the Low Frequency Crystal Oscillator, LFXO
    CMU_OscillatorEnable(cmuOsc_LFXO, false, false);

    // Route LFA clock to LETIMER0 clock tree
    CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO);

    // Now, you must ensure that the global Low Frequency is enabled
    CMU_ClockEnable(cmuClock_CORELE, true);
}
