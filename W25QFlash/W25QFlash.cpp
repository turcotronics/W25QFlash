/*
 * W25QFlash.cpp
 * 
 * Version 1.0.0
 * https://github.com/turcotronics/W25QFlash
 * 
 * Created by Rodolfo Turco, Turcotronics
 * www.turcotronics.it
 * www.turcotronics.com
 * https://github.com/turcotronics
 *
 * MIT License
 *
 * Simple library for Winbond W25Qxx flash that could work with other brands too
 * Tested on Winbond W25Q64JV
 *
 */

#include "W25QFlash.h"

#define W25QFlash_PAGE_PROGRAM       0x02
#define W25QFlash_READ_DATA          0x03
#define W25QFlash_WRITE_DISABLE      0x04
#define W25QFlash_STATUS_REG_1       0x05
#define W25QFlash_WRITE_ENABLE       0x06
#define W25QFlash_SECTOR_ERASE4K     0x20
#define W25QFlash_BLOCK_ERASE32K     0x52
#define W25QFlash_JEDEC_ID           0x9F
#define W25QFlash_CHIP_ERASE         0xC7
#define W25QFlash_BLOCK_ERASE64K     0xD8
 
//______________________________________
W25QFlash::W25QFlash()
{
}

//______________________________________
void W25QFlash::begin(int flash_CS_pin, SPIClass *SPIint)
{
  flash_CS=flash_CS_pin;
  SPIi=SPIint;
  pinMode(flash_CS, OUTPUT);
  getJedecId();
}

//______________________________________
uint8_t W25QFlash::getManufactureId()
{
  return mid;
}

//______________________________________
uint8_t W25QFlash::getType()
{
  return mty;
}

//______________________________________
uint8_t W25QFlash::getCapacity()
{
  return mca;
}

//______________________________________
bool W25QFlash::isBusy()
{
  digitalWrite(flash_CS, LOW);
  SPIi->transfer(W25QFlash_STATUS_REG_1);
  bool ret = ((SPIi->transfer(0) & 0x01)==0x01);
  digitalWrite(flash_CS, HIGH);
  return ret;
}

//______________________________________
bool W25QFlash::waitReady(int32_t timeout)
{
  bool bRet=true;
  int32_t iCount=0;
  while(isBusy())
  {
    delayMicroseconds(1000);
    iCount++;
    if(iCount>timeout)
    {
      bRet=false;
      break;
    }
  }
  return(bRet);
}

//______________________________________
// bytes
uint8_t W25QFlash::byteRead(uint32_t addr)
{
  digitalWrite(flash_CS, LOW);
  command(W25QFlash_READ_DATA, addr);
  uint8_t ret = SPIi->transfer(0);
  digitalWrite(flash_CS, HIGH);
  return ret;
}

//______________________________________
// bytes
void W25QFlash::bytesRead(uint32_t addr, uint8_t data[], uint32_t size)
{
  digitalWrite(flash_CS, LOW);
  command(W25QFlash_READ_DATA, addr);
  for(int i=0; i<size; i++)
    data[i] = SPIi->transfer(0);
  digitalWrite(flash_CS, HIGH);
}

//______________________________________
// records of 16 bytes
// record 0 starts at address 0, record 1 starts at address 16 and so on
void W25QFlash::recordRead16(uint32_t record, uint8_t data[])
{
  digitalWrite(flash_CS, LOW);
  command(W25QFlash_READ_DATA, ((uint32_t)record)<<4);
  for(int i=0; i<16; i++)
    data[i] = SPIi->transfer(0);
  digitalWrite(flash_CS, HIGH);
}

//______________________________________
// pages of 256 bytes
// page 0 starts at address 0, page 1 starts at address 256 and so on
void W25QFlash::pageRead256(uint16_t page, uint8_t data[])
{
  digitalWrite(flash_CS, LOW);
  command(W25QFlash_READ_DATA, ((uint32_t)page)<<8);
  for(int i=0; i<256; i++)
    data[i] = SPIi->transfer(0);
  digitalWrite(flash_CS, HIGH);
}

//______________________________________
void W25QFlash::byteWrite(uint32_t addr, uint8_t data)
{
  writeEnable();
  digitalWrite(flash_CS, LOW);
  command(W25QFlash_PAGE_PROGRAM, addr);
  SPIi->transfer(data);
  digitalWrite(flash_CS, HIGH);
}

//______________________________________
// records of 16 bytes
// record 0 starts at address 0, record 1 starts at address 16 and so on
void W25QFlash::recordWrite16(uint32_t record, uint8_t data[])
{
  writeEnable();
  digitalWrite(flash_CS, LOW);
  command(W25QFlash_PAGE_PROGRAM, ((uint32_t)record)<<4);
  for (int i = 0; i < 16; ++i)
    SPIi->transfer(data[i]);
  digitalWrite(flash_CS, HIGH);
}

//______________________________________
// pages of 256 bytes
// page 0 starts at address 0, page 1 starts at address 256 and so on
void W25QFlash::pageWrite256(uint16_t page, uint8_t data[])
{
  writeEnable();
  digitalWrite(flash_CS, LOW);
  command(W25QFlash_PAGE_PROGRAM, ((uint32_t)page)<<8);
  for (int i = 0; i < 256; ++i)
    SPIi->transfer(data[i]);
  digitalWrite(flash_CS, HIGH);
}

//______________________________________
// sectors of 4K bytes
// sector 0 starts at address 0, sector 1 starts at address 4096 and so on
void W25QFlash::sectorErase4K(uint16_t sector)
{
  writeEnable();
  digitalWrite(flash_CS, LOW);
  command(W25QFlash_SECTOR_ERASE4K, ((uint32_t)sector)<<12);
  digitalWrite(flash_CS, HIGH);
}

//______________________________________
// blocks of 32K bytes
// block 0 starts at address 0, block 1 starts at address 32768 and so on
void W25QFlash::blockErase32K(uint8_t block)
{
  writeEnable();
  digitalWrite(flash_CS, LOW);
  command(W25QFlash_BLOCK_ERASE32K, ((uint32_t)block)<<15);
  digitalWrite(flash_CS, HIGH);
}

//______________________________________
// blocks of 64K bytes
// block 0 starts at address 0, block 1 starts at address 65536 and so on
void W25QFlash::blockErase64K(uint8_t block)
{
  writeEnable();
  digitalWrite(flash_CS, LOW);
  command(W25QFlash_BLOCK_ERASE64K, ((uint32_t)block)<<16);
  digitalWrite(flash_CS, HIGH);
}

//______________________________________
// bytes
void W25QFlash::chipErase()
{
  writeEnable();
  digitalWrite(flash_CS, LOW);
  SPIi->transfer(W25QFlash_CHIP_ERASE);
  digitalWrite(flash_CS, HIGH);
}

//____________________________________________________________________________
// Private

//______________________________________
void W25QFlash::getJedecId()
{
  digitalWrite(flash_CS, LOW);
  SPIi->transfer(W25QFlash_JEDEC_ID);
  mid = SPIi->transfer(0);
  mty = SPIi->transfer(0);
  mca = SPIi->transfer(0);
  digitalWrite(flash_CS, HIGH);
}

//______________________________________
void W25QFlash::writeEnable()
{
  digitalWrite(flash_CS, LOW);
  SPIi->transfer(W25QFlash_WRITE_ENABLE);
  digitalWrite(flash_CS, HIGH);
  delayMicroseconds(1);
}

//______________________________________
void W25QFlash::command(uint8_t cmd, uint32_t addr)
{
  SPIi->transfer(cmd); 
  SPIi->transfer((addr >> 16) & 0xFF);
  SPIi->transfer((addr >> 8) & 0xFF); 
  SPIi->transfer(addr & 0xFF);
}
