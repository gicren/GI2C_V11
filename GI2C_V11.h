//*********************** Copyright Gicren Robotics and Electronics ***********************
//* File Name    : GI2C_V11.h
//* Author       : Gicren Team (www.gicren.com)
//* Version      : V0001
//* Data         : 11/01/2013
//* Description  : This file contains all the functions prototypes for the GI2C_V11 library
//*****************************************************************************************

#ifndef _GI2C_V11_H
#define _GI2C_V11_H

#define GI2C_HPID     0x7f  //Handshake , Get the check byte of last instruction from slave device
#define GI2C_EPID     0x7e  //Get the Error Word
#define GI2C_SPID_FC  0x7d  //Factory settings
#define GI2C_SPID_EE  0x7c  //Update the bytes stored in EEPROM
#define GI2C_SPID_RS  0x7b  //Restart
#define GI2C_SPID_EC  0x7a  //Clear the Error Word
#define GI2C_SPID_CE  0x79  //Enable the immediate write mode
#define GI2C_SPID_AE  0x78  //Enable the non immediate write mode
#define GI2C_SPID_AC  0x77  //Action with data from non immediate write mode

class GI2CV11
{
  private:
    unsigned char *BufStartAddr;
    unsigned char BufSize;
    unsigned char GeneratePID(unsigned char PID);
    unsigned char CheckPID(unsigned char PID);
    
  public:
    GI2CV11(unsigned char *BufAddr,unsigned char Num);
    unsigned char Write(unsigned char SSA,unsigned char RegAddr,unsigned char Num);
    unsigned char Read(unsigned char SSA,unsigned char RegAddr,unsigned char Num);
    void SPID_Send(unsigned char SSA,unsigned char PID);
    unsigned int GetError(unsigned char SSA);
};

#endif

//*********************** Copyright Gicren Robotics and Electronics ***********************
