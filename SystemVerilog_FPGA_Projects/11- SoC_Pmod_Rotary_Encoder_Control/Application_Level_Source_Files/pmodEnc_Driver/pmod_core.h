







#pragma once

#include "chu_init.h"

//=================================== pmod class =============================================
class PmodCore
{
public:
	enum{
		DATA_REG =0
	};
	PmodCore(uint32_t core_base_addr);
	~PmodCore();

	// -------------------- 2 functions to handle the pmod
	uint32_t read();

	int read(int bit_pos);

private:
	uint32_t base_addr;

};
