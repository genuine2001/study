#include "bsp_w25qxx.h"

static void w25qxx_cs_set(uint8_t value)
{

}

static uint8_t w25qxx_write_read_byte(uint8_t indata);
{

}

static uint8_t w25qxx_get_flag(uint32_t device_spi, uint8_t flag)
{

}

static w25qxx_status_t w25qxx_init( bsp_w25qxx_t *cthis )
{
    /* 1. Check the parameters */
    if ( NULL == cthis )
    {
#ifdef W25QXX_DEBUG
        DEBUG_OUT("W25QXX_ERRORPARAMETER");
#endif  //DEBUG
        return W25QXX_ERRORPARAMETER;
    }

    /* 2. Add your init code here */
    

    /* 3. Set the is_inited flag */
    cthis->is_inited = W25Q_INITED;

    return W25QXX_OK;
}

static w25qxx_status_t w25qxx_deinit( bsp_w25qxx_t *cthis )
{
    /* 1. Check the parameters */
    if ( NULL == cthis )
    {
#ifdef W25QXX_DEBUG
        DEBUG_OUT("W25QXX_ERRORPARAMETER");
#endif  //DEBUG
        return W25QXX_ERRORPARAMETER;
    }

    /* 2. Add your deinit code here */
    

    /* 3. Clear the is_inited flag */
    cthis->is_inited = W25Q_NOT_INITED;

    return W25QXX_OK;
}

static w25qxx_status_t w25qxx_read_id(     void * cthis,
                                        uint8_t *   mid,
                                       uint16_t *   did)
{
    /* 1. Check the parameters */
    if ( NULL == cthis ||
         NULL == mid   ||
         NULL == did   )
    {
#ifdef W25QXX_DEBUG
        DEBUG_OUT("W25QXX_ERRORPARAMETER");
#endif  //DEBUG
        return W25QXX_ERRORPARAMETER;
    }

    /* 2. Add your read_id code here */
    w25qxx_cs_set(0);
    w25qxx_write_read_byte(0x9F);
    *mid = w25qxx_write_read_byte(0XFF);
    *did = w25qxx_write_read_byte(0XFF);
    *did <<= 8;
    *did |= w25qxx_write_read_byte(0XFF);
    w25qxx_cs_set(0);

    return W25QXX_OK;
}

w25qxx_status_t w25qxx_driver_inst( bsp_w25qxx_t * cthis )
{
    /* 1. Check the parameters */
    if ( NULL == cthis )
    {
#ifdef W25QXX_DEBUG
        DEBUG_OUT("W25QXX_ERRORPARAMETER");
#endif  //DEBUG
        return W25QXX_ERRORPARAMETER;
    }

    /* 2. Check the Resourse */
    if ( W25Q_INITED == w25qxx->is_inited )
    {
#ifdef W25QXX_DEBUG
        DEBUG_OUT("W25QXX_ERRORPARAMETER");
#endif  //DEBUG
        return W25QXX_ERRORSOURCE;
    }

#ifdef W25QXX_DEBUG
    DEBUG_OUT("W25QXX driver inst Start");
#endif  //DEBUG

    /* 3. Initialize the w25qxx driver */
    cthis->pf_read_id = w25qxx_read_id;
    cthis->pf_init    = w25qxx_init;
    cthis->pf_deinit  = w25qxx_deinit;

#ifdef W25QXX_DEBUG
    DEBUG_OUT("W25QXX driver inst OK");
#endif  //DEBUG

    return W25QXX_OK;
}