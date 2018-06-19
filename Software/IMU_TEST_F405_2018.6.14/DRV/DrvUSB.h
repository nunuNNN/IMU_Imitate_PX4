
/**
* @file     DrvUSB.h
* @brief    
* @version  
* @author   
* @date     2017-04-26
* @note     
*/

#ifndef DRV_USB_H
#define DRV_USB_H

#ifdef __cplusplus
 extern "C" {
#endif 


#include "stmHead.h"


extern void DrvUSB_Init(void);
extern void DrvUSB_RegisterRxHandler(void *pFun);
extern void DrvUSB_SendData(u8* buf, u16 len);


#ifdef __cplusplus
}
#endif


#endif

/*********END OF FILE****/


