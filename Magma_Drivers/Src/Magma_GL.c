/*
 * Magma_GL.c
 *
 *  Created on: Sep 16, 2021
 *      Author: Ahmet
 */
#include "Magma_GL.h"
#include "math.h"


uint8_t Magma_Checksum_Handler(uint8_t *data, uint8_t len)
{
	uint64_t sum = 0;

	for(uint8_t i = 0; i < len; i++)
	{
		sum += data[i];
	}

	return (sum & 0xff);
}

void vars_to_str(char *buffer, const char *format, ...)
{
	va_list args;
	va_start (args, format);
	vsprintf (buffer, format, args);
	va_end (args);
}

// Reverses a string 'str' of length 'len'
void reverse(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

// Converts a given integer x to string str[].
// d is the number of digits required in the output.
// If d is more than the number of digits in x,
// then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}

// Converts a floating-point/double number to a string.
void ftoa(float n, char* res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;

    // Extract floating part
    float fpart = n - (float)ipart;

    // convert integer part to string
    int i = intToStr(ipart, res, 1);

    // check for display option after point
    if (afterpoint != 0) {
        res[i] = '.'; // add dot

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter
        // is needed to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);

        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}

void HAL_SYSTICK_Callback(void)
{
	static uint64_t ctr = 0;
	ctr++;

	if(ctr % 1 == 0)
	{
		sys_timer._1000_hz = TRUE;
	}

	if(ctr % 10 == 0)
	{
		sys_timer._100_hz = TRUE;
	}

	if(ctr % 100 == 0)
	{
		sys_timer._10_hz = TRUE;
	}

	if(ctr % 200 == 0)
	{
		sys_timer._5_hz = TRUE;
	}

	if(ctr % 500 == 0)
	{
		sys_timer._2_hz = TRUE;
	}

	if(ctr % 1000 == 0)
	{
		sys_timer._1_hz = TRUE;
	}

	if(ctr % 10000 == 0)
	{
		sys_timer._0_1_hz = TRUE;
	}
}

uint16_t Find_String_Index(char *find_this, char* find_from_this)
{
    uint16_t pos_search = 0;
    uint16_t pos_text = 0;
    uint16_t len_search = strlen(find_this);
    uint16_t len_text = strlen(find_from_this);
    for (pos_text = 0; pos_text < len_text - len_search;++pos_text)
    {

        if(find_from_this[pos_text] == find_this[pos_search])
        {
            ++pos_search;
            if(pos_search == len_search)
            {
            	return(pos_text);
            }
        }

        else
        {
           pos_text -=pos_search;
           pos_search = 0;
        }
    }
    return 0;
}


/*void MY_FLASH_EraseSector(void)
{
  HAL_FLASH_Unlock();
//Erase the required Flash sector
  FLASH_Erase_Sector(MY_SectorNum, FLASH_VOLTAGE_RANGE_3);
  HAL_FLASH_Lock();
}

//2. Set Sector Adress
void MY_FLASH_SetSectorAddrs(uint8_t sector, uint32_t addrs)
{
  MY_SectorNum = sector;
  MY_SectorAddrs = addrs;
}

//3. Write Flash
void MY_FLASH_WriteN(uint32_t idx, void *wrBuf, uint32_t Nsize, DataTypeDef dataType)
{
  uint32_t flashAddress = MY_SectorAddrs + idx;

//Erase sector before write
  MY_FLASH_EraseSector();

//Unlock Flash
  HAL_FLASH_Unlock();
//Write to Flash
  switch (dataType)
  {
  case DATA_TYPE_8:
    for (uint32_t i = 0; i < Nsize; i++)
    {
      HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, flashAddress, ((uint8_t*) wrBuf)[i]);
      flashAddress++;
    }
    break;

  case DATA_TYPE_16:
    for (uint32_t i = 0; i < Nsize; i++)
    {
      HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, flashAddress, ((uint16_t*) wrBuf)[i]);
      flashAddress += 2;
    }
    break;

  case DATA_TYPE_32:
    for (uint32_t i = 0; i < Nsize; i++)
    {
      HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flashAddress, ((uint32_t*) wrBuf)[i]);
      flashAddress += 4;
    }
    break;
  }
//Lock the Flash space
  HAL_FLASH_Lock();
}

//4. Read Flash
void MY_FLASH_ReadN(uint32_t idx, void *rdBuf, uint32_t Nsize, DataTypeDef dataType)
{
  uint32_t flashAddress = MY_SectorAddrs + idx;

  switch (dataType)
  {
  case DATA_TYPE_8:
    for (uint32_t i = 0; i < Nsize; i++)
    {
      *((uint8_t*) rdBuf + i) = *(uint8_t*) flashAddress;
      flashAddress++;
    }
    break;

  case DATA_TYPE_16:
    for (uint32_t i = 0; i < Nsize; i++)
    {
      *((uint16_t*) rdBuf + i) = *(uint16_t*) flashAddress;
      flashAddress += 2;
    }
    break;

  case DATA_TYPE_32:
    for (uint32_t i = 0; i < Nsize; i++)
    {
      *((uint32_t*) rdBuf + i) = *(uint32_t*) flashAddress;
      flashAddress += 4;
    }
    break;
  }
}*/

