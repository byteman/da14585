

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STD_TYPES_H
#define __STD_TYPES_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity */
typedef unsigned int   INT16U;                   /* Unsigned 16 bit quantity */
typedef signed   int   INT16S;                   /* Signed   16 bit quantity */
typedef unsigned long  INT32U;                   /* Unsigned 32 bit quantity */
typedef signed   long  INT32S;                   /* Signed   32 bit quantity */
typedef float			FP32;					/* Single precision floating point */
typedef double			FP64;					/* Double precision floating point */
#define	BYTE			INT8S					/* Define data types for backward compatibility ... */

typedef unsigned long   INTCPUU;                 /* Unsigned 32 bit quantity */
typedef signed   long   INTCPUS;                 /* Signed   32 bit quantity */

typedef  INT32S  Std_ReturnType;	/* 函数返回值标准格式，0：正确  负数：错误  正数：返回的状态参数 */
//#define  E_OK			0			/* 函数返回值格式 */ 				 
//#define  E_NOT_OK		-1			/* 函数返回值格式 */



/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

#define  NUL         0x00
#define  TRUE           1
#define  FALSE          0 

#define DebugPtf			printf			//调试printf输出
//#define DebugPtf							//关闭调试

/* Exported functions ------------------------------------------------------- */

#endif /* __STD_TYPES_H */

