//
//  FCAFFile.cpp
//  Amazing Balls
//
//  Created by Верун on 28.06.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#include "FCAFFile.h"
#import <AudioToolbox/AudioToolbox.h>
#import <UIKit/UIKit.h>

FCAFFile::FCAFFile( const FString & sFileName )
{
    NSString * lpName, * lpFullName;
    if( IsMainBundle( sFileName ) )
    {
        FString sFile = sFileName - BUNDLE_NAME;
        lpName = [NSString stringWithFormat: @"%s", sFile.GetChar()];
        lpFullName = [[NSBundle mainBundle] pathForResource :lpName ofType:nil];
    }
    else
    {
        NSArray * path = NSSearchPathForDirectoriesInDomains( NSDocumentDirectory, NSUserDomainMask, YES );
        NSString * doc = [path objectAtIndex:0];
        lpName = [NSString stringWithFormat:@"%s", sFileName.GetChar()];
        lpFullName = [doc stringByAppendingPathComponent:lpName];
    }

    AudioFileID aFileID = 0;
    NSURL * lpUrl = [NSURL fileURLWithPath :lpFullName];
    OSStatus oResult = AudioFileOpenURL( (__bridge CFURLRef)lpUrl, kAudioFileReadPermission, 0, &aFileID );
    if( oResult != 0 )
        return;
    
    UInt32 iPropSize = sizeof( UInt64 );
    AudioFileGetProperty( aFileID, kAudioFilePropertyAudioDataByteCount, &iPropSize, &iDataSize );
    AudioStreamBasicDescription aFormat;
    iPropSize = sizeof( aFormat );
    AudioFileGetProperty( aFileID, kAudioFilePropertyDataFormat, &iPropSize, &aFormat );
    iByteRate = aFormat.mBytesPerFrame;
    iBits = aFormat.mBitsPerChannel;
    iNumChannels = aFormat.mChannelsPerFrame;
    iSampleRate = aFormat.mSampleRate;
    iDataFormat = GetDataFormat( iNumChannels, iBits );
    
    lpData = malloc( iDataSize );
    AudioFileReadBytes( aFileID, false, 0, (UInt32 *)&iDataSize, lpData );
    
    AudioFileClose( aFileID );
}

FCAFFile::~FCAFFile()
{
    
}