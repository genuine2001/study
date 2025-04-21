#ifndef __BSP_W25QXX_DRIVER_H__
#define __BSP_W25QXX_DRIVER_H__

/********************************* Includes ***********************************/
#include <stdint.h>
/******************************** End Includes ********************************/


/********************************** Defines ***********************************/
#define  W25QXX_DEBUG                   /* Enable debug output                */
#define  DEBUG_OUT(x)       printf x    /* Debug output function              */

#define  W25Q_INITED               1    /* W25QXX is Initialized              */
#define  W25Q_NOT_INITED           2    /* W25QXX is not Initialized          */

/************** W25QXX Command *****************/
#define W25QXX_WRITE_ENABLE                 0x06   
#define W25QXX_WRITE_DISABLE                0x04   
#define W25QXX_READ_STATUS_REGISTER_1       0x05   
#define W25QXX_READ_STATUS_REGISTER_2       0x35   
#define W25QXX_READ_STATUS_REGISTER_3       0x15   
#define W25QXX_WriteStatusReg1              0x01  
#define W25QXX_WriteStatusReg2              0x31   
#define W25QXX_WriteStatusReg3              0x11   
#define W25QXX_READ_DATA                    0x03   
#define W25QXX_FAST_READ                    0x0B   
#define W25QXX_FFAST_READ_DUAL_OUTPUT       0x3B   
#define W25QXX_PAGE_PROGRAM                 0x02   
#define W25QXX_BLOCK_ERASE_64KB             0xD8   
#define W25QXX_BLOCK_ERASE_32KB		        0x52  
#define W25QXX_SECTOR_ERASE                 0x20   
#define W25QXX_CHIP_ERASE                   0xC7   
#define W25QXX_POWER_DOWN	                0xB9   
#define W25QXX_RELEASE_POWER_DOWN           0xAB    
#define W25QXX_JEDEC_ID	           	        0x9F   
#define W25QXX_DUMMY_BYTE			        0xFF  
/********************* END *********************/

typedef enum
{
    W25QXX_OK             = 0,   /* W25QXX operation completed successfully   */
    W25QXX_ERRORSOURCE    = 1,   /* W25QXX Resource not available             */
    W25QXX_ERRORPARAMETER = 2,   /* W25QXX Parameter error                    */ 
    W25QXX_ERRORTIMEOUT   = 3    /* W25QXX Time out error                     */
} w25qxx_status_t;

typedef struct bsp_w25qxx bsp_w25qxx_t;
typedef struct bsp_w25qxx
{
    /*********************** Trage of Features ***********************/
    uint8_t                                                  is_inited; 

    /*********************** Trage of IO needs ***********************/
    /* The interface from Core layer                                 */  


    /************************** Trage of APIS  ***********************/
    w25qxx_status_t                   (*pf_init)(bsp_w25qxx_t * cthis);
    w25qxx_status_t                 (*pf_deinit)(bsp_w25qxx_t * cthis);
    w25qxx_status_t                (*pf_read_id)(bsp_w25qxx_t * cthis , 
                                                      uint8_t * MID   , 
                                                     uint16_t * DID  );                                                                             
    w25qxx_status_t                   (*pf_read)(bsp_w25qxx_t * cthis , 
                                                     uint32_t   addr  , 
                                                      uint8_t * data  , 
                                                     uint32_t   len  );                             
    w25qxx_status_t                  (*pf_write)(bsp_w25qxx_t * cthis ,
                                                     uint32_t   addr  , 
                                                      uint8_t * data  , 
                                                     uint32_t   len  );                             
    w25qxx_status_t           (*pf_erase_sector)(bsp_w25qxx_t * cthis,
                                                     uint32_t   addr );
    w25qxx_status_t             (*pf_erase_chip)(bsp_w25qxx_t * cthis);

} bsp_w25qxx_t;

w25qxx_status_t w25qxx_driver_inst( bsp_w25qxx_t * cthis );

#endif /* __BSP_W25QXX_DRIVER_H__ */ 