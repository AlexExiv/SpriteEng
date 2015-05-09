//
//  FCAFFile.h
//  Amazing Balls
//
//  Created by Верун on 28.06.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#ifndef Amazing_Balls_FCAFFile_h
#define Amazing_Balls_FCAFFile_h


#include "FAudioFile.h"

class FCAFFile : public FAudioFile
{
public:
    FCAFFile( const FString & sFileName );
    ~FCAFFile();

};

#endif
