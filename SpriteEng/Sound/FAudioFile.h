//
//  FAudioFile.h
//  Amazing Balls
//
//  Created by Верун on 28.06.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#ifndef Amazing_Balls_FAudioFile_h
#define Amazing_Balls_FAudioFile_h

#include "FString.h"

class FAudioFile
{
protected:
    UI32 iSecLen, iSampleRate, iNumChannels, iByteRate, iBits, iDataSize, iDataFormat;
    void * lpData;
    
public:
    FAudioFile();
    virtual ~FAudioFile();
    
    UI32 GetSampleRate()const ;
    UI32 GetNumChannels()const ;
    UI32 GetByteRate()const ;
    UI32 GetBitsPerSample()const ;
    UI32 GetDataSize()const ;
    UI32 GetFileLenInSec()const;
    UI32 GetDataFormat()const;
    void * GetData();
    
    static FAudioFile * CreateFile( const FString & sFileName );
    static UI32 GetDataFormat( UI32 iNumChannels, UI32 iBits );
};

#endif
