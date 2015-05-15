#include "FWAVResource.h"


const FString sWav( "wav" );


struct FWAVHeader
{
	CHAR_ cChunkID[4];//������ ���� ����� RIFF
	UI32 iChunkSize;//������ ���������� ������ ������� �� ���� ����
	CHAR_ cFormat[4];//������ ���� WAVE
	CHAR_ cSubChunkID[4];//����� ���� fmt
	UI32 iSubChunkSize;//������ ���������� ������ ������� �� ���� ����
	UI16 iAudioFmt;//����� ������, ���� ��� ������ ����� 1
	UI16 iNumChan;//���-�� �������
	UI32 iSampleRate;//������� �������������
	UI32 iBytesRate;//���-�� ���� �� ������� ���������������
	UI16 iBytePerSample;//���-�� ���� �� �����
	UI16 iBitsPerSample;//���-�� ��� �� �����
	CHAR_ cSubChunkID2[4];//������ ���� ����� data
	UI32 iSubChunkSize2;//������ ������ wav
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
