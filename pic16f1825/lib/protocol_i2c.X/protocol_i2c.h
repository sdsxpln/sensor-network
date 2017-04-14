#ifndef PROTOCOL_I2C_H
#define	PROTOCOL_I2C_H

#ifdef	__cplusplus
extern "C" {
#endif
         
    /*
     * Functions
     */
    void PROTOCOL_I2C_Initialize(uint8_t device_id);
    uint8_t PROTOCOL_I2C_Who(void);
    void PROTOCOL_I2C_Sav(void);
    void PROTOCOL_I2C_Set_TLV(uint8_t type, uint8_t length, uint8_t *pbuffer);
    bool PROTOCOL_I2C_TLV_Status(void);
    void PROTOCOL_I2C_Sta(void);
    void PROTOCOL_I2C_Stp(void);
    uint8_t* PROTOCOL_I2C_Sen();
    
#ifdef	__cplusplus
}
#endif

#endif	/* PROTOCOL_I2C_H */

