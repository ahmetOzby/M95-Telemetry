#include "Magma_Typecast.h"


void Magma_UINT16toUINT8_LE(uint16_t *packData, uint8_t *packBuf, uint16_t *index)
{
	packBuf[*index] = ((uint8_t*)packData)[0];
	packBuf[(*index) + 1] = ((uint8_t*)packData)[1];

	*index = *index + sizeof(uint16_t);
}


void Magma_UINT16toUINT8_BE(uint16_t *packData, uint8_t *packBuf, uint16_t *index)
{
		packBuf[*index] = ((uint8_t*)packData)[1];
		packBuf[(*index) + 1] = ((uint8_t*)packData)[0];

		*index = *index + sizeof(uint16_t);
}

void Magma_INT16toUINT8_LE(int16_t *packData, uint8_t *packBuf, uint16_t *index)
{
	packBuf[*index] = ((uint8_t*)packData)[0];
	packBuf[(*index) + 1] = ((uint8_t*)packData)[1];

	*index = *index + sizeof(int16_t);
}


void Magma_INT16toUINT8_BE(int16_t *packData, uint8_t *packBuf, uint16_t *index)
{
		packBuf[*index] = ((uint8_t*)packData)[1];
		packBuf[(*index) + 1] = ((uint8_t*)packData)[0];

		*index = *index + sizeof(int16_t);
}

void Magma_UINT32toUINT8_LE(uint32_t *packData, uint8_t *packBuf, uint16_t *index)
{
	packBuf[*index] = ((uint8_t*)packData)[0];
	packBuf[(*index) + 1] = ((uint8_t*)packData)[1];
	packBuf[(*index) + 2] = ((uint8_t*)packData)[2];
	packBuf[(*index) + 3] = ((uint8_t*)packData)[3];

	*index = *index + sizeof(uint32_t);
}

void Magma_UINT32toUINT8_BE(uint32_t *packData, uint8_t* packBuf, uint16_t *index)
{
		packBuf[*index] = ((uint8_t*)packData)[3];
		packBuf[(*index) + 1] = ((uint8_t*)packData)[2];
		packBuf[(*index) + 2] = ((uint8_t*)packData)[1];
		packBuf[(*index) + 3] = ((uint8_t*)packData)[0];

		*index = *index + sizeof(uint32_t);
}

void Magma_INT32toUINT8_LE(int32_t *packData, uint8_t *packBuf, uint16_t *index)
{
	packBuf[*index] = ((uint8_t*)packData)[0];
	packBuf[(*index) + 1] = ((uint8_t*)packData)[1];
	packBuf[(*index) + 2] = ((uint8_t*)packData)[2];
	packBuf[(*index) + 3] = ((uint8_t*)packData)[3];

	*index = *index + sizeof(int32_t);
}

void Magma_INT32toUINT8_BE(int32_t *packData, uint8_t* packBuf, uint16_t *index)
{
		packBuf[*index] = ((uint8_t*)packData)[3];
		packBuf[(*index) + 1] = ((uint8_t*)packData)[2];
		packBuf[(*index) + 2] = ((uint8_t*)packData)[1];
		packBuf[(*index) + 3] = ((uint8_t*)packData)[0];

		*index = *index + sizeof(int32_t);
}

void Magma_FLOAT32toUINT8_LE(float *packData, uint8_t *packBuf, uint16_t *index)
{
	packBuf[*index] = ((uint8_t*)packData)[0];
	packBuf[(*index) + 1] = ((uint8_t*)packData)[1];
	packBuf[(*index) + 2] = ((uint8_t*)packData)[2];
	packBuf[(*index) + 3] = ((uint8_t*)packData)[3];

	*index = *index + sizeof(float);
}

void Magma_FLOAT32toUINT8_BE(float *packData, uint8_t *packBuf, uint16_t *index)
{
	packBuf[*index] = ((uint8_t*)packData)[3];
	packBuf[(*index) + 1] = ((uint8_t*)packData)[2];
	packBuf[(*index) + 2] = ((uint8_t*)packData)[1];
	packBuf[(*index) + 3] = ((uint8_t*)packData)[0];

	*index = *index + sizeof(float);
}

void Magma_FLOAT64toUINT8_LE(double *packData, uint8_t *packBuf, uint16_t *index)
{
	packBuf[*index] = ((uint8_t*)packData)[0];
	packBuf[(*index) + 1] = ((uint8_t*)packData)[1];
	packBuf[(*index) + 2] = ((uint8_t*)packData)[2];
	packBuf[(*index) + 3] = ((uint8_t*)packData)[3];
	packBuf[(*index) + 4] = ((uint8_t*)packData)[4];
	packBuf[(*index) + 5] = ((uint8_t*)packData)[5];
	packBuf[(*index) + 6] = ((uint8_t*)packData)[6];
	packBuf[(*index) + 7] = ((uint8_t*)packData)[7];

	*index = *index + sizeof(double);
}

void Magma_FLOAT64toUINT8_BE(double *packData, uint8_t *packBuf, uint16_t *index)
{
	packBuf[*index] = ((uint8_t*)packData)[7];
	packBuf[(*index) + 1] = ((uint8_t*)packData)[6];
	packBuf[(*index) + 2] = ((uint8_t*)packData)[5];
	packBuf[(*index) + 3] = ((uint8_t*)packData)[4];
	packBuf[(*index) + 4] = ((uint8_t*)packData)[3];
	packBuf[(*index) + 5] = ((uint8_t*)packData)[2];
	packBuf[(*index) + 6] = ((uint8_t*)packData)[1];
	packBuf[(*index) + 7] = ((uint8_t*)packData)[0];

	*index = *index + sizeof(double);
}

void Magma_UINT8toUINT8CODE(uint8_t *packData, uint8_t *packBuf, uint16_t *index)
{
	packBuf[*index] = ((uint8_t*)packData)[0];

	*index = *index + sizeof(uint8_t);
}

void Magma_INT8toUINT8CODE(int8_t *packData, uint8_t *packBuf, uint16_t *index)
{
	packBuf[*index] = ((uint8_t*)packData)[0];

		*index = *index + sizeof(int8_t);
}

void Magma_UINT8toINT16_LE(int16_t *packData, uint8_t* packBuf, uint16_t *index)
{
	((uint8_t *)packData)[0] = packBuf[*index];
	((uint8_t *)packData)[1] = packBuf[(*index) + 1];

	*index = *index + sizeof(int16_t);
}

void Magma_UINT8toINT16_BE(int16_t *packData, uint8_t* packBuf, uint16_t *index)
{
	((uint8_t *)packData)[1] = packBuf[*index];
	((uint8_t *)packData)[0] = packBuf[(*index) + 1];

	*index = *index + sizeof(int16_t);
}

void Magma_UINT8toUINT16_LE(uint16_t *packData, const uint8_t* packBuf, uint16_t *index)
{
	((uint8_t *)packData)[0] = packBuf[*index];
	((uint8_t *)packData)[1] = packBuf[(*index) + 1];

	*index = *index + sizeof(int16_t);
}

void Magma_UINT8toUINT16_BE(uint16_t *packData, const uint8_t* packBuf, uint16_t *index)
{
	((uint8_t *)packData)[1] = packBuf[*index];
	((uint8_t *)packData)[0] = packBuf[(*index) + 1];

	*index = *index + sizeof(int16_t);
}

void Magma_UINT8toINT32_LE(int32_t *packData, uint8_t* packBuf, uint16_t *index)
{
	((uint8_t *) packData)[0] = packBuf[*index];
	((uint8_t *) packData)[1] = packBuf[(*index) + 1];
	((uint8_t *) packData)[2] = packBuf[(*index) + 2];
	((uint8_t *) packData)[3] = packBuf[(*index) + 3];

	*index = *index + sizeof(int32_t);
}

void Magma_UINT8toINT32_BE(int32_t *packData, uint8_t* packBuf, uint16_t *index)
{
	((uint8_t *) packData)[3] = packBuf[*index];
	((uint8_t *) packData)[2] = packBuf[(*index) + 1];
	((uint8_t *) packData)[1] = packBuf[(*index) + 2];
	((uint8_t *) packData)[0] = packBuf[(*index) + 3];

	*index = *index + sizeof(int32_t);
}

void Magma_UINT8toUINT32_BE(uint32_t *packData, uint8_t *packBuf, uint16_t *index)
{
	((uint8_t*) packData)[3] = packBuf[(*index)];
	((uint8_t*) packData)[2] = packBuf[(*index) + 1];
	((uint8_t*) packData)[1] = packBuf[(*index) + 2];
	((uint8_t*) packData)[0] = packBuf[(*index) + 3];

	*index = *index + sizeof(uint32_t);
}

void Magma_UINT8toUINT32_LE(uint32_t *packData, uint8_t *packBuf, uint16_t *index)
{
	((uint8_t*) packData)[0] = packBuf[(*index)];
	((uint8_t*) packData)[1] = packBuf[(*index) + 1];
	((uint8_t*) packData)[2] = packBuf[(*index) + 2];
	((uint8_t*) packData)[3] = packBuf[(*index) + 3];

	*index = *index + sizeof(uint32_t);
}

void Magma_UINT8toUINT8ENCODE(uint8_t *packData, const uint8_t *packBuf, uint16_t *index)
{
	((uint8_t*) packData)[0] = packBuf[(*index)];

	*index = *index + sizeof(uint8_t);
}

void Magma_UINT8toINT8ENCODE(int8_t *packData, const uint8_t *packBuf, uint16_t *index)
{
	((uint8_t*) packData)[0] = packBuf[(*index)];

	*index = *index + sizeof(uint8_t);
}

void Magma_UINT8toFLOAT32_LE(float *packData, const uint8_t *packBuf, uint16_t *index)
{
	((uint8_t*) packData)[0] = packBuf[(*index)];
	((uint8_t*) packData)[1] = packBuf[(*index) + 1];
	((uint8_t*) packData)[2] = packBuf[(*index) + 2];
	((uint8_t*) packData)[3] = packBuf[(*index) + 3];

	*index = *index + sizeof(float);
}

void Magma_UINT8toFLOAT32_BE(float *packData, uint8_t *packBuf, uint16_t *index)
{
	((uint8_t*) packData)[3] = packBuf[(*index)];
	((uint8_t*) packData)[2] = packBuf[(*index) + 1];
	((uint8_t*) packData)[1] = packBuf[(*index) + 2];
	((uint8_t*) packData)[0] = packBuf[(*index) + 3];

	*index = *index + sizeof(float);
}

void Magma_UINT8toFLOAT64_LE(double *packData, uint8_t *packBuf, uint16_t *index)
{
	((uint8_t*) packData)[0] = packBuf[(*index)];
	((uint8_t*) packData)[1] = packBuf[(*index) + 1];
	((uint8_t*) packData)[2] = packBuf[(*index) + 2];
	((uint8_t*) packData)[3] = packBuf[(*index) + 3];
	((uint8_t*) packData)[4] = packBuf[(*index) + 4];
	((uint8_t*) packData)[5] = packBuf[(*index) + 5];
	((uint8_t*) packData)[6] = packBuf[(*index) + 6];
	((uint8_t*) packData)[7] = packBuf[(*index) + 7];
	*index = *index + sizeof(double);
}

void Magma_UINT8toFLOAT64_BE(double *packData, uint8_t *packBuf, uint16_t *index)
{
	((uint8_t*) packData)[7] = packBuf[(*index)];
	((uint8_t*) packData)[6] = packBuf[(*index) + 1];
	((uint8_t*) packData)[5] = packBuf[(*index) + 2];
	((uint8_t*) packData)[4] = packBuf[(*index) + 3];
	((uint8_t*) packData)[3] = packBuf[(*index) + 4];
	((uint8_t*) packData)[2] = packBuf[(*index) + 5];
	((uint8_t*) packData)[1] = packBuf[(*index) + 6];
	((uint8_t*) packData)[0] = packBuf[(*index) + 7];
	*index = *index + sizeof(double);
}

void Magma_UINT16toFLOAT_BE(float *packData, uint8_t *packBuf, uint16_t CONVERTER, uint16_t *index)
{
	 uint16_t data ;
	((uint8_t *)&data)[1] = packBuf[*index];
	((uint8_t *)&data)[0] = packBuf[(*index) + 1];

	*packData = (float)data / CONVERTER;
	*index = *index + sizeof(uint16_t);
}

void Magma_UINT16toFLOAT_LE(float *packData, const uint8_t *packBuf, uint16_t CONVERTER, uint16_t *index)
{
	 uint16_t data;
	((uint8_t *)&data)[0] = packBuf[*index];
	((uint8_t *)&data)[1] = packBuf[(*index) + 1];

	*packData = (float)data / CONVERTER;
	*index = *index + sizeof(uint16_t);
}

void Magma_INT16toFLOAT_BE(float *packData, uint8_t *packBuf, uint16_t CONVERTER, uint16_t *index)
{
	 int16_t data ;
	((uint8_t *)&data)[1] = packBuf[*index];
	((uint8_t *)&data)[0] = packBuf[(*index) + 1];

	*packData = (float)data / CONVERTER;
	*index = *index + sizeof(int16_t);
}

void Magma_INT16toFLOAT_LE(float *packData, const uint8_t *packBuf, uint16_t CONVERTER, uint16_t *index)
{
	 int16_t data;
	((uint8_t *)&data)[0] = packBuf[*index];
	((uint8_t *)&data)[1] = packBuf[(*index) + 1];

	*packData = (float)data / CONVERTER;

	*index = *index + sizeof(int16_t);
}



