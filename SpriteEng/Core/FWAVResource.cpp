#include "FWAVResource.h"


const FString sWav( "wav" );


struct FWAVHeader
{
	CHAR_ cChunkID[4];//должно быть равно RIFF
	UI32 iChunkSize;//размер оставшихся данных начиная со след поля
	CHAR_ cFormat[4];//должно быть WAVE
	CHAR_ cSubChunkID[4];//дожно быть fmt
	UI32 iSubChunkSize;//размер оставшихся данных начиная со след поля
	UI16 iAudioFmt;//аудио формат, если без сжатия равно 1
	UI16 iNumChan;//кол-во каналов
	UI32 iSampleRate;//частота дискретизации
	UI32 iBytesRate;//кол-во байт на секунду воспроизведения
	UI16 iBytePerSample;//кол-во байт на семпл
	UI16 iBitsPerSample;//кол-во бит на семпл
	CHAR_ cSubChunkID2[4];//должно быть равно data
	UI32 iSubChunkSize2;//размер данных wav
};

FWAVResource::FWAVResource( void * lpData, UI32 iDataLen, FResourceManager * lpCreator ) : FResource( sWav, lpCreator )
{
	FWAVHeader * lpHeader = (FWAVHeader *)lpHeader;
}

FWAVResource::~FWAVResource()
{
}

FResource * FWAVResource::Make( void * lpData, UI32 iDataLen, FResourceManager * lpCreator )
{
	return new FWAVResource( lpData, iDataLen, lpCreator );
}

void FWAVResource::SaveResource( void ** lpData, UI32 & iDataSize )
{
}

F32 FWAVResource::GetLen()const
{
	return 0;
}

UI32 FWAVResource::GetSampleRate()const
{
	return 0;
}

UI32 FWAVResource::GetNumChan()const
{
	return 0;
}

UI32 FWAVResource::GetBitsPerSample()const
{
	return 0;
}
