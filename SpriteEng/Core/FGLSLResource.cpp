#include "FGLSLResource.h"
#include "FLog.h"
#include "FException.h"
#include "FLoopAllocator.h"
#include <string.h>
#include "FStack.h"



const FString sGLSLExt( "shr" );

///символы дл€ слов
const CHAR_ cWordSym [] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '_' };

//символы дл€ чисел
const CHAR_ cNumberSym [] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.' };

//типы токенов
#define TOKEN_WORD 1 //слово
#define TOKEN_NUMBER 2 //число
#define TOKEN_SQBRACK0 3 //квадратна€ скобка [
#define TOKEN_SQBRACK1 4 //квадратна€ скобка ]
#define TOKEN_EOF 5 //конец файла
#define TOKEN_STRING 6 //строка
#define TOKEN_EOL 7 //конец строки
#define TOKEN_SUBTRACT 8 //тире
#define TOKEN_EQUAL 9//равно
#define TOKEN_SEMICOLON 10//точка с зап€той

///типы шейдерных программ
#define VERTEX_PROGRAM_ 1 //вершинный шейдер
#define FRAGMENT_PROGRAM_ 2 //фрагментный шейдер
#define GEOMETRIC_PROGRAM_ 3 //геометрический шейдер
#define TESSELATE_PROGRAM_ 4//тесс€л€ционый шейдер
#define BLOCK_PROGRAM_ 5 //блок расстановки текстур
#define UNKNOWN_PROGRAM 0xFFFF//неизвестна€ программа

struct FProgramType
{
	FString sName;
	UI32 iProgType;
};

const FProgramType pProgramType[] = 
{
	{ "VERTEX_PROGRAM", VERTEX_PROGRAM_ },
	{ "FRAGMENT_PROGRAM", FRAGMENT_PROGRAM_ },
	{ "GEOMETRIC_PROGRAM", GEOMETRIC_PROGRAM_ },
	{ "TESSELATE_PROGRAM", TESSELATE_PROGRAM_ },
	{ "BLOCK_PROGRAM", BLOCK_PROGRAM_ }
};

struct FProgramMap
{
	UI32 iProgram;
	UI32 iResProgram;
};

const FProgramMap pProgramMap[] =
{
	{ VERTEX_PROGRAM_, FGLSLResource::VERTEX_PROGRAM },
	{ FRAGMENT_PROGRAM_, FGLSLResource::FRAGMENT_PROGRAM },
	{ GEOMETRIC_PROGRAM_, FGLSLResource::GEOMETRIC_PROGRAM },
	{ TESSELATE_PROGRAM_, FGLSLResource::TESSELATE_PROGRAM }
};

UI32 ProgramRemap( UI32 iProg )
{
	for( UI32 i = 0;i < ARRAY_SIZE( pProgramMap );i++ )
	{
		if( pProgramMap[i].iProgram == iProg )
			return pProgramMap[i].iResProgram;
	}

	return UNKNOWN_PROGRAM;
}

///проверка €вл€етс€ ли символ символом слова
bool IsWordSym( CHAR_ cChar )
{
	for( I32 i = 0;i < ARRAY_SIZE( cWordSym );i++ )
	{
		if( cWordSym[i] == cChar )
			return true;
	}

	return false;
}

//проверка €вл€етс€ ли символ символом числа
bool IsNumberSym( CHAR_ cChar )
{
	for( I32 i = 0;i < ARRAY_SIZE( cNumberSym );i++ )
	{
		if( cNumberSym[i] == cChar )
			return true;
	}

	return false;
}

//прогпустить пробелы и табул€цию
UI32 SkipSpaces( CHAR_ * lpStr )
{
	UI32 iInd = 0;
	while( (lpStr[iInd] == ' ') || (lpStr[iInd] == '\t') )
		iInd++;

	return iInd;
}

//пропустить пустые строки
UI32 SkipRet( CHAR_ * lpStr )
{
	UI32 iInd = 0;
	while( (lpStr[iInd] == '\r') || (lpStr[iInd] == '\n') )
		iInd++;

	return iInd;
}

//пропустить пустые строки, пробелы и табул€цию
UI32 SkipSpacesRet( CHAR_ * lpStr )
{
	UI32 iInd = 0;
	while( (lpStr[iInd] == '\r') || (lpStr[iInd] == ' ') || (lpStr[iInd] == '\t') || (lpStr[iInd] == '\n') )
		iInd++;

	return iInd;
}

//перевод им€ опции конфига во внутрений код
UI32 GetProgramType( const FString & sName )
{
	for( I32 i = 0;i < ARRAY_SIZE( pProgramType );i++ )
	{
		if( pProgramType[i].sName == sName )
			return pProgramType[i].iProgType;
	}

	return UNKNOWN_PROGRAM;
}

bool CheckNumber( CHAR_ * lpStr )
{
	UI32 iInd = 0;
	bool bSep = false;

	while( lpStr[iInd] != 0 )
	{
		if( !IsNumberSym( lpStr[iInd] ) )
			return false;
		if( lpStr[iInd] == '.' )
		{
			if( bSep )
				return false;
			bSep = true;
		}
		iInd++;
	}

	return true;
}

//получить тип константы целое число, с плавающей точкой или строка
UI32 GetTokenType( const CHAR_ * lpStr )
{
	if( IsNumberSym( *lpStr ) )
	{
		while( *lpStr != 0 )
		{
			if( *lpStr == '.' )
				return FGLSLResource::PROGRAM_TYPE_FLOAT;
			lpStr++;
		}
		return FGLSLResource::PROGRAM_TYPE_INT;
	}

	return FGLSLResource::PROGRAM_TYPE_STR;
}

//получить следущий токен вход строка, на выходе токен в буфере, его код и последний индекс символа
UI32 GetToken( CHAR_ * lpStr, CHAR_ * lpBuffer, UI32 & iTokenType )
{
	UI32 iInd = SkipSpaces( lpStr );//пропускаем пробелы

	
	if( IsNumberSym( lpStr[iInd] ) )//если первый символ €вл€етс€ символом числа то токен €вл€етс€ числом
		iTokenType = TOKEN_NUMBER;
	else if( IsWordSym( lpStr[iInd] ) )//если первый токен €вл€етс€ символом слова то токен €вл€етс€ словом
		iTokenType = TOKEN_WORD;
	else
	{
		UI32 i = 0;
		lpBuffer[0] = lpStr[iInd];
		lpBuffer[1] = 0;
		switch( lpStr[iInd] )
		{
		case '[':
			iTokenType = TOKEN_SQBRACK0;
			return iInd + 1;
		case ']':
			iTokenType = TOKEN_SQBRACK1;
			return iInd + 1;
		case 0:
			iTokenType = TOKEN_EOF;
			return iInd;
		case '"'://токен €вл€етс€ строкой
			iTokenType = TOKEN_STRING;
			iInd++;
			while( (lpStr[iInd] != 0)&&(lpStr[iInd] != '\r')&&(lpStr[iInd] != '"') )
				lpBuffer[i++] = lpStr[iInd++];
			lpBuffer[i] = 0;
			if( lpStr[iInd] == '"' )
			{
				iInd++;
				return iInd;
			}

			FLog::PutError( "Unexpected end of string: %s", lpBuffer );
			iTokenType = 0;
			return iInd;
		case '\r':
			iTokenType = TOKEN_EOL;
			return iInd + 1;
		case '-':
			iTokenType = TOKEN_SUBTRACT;
			return iInd + 1;
		case '=':
			iTokenType = TOKEN_EQUAL;
			return iInd + 1;
		case ';':
			iTokenType = TOKEN_SEMICOLON;
			return iInd + 1;
		default:
			iTokenType = 0;
			FLog::PutError( "Unnown symbol: \"%c\"", lpStr[iInd] );
			return 0;
		}
	}

	//копируем символы м буффер
	UI32 i = 0;
	while( IsNumberSym( lpStr[iInd] )||IsWordSym( lpStr[iInd] ) )
		lpBuffer[i++] = lpStr[iInd++];
	lpBuffer[i] = 0;

	if( iTokenType == TOKEN_NUMBER )//если токен число то провер€ем правильность записи
	{
		if( !CheckNumber( lpBuffer ) )
		{
			FLog::PutError( "Impossible characters including in number: %s", lpBuffer );
			iTokenType = 0;
		}
	}

	return iInd;
}

I32 TextRollback( CHAR_ * lpText, CHAR_ * lpStartPoint )
{
	I32 iInd = 0;
	while( (lpText[iInd] != '\n') && ((lpText + iInd) != lpStartPoint) )
	{
		iInd--;
	}

	return iInd;
}

FGLSLResource::FGLSLResource() : FResource( sGLSLExt, FString( "Shader" ) )
{
}

FGLSLResource::FGLSLResource( void * lpData0, UI32 iDataLen, FResourceManager * lpCreator ) : FResource( sGLSLExt, lpCreator )
{
	bRemoveData = false;

	lpData = lpData0;
	CHAR_ * lpText = (CHAR_ *)PUSH_BLOCKT( iDataLen + 1 );
	memcpy( lpText, lpData0, iDataLen );
	lpText[iDataLen] = 0;
	UI32 iTokenType = 0, iLen = 0;
	CHAR_ cBuffer[256];

	try
	{
		for( UI32 i = 0;i < ARRAY_SIZE( pProgramType );i++ )
		{
			CHAR_ * lpStr = strstr( lpText, pProgramType[i].sName.GetChar() );
			if( lpStr == NULL )
				continue;
			lpStr += TextRollback( lpStr, lpText );
			lpStr += SkipSpacesRet( lpStr );

			lpStr += GetToken( lpStr, cBuffer, iTokenType );
			if( iTokenType != TOKEN_SQBRACK0 )
				throw FException( FException::EXCP_UNK_FILE_FORMAT, FString::PrintString( "Unexpected token \"%s\"", cBuffer ) );

			lpStr += GetToken( lpStr, cBuffer, iTokenType );
			if( iTokenType != TOKEN_WORD )
				throw FException( FException::EXCP_UNK_FILE_FORMAT, FString::PrintString( "Unexpected token, must be word \"%s\"", cBuffer ) );

			UI32 iProgType = GetProgramType( cBuffer );
			if( iProgType != pProgramType[i].iProgType )
				throw FException( FException::EXCP_UNK_FILE_FORMAT, FString::PrintString( "Unknown shader program type \"%s\"", cBuffer ) );

			lpStr += GetToken( lpStr, cBuffer, iTokenType );
			if( iTokenType != TOKEN_SQBRACK1 )
				throw FException( FException::EXCP_UNK_FILE_FORMAT, FString::PrintString( "Unexpected token \"%s\"", cBuffer ) );

			lpStr += GetToken( lpStr, cBuffer, iTokenType );
			if( iTokenType != TOKEN_EOL )
				throw FException( FException::EXCP_UNK_FILE_FORMAT, FString::PrintString( "Shader program must start from new line" ) );

			iLen = 0;
			while( true )
			{
				while( lpStr[iLen] != '[' && lpStr[iLen] != 0 )
					iLen++;
				if( lpStr[iLen] == 0 )
					break;

				UI32 iInd = GetToken( lpStr + iLen + 1, cBuffer, iTokenType );
				if( iTokenType == TOKEN_WORD )
				{
					UI32 iProgType = GetProgramType( cBuffer );
					if( iProgType != UNKNOWN_PROGRAM )
						break;
				}
				iLen += iInd;
			}
			//iLen--;
			if( pProgramType[i].iProgType == BLOCK_PROGRAM_ )
			{
				ParseBlocks( lpStr, iLen );
			}
			else
			{
				UI32 iProg = ProgramRemap( pProgramType[i].iProgType );
				pProgram[iProg].lpProgram = lpStr;
				pProgram[iProg].iLen = iLen;
			}
		}
	}
	catch( FException eExcp )
	{
		POP_BLOCK;
		throw eExcp;
	}
	POP_BLOCK;
}

FGLSLResource::~FGLSLResource()
{
}
/*
каждое присваивание блока имеет вид tTextureMap = [0..N]
*/
UI32 FGLSLResource::ParseBlocks( CHAR_ * lpText, UI32 iLen )
{
	UI32 iInd = 0, iTokenType;
	CHAR_ cBuffer[256];

	while( iInd < iLen )
	{
		iInd += SkipSpacesRet( &lpText[iInd] );

		iInd += GetToken( &lpText[iInd], cBuffer, iTokenType );
		if( iTokenType != TOKEN_WORD )
			throw FException( FException::EXCP_UNK_FILE_FORMAT, FString::PrintString( "Unexpected token, must be word: \"%s\"", cBuffer ) );

		FString sTexName( cBuffer );
		iInd += GetToken( &lpText[iInd], cBuffer, iTokenType );
		if( iTokenType != TOKEN_EQUAL )
			throw FException( FException::EXCP_UNK_FILE_FORMAT, FString::PrintString( "Unexpected token, must be =: \"%s\"", cBuffer ) );

		iInd += GetToken( &lpText[iInd], cBuffer, iTokenType );
		if( iTokenType != TOKEN_NUMBER )
			throw FException( FException::EXCP_UNK_FILE_FORMAT, FString::PrintString( "Unexpected token, must be =: \"%s\"", cBuffer ) );
		if( GetTokenType( cBuffer ) != PROGRAM_TYPE_INT )
			throw FException( FException::EXCP_UNK_FILE_FORMAT, FString::PrintString( "Number must be integer: \"%s\"", cBuffer ) );
		I32 iBlock = atoi( cBuffer );
		if( iBlock < 0 )
			throw FException( FException::EXCP_UNK_FILE_FORMAT, FString::PrintString( "Number must be positive integer: \"%s\"", cBuffer ) );

		FTextureBlock * lpTexBlock = (FTextureBlock *)ALLOC_LOOPT( sizeof( FTextureBlock ) );
		lpTexBlock->iBlock = iBlock;
		lpTexBlock->sTexName = sTexName;
		aTextureArray.Add( lpTexBlock );

		iInd += SkipSpacesRet( &lpText[iInd] );
	}

	return iInd;
}

CHAR_ * FGLSLResource::GetVertexProgram()const
{
	return pProgram[VERTEX_PROGRAM].lpProgram;
}

CHAR_ * FGLSLResource::GetFragmentProgram()const
{
	return pProgram[FRAGMENT_PROGRAM].lpProgram;
}

CHAR_ * FGLSLResource::GetGeometricProgram()const
{
	return pProgram[GEOMETRIC_PROGRAM].lpProgram;
}

CHAR_ * FGLSLResource::GetTesselateProgram()const
{
	return pProgram[TESSELATE_PROGRAM].lpProgram;
}

UI32 FGLSLResource::GetVertexProgramLen()const
{
	return pProgram[VERTEX_PROGRAM].iLen;
}

UI32 FGLSLResource::GetFragmentProgramLen()const
{
	return pProgram[FRAGMENT_PROGRAM].iLen;
}

UI32 FGLSLResource::GetGeometricProgramLen()const
{
	return pProgram[GEOMETRIC_PROGRAM].iLen;
}

UI32 FGLSLResource::GetTesselateProgramLen()const
{
	return pProgram[TESSELATE_PROGRAM].iLen;
}

UI32 FGLSLResource::GetBlockCount()const
{
	return aTextureArray.GetCount();
}

void FGLSLResource::GetBlockParams( UI32 iIndex, FString & sTexName, UI32 & iBlock )const
{
	sTexName = aTextureArray[iIndex]->sTexName;
	iBlock = aTextureArray[iIndex]->iBlock;
}

FResource * FGLSLResource::Make( void * lpPlacement, void * lpData, UI32 iDataLen, FResourceManager * lpCreator ) 
{
	return new (lpPlacement) FGLSLResource( lpData, iDataLen, lpCreator );
}

UI32 FGLSLResource::GetSize()const
{
	return sizeof( FGLSLResource );
}


void FGLSLResource::SaveResource( void ** lpData, UI32 & iDataSize )
{

}
