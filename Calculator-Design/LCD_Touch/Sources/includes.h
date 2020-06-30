#ifndef _INCLUDES_H_
#define _INCLUDES_H_

#include <hidef.h>           /* common defines and macros */
#include <MC9S12XS128.h>     /* derivative information */

#include <ctype.h>
#include <string.h>
#include <stdarg.h>


#define  uint8   unsigned char
#define  int8    signed char
#define  uint16  unsigned int
#define  int16   signed int
#define  uint32  unsigned long
#define  int32   signed long
/* Typedefs */
typedef unsigned char     UINT8;  		/*unsigned 8 bit definition */
typedef unsigned short    UINT16; 		/*unsigned 16 bit definition*/
typedef unsigned long     UINT32; 		/*unsigned 32 bit definition*/
typedef signed char       INT8;   		/*signed 8 bit definition */
typedef short      		    INT16;  		/*signed 16 bit definition*/
typedef long int    	    INT32;  		/*signed 32 bit definition*/


#ifndef  TRUE
#define  TRUE 1
#endif
#ifndef  FALSE
#define  FALSE 0
#endif

//包含自定义头文件
#include "ili9320.h" 
#include "ads7843drv.h"  
  
#endif 