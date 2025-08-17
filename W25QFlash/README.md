# W25QFlash library

https://github.com/turcotronics/W25QFlash

Version 1.0.0
MIT License

Simple library for Winbond W25Qxx flash that could work with other brands too
Tested on Winbond W25Q64JV
It's non blocking or blocking with Timeout

Rodolfo Turco (Turcotronics) <rodolfo.turco@turcotronics.com>
https://www.turcotronics.it
https://www.turcotronics.com

INSTALL:
From Arduino Sketch menu / Include Library / Add .ZIP Library...

USAGE:
Before write operations that bytes must be already erased (All 0xFF), erase can be done only in sectors or blocks

//______________________________________
// Costructor
W25QFlash();

//______________________________________
// To be called in setup() e.g. myFlash.begin(18); or myFlash.begin(18, &mySPI);
void begin(int flash_CS, SPIClass *SPIint=&SPI);

//______________________________________
// return true if flash is busy
bool isBusy();

//______________________________________
// timeout in milliseconds, return when flash is ready returning true, or after timeout returning false
bool waitReadyOk(int32_t timeout);

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
//Erase the whole chip, may take a while, read your datasheet
void chipErase();
