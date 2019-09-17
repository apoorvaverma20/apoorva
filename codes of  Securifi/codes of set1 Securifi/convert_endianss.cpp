#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

//Function to chnge the endianess
uint32_t ChangeEndianness(uint32_t Value)
{
uint32_t Result = 0;

Result |= (Value & 0x000000FF) << 24;
Result |= (Value & 0x0000FF00) << 8;
Result |= (Value & 0x00FF0000) >> 8;
Result |= (Value & 0xFF000000) >> 24;


return Result;
}

int main()
{

uint32_t CheckData  = 0x11223344; //uint32_t is same as unsigned int
uint32_t ResultData =0;

ResultData = ChangeEndianness(CheckData);  //swap the data

printf("0x%x\n",ResultData);

CheckData = ResultData;

ResultData = ChangeEndianness(CheckData);//again swap the data

printf("0x%x\n",ResultData);

return 0;

}

