#include <stdint.h>
#include <moduleLoader.h>

static const uint64_t PageSize = 0x400000;
extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;


typedef int (*EntryPoint)(int argc, char *argv[]);
/*
static void * const sampleCodeModuleAddress = (void*)0x800000;
static void * const sampleDataModuleAddress = (void*)0x600000;
static void * const shellAddress = (void*)0x400000;
*/
static void * const executableMemoryAdress = (void*)0xA00000;

char* moduleNames[] = {"shell", "sampleDataModule", "sampleCodeModule", 0};
void * moduleAddresses[] = {0x400000, 0x600000, 0x800000};


void copyAndExectueDefaultModule(){
	memcpy(executableMemoryAdress, moduleAddresses[0], 0x20000);
	((EntryPoint)executableMemoryAdress)(0,0);


}
void copyAndExecuteModule(int moduleIndex, int argc, char *argv[]){
	memcpy(executableMemoryAdress, moduleAddresses[moduleIndex], 0x200000);
	((EntryPoint)executableMemoryAdress)(argc, argv);
}

void setKernelPresent(int present){
  uint64_t *PD = 0x10000; 
  uint64_t entry= *PD; 
  if(present) 
    *PD = entry | 0x1; 
  else 
    *PD = entry & ~0x1; 
}

void changePDEPresent(int entry, int present){
	uint64_t* PD = 0x10000;

	while(entry){ 
    PD + 0x8; 
    --entry; 
  	} 
 	uint64_t PDE = *PD;
 
 	if(present)
  		*PD =  PDE | 0x1; 
  	else
  		*PD = PDE & ~0x1;  
}

void changePDE(int entry, uint64_t* physAddr, int present){ 
  if(*physAddr & 0x000FFFFF != 0) 
    return; 
 
  
  uint64_t *PD = 0x10000; 
 
  while(entry){ 
    PD + 0x4; 
    --entry; 
  } 

  uint64_t oldEntry = *PD;
 
  if(present) 
    *PD = oldEntry & 0xFFFFF | (uint64_t)physAddr & ~0xFFFFF | 0x1; 
  else 
    *PD = oldEntry & 0xFFFFF | (uint64_t)physAddr & ~0xFFFFF & ~0x1; 
} 

void pageFaultHandler(){ 
  copyAndExectueDefaultModule();
} 

void clearBSS(void * bssAddress, uint64_t bssSize)
{
  memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
  return (void*)(
    (uint64_t)&endOfKernel
    + PageSize * 8        //The size of the stack itself, 32KiB
    - sizeof(uint64_t)      //Begin at the top of the stack
  );
}

void * initializeKernelBinary()
{

  loadModules(&endOfKernelBinary, moduleAddresses);
  clearBSS(&bss, &endOfKernel - &bss);
  return getStackBase();
}
