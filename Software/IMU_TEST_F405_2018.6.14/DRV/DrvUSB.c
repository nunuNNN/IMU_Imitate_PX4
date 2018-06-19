
/**
* @file     bsp_usb.c
* @brief    
* @version  
* @author   
* @date     2017-04-26
* @note     compatible for both 'FS' & 'HS' (FS: Full Speed, HS: High Speed)
*/


#include "DrvUSB.h"
#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usb_conf.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"
#include "usb_dcd_int.h"


__ALIGN_BEGIN USB_OTG_CORE_HANDLE  USB_OTG_dev __ALIGN_END;

extern CDC_IF_Prop_TypeDef VCP_fops;
extern uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);

void (*pUsbVCPRecvData)(uint8_t *buf, int len);

/******************************************************************************
* @函 数 名： DrvUSB_Init
* @函数描述： USB初始化
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void DrvUSB_Init(void)
{
		USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_CDC_cb, &USR_cb);
}

/******************************************************************************
* @函 数 名： DrvUSB_RegisterRxHandler
* @函数描述： 注册USBRX通信处理函数
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void DrvUSB_RegisterRxHandler(void *pFun)
{
  if( pFun != NULL )
  {
    pUsbVCPRecvData = (void (*)(uint8_t *buf, int len)) pFun;
  }
}


/******************************************************************************
* @函 数 名： DrvUSB_SendData
* @函数描述： usb 发送数据
* @参    数： 
* @返 回 值： 
* @备    注： 
******************************************************************************/
void DrvUSB_SendData(u8* buf, u16 len)
{	
	VCP_fops.pIf_DataTx(buf, len);
}

/**
  * @brief  This function handles OTG_HS Handler.
  * @param  None
  * @retval None
  */
#ifdef USE_USB_OTG_HS  
void OTG_HS_IRQHandler(void)
#else
void OTG_FS_IRQHandler(void)
#endif
{
  USBD_OTG_ISR_Handler (&USB_OTG_dev);
}



/*********END OF FILE****/




