#ifndef __COMMON_H__
#define __COMMON_H__

#include "BAT32A237.h"

#if defined BAT32G137_64PIN || defined BAT32A237_64PIN
	#define BAT32A2X7_64PIN
	#define BAT32G1X7_64PIN
#elif defined BAT32G137_48PIN || defined BAT32A237_48PIN
	#define BAT32A2X7_48PIN
	#define BAT32G1X7_48PIN	
#elif defined BAT32G137_40PIN || defined BAT32A237_40PIN
	#define BAT32A2X7_40PIN
	#define BAT32G1X7_40PIN	
#elif defined BAT32G137_32PIN || defined BAT32A237_32PIN
	#define BAT32A2X7_32PIN
	#define BAT32G1X7_32PIN		
#elif defined BAT32G137_24PIN || defined BAT32A237_24PIN
	#define BAT32A2X7_24PIN
	#define BAT32G1X7_24PIN	
#endif


typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;
typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))
//#define SPI_INTERFACE

#endif
