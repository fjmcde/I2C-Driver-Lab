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
#include "sleep_routines.h"


//***********************************************************************************
// defined macros
//***********************************************************************************
// I2Cn SCL & SDA
#define I2C0_SCL_ROUTE15  I2C_ROUTELOC0_SCLLOC_LOC15  // SCL PC11: route location #15 (TRM 6.3 pg 75)
#define I2C0_SDA_ROUTE15  I2C_ROUTELOC0_SDALOC_LOC15  // SDA PC10: route location #15 (TRM 6.3 pg 78)
#define I2C0_SCL_PEN      I2C_ROUTEPEN_SCLPEN         // SCL PEN is bit 1 (TRM: 16.5.18)
#define I2C0_SDA_PEN      I2C_ROUTEPEN_SDAPEN         // SDA PEN is bit 2 (TRM 16.5.18)
// I2Cn Clock
#define I2C_FREQ          I2C_FREQ_FAST_MAX           // Max I2C frequency is 4kHz (EFM32PG12 DS 4.1.20.2 & Si7021-A20 DS Table 3)
#define I2C_CLHR_6_3      i2cClockHLRAsymetric        // IC2 CLHR 6:3 (TRM 16.5.1 & EFM32PG12 HAL I2C_ClockHLR_TypeDef enumeration)
// I2Cn State Machine Bus Busy [busy]
#define I2C_BUS_READY     (0x0UL << 0)                // Clear when bus is available
#define I2C_BUS_BUSY      (0x1UL << 0)                // Set when bus is busy
// I2C Energy Modes
#define I2C_EM_BLOCK      EM2                         // I2C Cannot go below EM2

//***********************************************************************************
// enums
//***********************************************************************************
typedef enum
{
  req_res,          /* Request resource: Transmit START bit, 7-bit slave addr + r/w-bit (TRM 16.3.7.6: 0x57) */
  command_tx,       /* Transmit command to device (TRM 16.3.7.6: 0x93)*/
  data_rx,          /* Data received (TRM 16.3.7.6)*/
  m_stop_tx,        /* STOP bit sent */
  end_proc          /* m_stop bit received; end process */
}I2C_MACHINE_STATES_Typedef;

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


// I2C struct for managing the I2C state machine. Instantiated as a private
// data struct in i2c.c
typedef struct
{
    I2C_TypeDef                  *I2Cn;                   // pointer to I2C peripheral (I2C0 or I2C1)
    I2C_MACHINE_STATES_Typedef    curr_state;             // tracks the current state of the state machine
    uint32_t                      slave_addr;             // pointer to the address of slave device currently being communicated with
    uint32_t                      r_w;                    // set read/write bit
    bool                          busy;                   // True when bus is busy; False when bus is available
    uint32_t                     *rxdata;                 // pointer to receive buffer address
    uint32_t                     *txdata;                 // pointer to transmit buffer address
    uint32_t                      data;                   // store state machine received/transmit data
    uint32_t                      num_bytes;              // number of bytes expected
    uint32_t                      i2c_cb;                 // I2C call back event to request upon completion of I2C operation
}I2C_STATE_MACHINE_STRUCT;


//***********************************************************************************
// function prototypes
//***********************************************************************************
void i2c_open(I2C_TypeDef *i2c, I2C_OPEN_STRUCT *app_i2c_struct);
void i2c_start(I2C_TypeDef *i2c, uint32_t slave_addr, uint32_t r_w, uint32_t *read_result);

#endif
