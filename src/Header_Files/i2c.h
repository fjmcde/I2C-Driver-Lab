//*******************************************************
// header guards
//*******************************************************
#ifndef I2C_HG
#define I2C_HG


//***********************************************************************************
// included files
//***********************************************************************************
// system included files
#include <stdbool.h>


// Silicon Labs included files
#include "em_i2c.h"
#include "em_assert.h"


// developer included files
#include "cmu.h"


//***********************************************************************************
// defined macros
//***********************************************************************************
#define I2C0_SCL_ROUTE15  I2C_ROUTELOC0_SCLLOC_LOC15  // SCL PC11: route location #15 (TRM 6.3 pg 75)
#define I2C0_SDA_ROUTE15  I2C_ROUTELOC0_SDALOC_LOC15  // SDA PC10: route location #15 (TRM 6.3 pg 78)
#define I2C0_SCL_PEN      I2C_ROUTEPEN_SCLPEN         // SCL PEN is bit 1 (TRM: 16.5.18)
#define I2C0_SDA_PEN      I2C_ROUTEPEN_SDAPEN         // SDA PEN is bit 2 (TRM 16.5.18)
#define I2C_FREQ          I2C_FREQ_FAST_MAX           // Max I2C frequency is 4kHz (EFM32PG12 DS 4.1.20.2 & Si7021-A20 DS Table 3)
#define I2C_CLHR_6_3      i2cClockHLRAsymetric        // IC2 CLHR 6:3 (TRM 16.5.1 & EFM32PG12 HAL I2C_ClockHLR_TypeDef enumeration)


//***********************************************************************************
// enums
//***********************************************************************************


//***********************************************************************************
// structs
//***********************************************************************************
// I2C struct for use in opening the I2C peripheral
typedef struct
{
  bool                  enable;   // enable I2C peripheral when init complete
  bool                  master;   // set to master (true) or Slave (false)
  uint32_t              refFreq;  // I2C reference clock assumed when configuring bus frequency setup
  uint32_t              freq;     // max I2C bus frequency
  I2C_ClockHLR_TypeDef  clhr;     // clock low/high ratio control
  uint32_t              scl_loc0; // SCL route to GPIO port/pin
  uint32_t              sda_loc0; // SDA route to GPIO port/pin
  uint32_t              scl_pen;  // enable SCL pin
  uint32_t              sda_pen;  // enable SDA pin
}I2C_OPEN_STRUCT;


// I2C struct for managing the I2C state machine
// TODO: BUILD STATE MACHINE AND FILL IN STRUCT WITH RELEVENT DATA
typedef struct
{


}I2C_STATE_MACHINE;


//***********************************************************************************
// function prototypes
//***********************************************************************************
void i2c_open(I2C_TypeDef *i2c, I2C_OPEN_STRUCT *app_i2c_struct);


#endif
