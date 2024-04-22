


#include "pmod_core.h"

PmodCore::PmodCore(uint32_t core_base_addr){ //Constructor
	base_addr=core_base_addr;
}

PmodCore::~PmodCore(){} //Not used

uint32_t PmodCore::read(){
	return (io_read(base_addr,DATA_REG));
}

int PmodCore::read(int bit_pos){
	uint32_t rd_data = io_read(base_addr,DATA_REG);
	return ((int)bit_read(rd_data,bit_pos));
}


