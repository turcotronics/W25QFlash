//Simple example with a simple library

#include "W25QFlash.h"

#define SPI_MOSI      0
#define SPI_SCLK      1
#define SPI_MISO      3
#define SPI_SPEED     4000000

#define BUFFERSIZE    256

W25QFlash myFlash;

uint32_t addr=0;
uint32_t page=0;
uint8_t myBuffer[BUFFERSIZE];

// You can use more flashes with different CS, in the same or in different SPI interfaces, just declare more instances and do the proper begin(...)
// Before write operations, those bytes must be already erased (All 0xFF); erase can be done only in sectors or blocks

//__________________________
void setup()
{
  Serial.begin();

  SPI.begin(SPI_SCLK, SPI_MISO, SPI_MOSI);
  SPI.setDataMode(0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setFrequency(SPI_SPEED);

  myFlash.begin(18, &SPI);// My Flash CS is on pin 18, put your one
  // same as myFlash.begin(18); 
  // or W25QLib myFlash.begin(18, &mySPI);// if you use a SPI different than the standard Arduino SPI or the same with different parameters, where mySPI is a SPIClass instance
   
  Serial.printf("\nFlash Manufacture Id: 0x%02X, ", myFlash.getManufactureId());
  Serial.printf("Type: 0x%02X, ", myFlash.getType());
  Serial.printf("Capacity: 0x%02X\n\n", myFlash.getCapacity());  
}

//__________________________
void loop()
{
  myFlash.bytesRead(addr, myBuffer, BUFFERSIZE);
  addr+=256;
  Serial.printf("Data starting at address %d: 0x%02X, 0x%02X, 0x%02X, ...\n", addr, myBuffer[0], myBuffer[1], myBuffer[2]);

  // Same as before but one page of 256 bytes at a time
  myFlash.pageRead256(page, myBuffer);
  page++;
  Serial.printf("Data starting at page %d: 0x%02X, 0x%02X, 0x%02X, ...\n", page, myBuffer[0], myBuffer[1], myBuffer[2]);

  // You may need to check if the flash is busy after Write or Erase operations, see you flash datasheet for timings
  // e.g. if(myFlash.waitReady(500)) ...
  // or if(myFlash.isBusy()) ...
  
  // Generally speeking read/write/erase in records/sectors/blocks is faster than the operation in the same amount of bytes one at a time

  delay(1000);
}
