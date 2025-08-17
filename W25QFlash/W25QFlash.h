/*
 * W25QFlash.h
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

#ifndef W25QFlash_h
#define W25QFlash_h

#include "Arduino.h"
#include "SPI.h"

class W25QFlash
{
  public:
  //______________________________________
  // Costructor
  W25QFlash();
  
  //______________________________________
  // To be called in setup()
  void begin(int flash_CS_pin, SPIClass *SPIi=&SPI);
  
  //______________________________________
  // true if busy
  bool isBusy();
  
  //______________________________________
  // timeout in milliseconds
  bool waitReady(int32_t timeout);
  
  //______________________________________
  uint8_t getManufactureId();
  
  //______________________________________
  uint8_t getType();
  
  //______________________________________
  uint8_t getCapacity();
  
  //______________________________________
  uint8_t byteRead(uint32_t addr);
  
  //______________________________________
  void bytesRead(uint32_t addr, uint8_t data[], uint32_t size);
  
  //______________________________________
  // Records of 16 bytes
  // Record 0 starts at address 0, record 1 starts at address 16 and so on
  void recordRead16(uint32_t record, uint8_t data[]);
  
  //______________________________________
  // Pages of 256 bytes
  // Page 0 starts at address 0, page 1 starts at address 256 and so on
  void pageRead256(uint16_t page, uint8_t data[]);

  //______________________________________
  void byteWrite(uint32_t addr, uint8_t data);
  
  //______________________________________
  // Records of 16 bytes
  // Record 0 starts at address 0, record 1 starts at address 16 and so on
  void recordWrite16(uint32_t record, uint8_t data[]);
  
  //______________________________________
  // Pages of 256 bytes
  // Page 0 starts at address 0, page 1 starts at address 256 and so on
  void pageWrite256(uint16_t page, uint8_t data[]);
  
  //______________________________________
  // Sectors of 4K bytes
  // Sector 0 starts at address 0, sector 1 starts at address 4096 and so on
  void sectorErase4K(uint16_t sector);
  
  //______________________________________
  // Blocks of 32K bytes
  // Block 0 starts at address 0, block 1 starts at address 32768 and so on
  void blockErase32K(uint8_t block);
  
  //______________________________________
  // Blocks of 64K bytes
  // Block 0 starts at address 0, block 1 starts at address 65536 and so on
  void blockErase64K(uint8_t block);
  
  //______________________________________
  void chipErase();

  //______________________________________
  private:
    int flash_CS;
    SPIClass *SPIi;
    uint8_t mid=0;
    uint8_t mty=0;
    uint8_t mca=0;
    void getJedecId();
    void writeEnable();
    void command(uint8_t cmd, uint32_t addr);
};

#endif
