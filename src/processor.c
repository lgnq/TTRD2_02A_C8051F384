/*----------------------------------------------------------------------------*-

  ttrd2-02a-t0091a-v001c_processor.c (Release 2017-02-24a)

  Configures Processor in one of the following Modes / States after a reset:

    FAIL_SAFE_S
    NORMAL_M

-*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*-

  This code is copyright (c) 2014-2016 SafeTTy Systems Ltd.

  This code forms part of a Time-Triggered Reference Design (TTRD) that is 
  documented in the following book: 

   Pont, M.J. (2016) 
   "The Engineering of Reliable Embedded Systems (Second Edition)", 
   Published by SafeTTy Systems Ltd. ISBN: 978-0-9930355-3-1.

  Both the TTRDs and the above book ("ERES2") describe patented 
  technology and are subject to copyright and other restrictions.

  This code may be used without charge: [i] by universities and colleges on 
  courses for which a degree up to and including 'MSc' level (or equivalent) 
  is awarded; [ii] for non-commercial projects carried out by individuals 
  and hobbyists.

  Any and all other use of this code and / or the patented technology 
  described in ERES2 requires purchase of a ReliabiliTTy Technology Licence:
  http://www.safetty.net/technology/reliabilitty-technology-licences

  Please contact SafeTTy Systems Ltd if you require clarification of these 
  licensing arrangements: http://www.safetty.net/contact

  CorrelaTTor, DecomposiTTor, DuplicaTTor, MoniTTor, PredicTTor, ReliabiliTTy,  
  SafeTTy, SafeTTy Systems, TriplicaTTor and WarranTTor are registered 
  trademarks or trademarks of SafeTTy Systems Ltd in the UK & other countries.

-*----------------------------------------------------------------------------*/

// Processor Header
#include "main.h"

// Tasks
#include "heartbeat_task.h"
#include "uart_task.h"
//#include "../tasks/ttrd2-02a-t0091a-v001c_iwdt_task.h"
//#include "../tasks/ttrd2-02a-t0091a-v001c_switch_task.h"

// ------ Private variable definitions -----------------------------

// The current Processor Mode / State
static eProc_MoSt Processor_MoSt_g;

// ------ Private function declarations ----------------------------

void PROCESSOR_Identify_Reqd_MoSt(void);
void PROCESSOR_Configure_Required_Mode(void);

/*----------------------------------------------------------------------------*-

  PROCESSOR_Init()

  Wrapper for system startup functions.
  
  PARAMETERS:
     None.

  LONG-TERM DATA:
     None.

  MCU HARDWARE:
     None.

  PRE-CONDITION CHECKS:
     None.

  POST-CONDITION CHECKS:
     None.

  ERROR DETECTION / ERROR HANDLING:
     None.

  RETURN VALUE:
     None.

-*----------------------------------------------------------------------------*/
void PROCESSOR_Init(void)
{
    PROCESSOR_Identify_Reqd_MoSt();
    PROCESSOR_Configure_Required_Mode();
}

/*----------------------------------------------------------------------------*-

  PROCESSOR_Identify_Reqd_MoSt()

  Identify the cause of the system reset.
  Set the system mode / state accordingly.

  PARAMETERS:
     None.

  LONG-TERM DATA:
     Processor_MoSt_g (W)

  MCU HARDWARE:
     Registers indicating cause of reset (R).

  PRE-CONDITION CHECKS:
     None.

  POST-CONDITION CHECKS:
     None.

  ERROR DETECTION / ERROR HANDLING:
     Check for reset caused by iWDT => 'fail safe'

  RETURN VALUE:
     None.
   
-*----------------------------------------------------------------------------*/
void PROCESSOR_Identify_Reqd_MoSt(void)
{
#if 0    
    // Check cause of reset
    if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET)
    {
        // Reset was caused by WDT => "Fail Silent" state
        Processor_MoSt_g = FAIL_SAFE_S;
    }
    else
    {
        // Here we treat all other forms of reset in the same way
        // => "Normal" mode
        Processor_MoSt_g = NORMAL_M;
    }
      
    // Clear cause-of-reset flags
    RCC_ClearFlag();     
#else
    uint8_t WDT_flag = 0;  // Set to 1 if WDT caused latest reset
   
    // Disable Watchdog timer, WDTE = 0
    PCA0MD &= ~0x40;						
   
    // Check cause of reset
    // First check the PORSF bit:
    // if PORSF is set, all other RSTSRC flags are indeterminate.
    if ((RSTSRC & 0x02) == 0x00)        
    {
        if (RSTSRC == 0x08)
        {
            // Last reset was caused by WDT
            WDT_flag = 1;
        }
    }

    if (WDT_flag == 1)
    {
        // Reset was caused by WDT => "Fail Silent" state
        Processor_MoSt_g = FAIL_SAFE_S;
    }
    else
    {
        // Here we treat all other forms of reset in the same way
        // => "Normal" mode
        Processor_MoSt_g = NORMAL_M;
    }    
#endif
}

/*----------------------------------------------------------------------------*-

  PROCESSOR_Configure_Required_Mode()

  Configure the system in the required mode.  

  PARAMETERS:
     None.

  LONG-TERM DATA:
     Processor_MoSt_g (R)

  MCU HARDWARE:
     None.

  PRE-CONDITION CHECKS:
     None.

  POST-CONDITION CHECKS:
     None.

  ERROR DETECTION / ERROR HANDLING:
     None.

  RETURN VALUE:
     None.
   
-*----------------------------------------------------------------------------*/
void PROCESSOR_Configure_Required_Mode(void)
{
    switch (Processor_MoSt_g)
    {
        // Default to "Fail Safe" state 
        default:          
        case FAIL_SAFE_S:
        {
            // Reset caused by iWDT
            // Trigger "fail safe" behaviour
            PROCESSOR_Perform_Safe_Shutdown();

            break;
        }

        // NORMAL mode
        case NORMAL_M:
        {
            sysclk_init();
            port_init();
            
            // Set up the scheduler for 1 ms ticks (tick interval in *ms*)
            SCH_Init_Milliseconds(1);

            // Set up WDT 
            // Timeout is parameter * 100 µs: 25 => ~2.5 ms
            // NOTE: WDT driven by RC oscillator - timing varies with temperature 
//            WATCHDOG_Init(25);

            // Prepare for switch-reading task
//            SWITCH_BUTTON1_Init();

            // Prepare for heartbeat task
            HEARTBEAT_SW_Init();
            
            // Prepare for UART1 task (set baud rate)
            UART2_BUF_O_Init(115200);
          
            // Report mode (via buffer)          
            UART2_BUF_O_Write_String_To_Buffer("\nNormal mode\n");              

            // Add tasks to schedule.
            // Parameters are:
            // A. Task name
            // B. Initial delay / offset (in Ticks)
            // C. Task period (in Ticks): Must be > 0
            //           A                      B  C
//            SCH_Add_Task(WATCHDOG_Update,       0, 1);    // Feed watchdog
//            SCH_Add_Task(SWITCH_BUTTON1_Update, 0, 10);   // Switch interface 
            SCH_Add_Task(HEARTBEAT_SW_Update,   0, 1000); // Heartbeat LED
            SCH_Add_Task(UART2_BUF_O_Update,    0, 1);    // UART reports

            // Feed the watchdog
//            WATCHDOG_Update();

            break;
        }
    }
}

/*----------------------------------------------------------------------------*-

  PROCESSOR_Perform_Safe_Shutdown()

  Attempt to move the system into a safe ('silent') state.

  PARAMETERS:
     None.

  LONG-TERM DATA:
     None.

  MCU HARDWARE:
     Heartbeat LED (GPIO pin).

  PRE-CONDITION CHECKS:
     None.

  POST-CONDITION CHECKS:
     None.

  ERROR DETECTION / ERROR HANDLING:
     None.

  RETURN VALUE:
     None.
   
-*----------------------------------------------------------------------------*/
void PROCESSOR_Perform_Safe_Shutdown(void)
{
    uint32_t Delay1, Delay2;

    // Here we simply "fail silent" with rudimentary fault reporting.
    // OTHER BEHAVIOUR IS LIKELY TO BE REQUIRED IN YOUR DESIGN

    // *************************************
    // NOTE: This function should NOT return
    // *************************************

    // Set up Heartbeat LED pin
    HEARTBEAT_SW_Init();

    while (1)
    {
        // Flicker Heartbeat LED to indicate fault
        for (Delay1 = 0; Delay1 < 500000; Delay1++) 
        {
            Delay2 *= 3;
        }

        HEARTBEAT_SW_Update();
    }
}

/*----------------------------------------------------------------------------*-
  ------------------------------ END OF FILE ---------------------------------
-*----------------------------------------------------------------------------*/
