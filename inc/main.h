/*----------------------------------------------------------------------------*-

   main.h (Release 2017-02-24a)

  --------------------------------------------------------------------------------------
   
   This is the Processor Header file.

   This file needs to be adapted for each project.

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

#ifndef __MAIN_H__
#define __MAIN_H__

#include "typedef.h"
#include "compiler_defs.h"             // Compiler declarations
#include "C8051F380_defs.h"            // SFR declarations

// Required system operating frequency (in Hz)
// Will be checked in the scheduler initialisation file
#define REQUIRED_PROCESSOR_CORE_CLOCK (48000000)

#include "mcu.h"

// Port header
//#include "port.h"

// Processor module
#include "processor.h"

// Scheduler module
#include "scheduler.h"

#endif

/*----------------------------------------------------------------------------*-
  ------------------------------ END OF FILE ---------------------------------
-*----------------------------------------------------------------------------*/

