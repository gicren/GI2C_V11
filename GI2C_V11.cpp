//*********************** Copyright Gicren Robotics and Electronics ***********************
//*File Name    : GI2C_V11.cpp
//*Author       : Gicren Team (www.gicren.com)
//*Version      : V0001
//*Data         : 11/01/2013
//*Description  : This file provides all the GI2C_V11 firmware functions
//*****************************************************************************************

#include <Arduino.h>
#include <Wire.h>
#include "GI2C_V11.h"

//------------------------------------------------------------
//Constructor
//------------------------------------------------------------
GI2CV11::GI2CV11(unsigned char *BufAddr,unsigned char Num)
{
  Wire.begin(); 
  this->BufStartAddr=BufAddr;
  this->BufSize=Num;
}

//------------------------------------------------------------
//Generate PID
//------------------------------------------------------------
unsigned char GI2CV11::GeneratePID(unsigned char PID)
{
  unsigned char count=0,temp=PID;
  while(temp)
  {
    temp&=(temp-1);
    count++;
  }
  if(count%2)
    return (PID<<1);
  else
    return ((PID<<1)+1);
}

//------------------------------------------------------------
//Check PID
//------------------------------------------------------------
unsigned char GI2CV11::CheckPID(unsigned char PID)
{
  unsigned char count=0,temp=PID;
  while(temp)
  {
    temp&=(temp-1);
    count++;
  }
  if(count%2)
    return PID;
  else 
    return 0xff;
}

//--------------------- Write Registers ----------------------
//SSA     : System Slave Address
//RegAddr : The start address of user registers to be written
//Num     : The number of bytes to be written
//------------------------------------------------------------
unsigned char GI2CV11::Write(unsigned char SSA,unsigned char RegAddr,unsigned char Num)
{
  unsigned char i,temp,check=0;
  
  if(((unsigned int)RegAddr+(unsigned int)Num)>this->BufSize)  
    return 0xff;
    
  //Write:RPID+Data+Check
  Wire.beginTransmission(SSA);
  Wire.write(this->GeneratePID(RegAddr));
  for(i=RegAddr;i<(RegAddr+Num);i++)
  {
    temp=*(this->BufStartAddr+(i%this->BufSize));
    Wire.write(temp);
    check+=temp;
  }
  check=~check;
  Wire.write(check);
  Wire.endTransmission();
  delayMicroseconds(100);
  
  //Write:HPID   
  Wire.beginTransmission(SSA);
  Wire.write(this->GeneratePID(GI2C_HPID));
  Wire.endTransmission();
  delayMicroseconds(100);
  
  //Read:Check
  while(Wire.available())
    Wire.read();
  Wire.requestFrom((int)SSA,1,true);
  if(Wire.read()==check)
    return 0;
  else
    return 0xff;
}

//---------------------- Read Registers ----------------------
//SSA     : System Slave Address
//RegAddr : The start address of user registers to be read
//Num     : The number of bytes to be read
//------------------------------------------------------------
unsigned char GI2CV11::Read(unsigned char SSA,unsigned char RegAddr,unsigned char Num)
{
  unsigned char i,temp,check=0;
  
  if(((unsigned int)RegAddr+(unsigned int)Num)>this->BufSize)  
    return 0xff;
  
  //Write:RPID
  Wire.beginTransmission(SSA);
  Wire.write(this->GeneratePID(RegAddr));
  Wire.endTransmission();
  delayMicroseconds(100);
  
  //Read:Data
  while(Wire.available())
    Wire.read();
  Wire.requestFrom((int)SSA,(int)Num,true);
  for(i=RegAddr;i<(RegAddr+Num);i++)
  { 
    temp=Wire.read();
    *(this->BufStartAddr+(i%this->BufSize))=temp;
    check+=temp;
  }
  check=~check;
  delayMicroseconds(100);
  
  //Write:HPID
  Wire.beginTransmission(SSA);
  Wire.write(this->GeneratePID(GI2C_HPID));
  Wire.endTransmission();
  delayMicroseconds(100);
  
  //Read:Check
  while(Wire.available())
    Wire.read();
  Wire.requestFrom((int)SSA,1,true);
  if(check==Wire.read())
    return 0;
  else
    return 0xff;
}

//------------------------ Send SPID -------------------------
//SSA     : System Slave Address
//PID     : SPID Value
//------------------------------------------------------------
void GI2CV11::SPID_Send(unsigned char SSA,unsigned char PID)
{
  Wire.beginTransmission(SSA);
  Wire.write(this->GeneratePID(PID));
  Wire.endTransmission();
}

//---------------------- Get Error Word ----------------------
//SSA     : System Slave Address
//------------------------------------------------------------
unsigned int GI2CV11::GetError(unsigned char SSA)
{
  unsigned int temp;
  //Write:EPID
  Wire.beginTransmission(SSA);
  Wire.write(this->GeneratePID(GI2C_EPID));
  Wire.endTransmission();
  delayMicroseconds(100);
  
  //Read:Error Word
  while(Wire.available())
    Wire.read();
  Wire.requestFrom((int)SSA,2,true);
  temp=Wire.read();
  temp<<=8;
  temp+=Wire.read();
  
  return temp;
}

//*********************** Copyright Gicren Robotics and Electronics ***********************
