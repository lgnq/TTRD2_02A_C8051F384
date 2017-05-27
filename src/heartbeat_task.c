/*----------------------------------------------------------------------------*-

  ttrd2-02a-t0091a-v001c_heartbeat_sw_task.c (Release 2017-02-24a)

  ----------------------------------------------------------------------------
   
  Simple 'Heartbeat switch' task for STM32F091RC.

  Targets Nucleo F091RC board.

  Usually released once per second => flashes 0.5 Hz, 50% duty cycle.

  Linked to switch interface: if Button 1 is pressed, LED will not flash.

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

// Module header
#include "heartbeat_task.h"

/*----------------------------------------------------------------------------*-

  HEARTBEAT_SW_Init()

  Prepare for HEARTBEAT_SW_Update() function - see below.
  
  PARAMETERS:
     None.

  LONG-TERM DATA:
     None.

  MCU HARDWARE:
     GPIO pin (Heartbeat pin).

  PRE-CONDITION CHECKS:
     None.

  POST-CONDITION CHECKS:
     None.

  ERROR DETECTION / ERROR HANDLING:
     None.

  RETURN VALUE:
     None.

-*----------------------------------------------------------------------------*/
void HEARTBEAT_SW_Init(void)
{
    P0MDOUT = 0x03;     // Enable LED as a push-pull output
    P0SKIP  = 0xCF;		// GPIO, GPIO, TX, RX, GPIO...

    XBR1 = 0xC0;        // disable weak pull-up, enable crossbar    
}

/*----------------------------------------------------------------------------*-

  HEARTBEAT_SW_Update()

  Flashes at 0.5 Hz.

  Must schedule every second (soft deadline).
   
  PARAMETERS:
     None.

  LONG-TERM DATA:
     Heartbeat_state_s (W)

  MCU HARDWARE:
     GPIO pin (Heartbeat pin).

  PRE-CONDITION CHECKS:
     None.

  POST-CONDITION CHECKS:
     None.

  ERROR DETECTION / ERROR HANDLING:
     None.

  WCET:
     Not yet determined.

  BCET:
     Not yet determined.

  RETURN VALUE:
     None.
 
-*----------------------------------------------------------------------------*/
void HEARTBEAT_SW_Update(void)
{
    static uint32_t Heartbeat_state_s = 0;

    // Change the LED from OFF to ON (or vice versa)
    if (Heartbeat_state_s == 1)
    {
        LED_R = 1;
        LED_G = 0;
        Heartbeat_state_s = 0;
    }
    else
    {
        LED_R = 0;
        LED_G = 1;        
        Heartbeat_state_s = 1;
    }
}

/*----------------------------------------------------------------------------*-
  ------------------------------ END OF FILE ---------------------------------
-*----------------------------------------------------------------------------*/
