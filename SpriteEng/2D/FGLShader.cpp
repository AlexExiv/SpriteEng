#include "FGLShader.h"

#ifndef MOBILE_DEVICE

#include "libExt.h"
#include "..\Core\FLog.h"
#include "..\Core\FGLSLResource.h"
#include "..\Core\FException.h"
#include "..\Core\FResourceManager.h"



FGLShader::FGLShader( const FString & sName ) : FShader( sName ), iProgramID( 0 ), iVertexID( 0 ), iFragmentID( 0 ), iCurAttrib( 0 )
{
	iVertexID = glCreateShaderObject( GL_VERTEX_SHADER );
	iFragmentID = glCreateShaderObject( GL_FRAGMENT_SHADER );
	iProgramID = glCreateProgramObject();

	FGLSLResource * lpRes = NULL;
	try
	{
		lpRes = (FGLSLResource *)FResourceManager::SharedManager()->CreateResource( sName );
		if( !lpRes )
			throw FException( FException::EXCP_FILE_NOT_FOUND, FString::PrintString( "Can't create shader object, program file not exist or corrupted: \"%s\"", sName.GetChar()) );

		GLcharARB * lpPrograms[] = { lpRes->GetVertexProgram(), lpRes->GetFragmentProgram() };
		GLint iLens[] = { lpRes->GetVertexProgramLen(), lpRes->GetFragmentProgramLen() };
		CHAR_ cBuffer[MAX_BUFFER];
		GLint iVertCompile = 0, iFragCompile = 0, iLinkProg = 0;

		glShaderSource( iVertexID, 1, (const GLcharARB **)&lpPrograms[0], &iLens[0] );
		glShaderSource( iFragmentID, 1, (const GLcharARB **)&lpPrograms[1], &iLens[1] );

		glCompileShader( iVertexID );
		glCompileShader( iFragmentID );

		glGetObjectParameteriv( iVertexID, GL_OBJECT_COMPILE_STATUS_ARB, &iVertCompile );
		glGetObjectParameteriv( iFragmentID, GL_OBJECT_COMPILE_STATUS_ARB, &iFragCompile );

		if( iVertCompile == 0 )
		{
			glGetInfoLog( iVertexID, MAX_BUFFER, NULL, cBuffer );
			cBuffer[MAX_BUFFER - 1] = 0;
			throw FException( FException::EXCP_UNK_FILE_FORMAT, FString( cBuffer ) );
		}

		if( iFragCompile == 0 )
		{
			glGetInfoLog( iFragmentID, MAX_BUFFER, NULL, cBuffer );
			cBuffer[MAX_BUFFER - 1] = 0;
			throw FException( FException::EXCP_UNK_FILE_FORMAT, FString( cBuffer ) );
		}

		glAttachObject( iProgramID, iVertexID );
		glAttachObject( iProgramID, iFragmentID );
		glLinkProgram( iProgramID );
		glGetObjectParameteriv( iProgramID, GL_OBJECT_LINK_STATUS_ARB, &iLinkProg );
		if( iLinkProg == 0 )
		{
			glGetInfoLog( iProgramID, MAX_BUFFER, NULL, cBuffer );
			cBuffer[MAX_BUFFER - 1] = 0;
			throw FException( FException::EXCP_UNK_FILE_FORMAT, FString( cBuffer ) );
		}
		
		for( UI32 i = 0;i < lpRes->GetBlockCount();i++ )
		{
			FString sTexName;
			UI32 iBlock;
			lpRes->GetBlockParams( i, sTexName, iBlock );
			FTextureBlock * lpBlock = new FTextureBlock;
			lpBlock->iBlock = iBlock;
			dTextureDict.AddRecord( sTexName, lpBlock );
		}
	}
	catch( FException eExcp )
	{
		if( lpRes )
			delete lpRes;
		throw;
	}
	delete lpRes;
}

FGLShader::~FGLShader()
{

	glDetachObject( iProgramID, iVertexID );
	glDetachObject( iProgramID, iFragmentID );
	glDeleteObject( iVertexID );
	glDeleteObject( iFragmentID );
	glDeleteObject( iProgramID );
}

void FGLShader::PushAttrib( UI32 iIndex )
{
	iAttribs[iCurAttrib++] = iIndex;
	glEnableVertexAttribArray( iIndex );
}

void FGLShader::PopAll()
{
	for( UI32 i = 0;i < iCurAttrib;i++ )
		glDisableVertexAttribArray( iAttribs[i] );
	iCurAttrib = 0;
}

void FGLShader::Bind()
{
	glUseProgramObject( iProgramID );
}

void FGLShader::UnBind() 
{
	glUseProgramObject( 0 );
	PopAll();
}


void FGLShader::SetTexture( const FString & sTexName, UI32 iBlock )
{
	GLuint iLoc = glGetUniformLocation( iProgramID, sTexName.GetChar() );
	if( iLoc == -1 )
	{
		FLog::PutError( "Unknown shader texture map variable name: %s", sTexName.GetChar() );
		return;
	}

	glUniform1i( iLoc, iBlock );
}

void FGLShader::SetFloat( const FString & sVarName, F32 fFloat )
{
	GLuint iLoc = glGetUniformLocation( iProgramID, sVarName.GetChar() );
	if( iLoc == -1 )
	{
		FLog::PutError( "Unknown shader float variable name: %s", sVarName.GetChar() );
		return;
	}

	glUniform1f( iLoc, fFloat );
}

void FGLShader::SetFloatPointer( const FString & sVarName, UI32 iStride, const F32 * lpPointer )
{
	GLuint iLoc = glGetAttribLocation( iProgramID, sVarName.GetChar() );
	if( iLoc == -1 )
	{
		FLog::PutError( "Unknown shader float variable name: %s", sVarName.GetChar() );
		return;
	}

	PushAttrib( iLoc );
	glVertexAttribPointer( iLoc, 1, GL_FLOAT, GL_FALSE, iStride, lpPointer );
}

void FGLShader::SetInt( const FString & sVarName, I32 iInt )
{
	GLuint iLoc = glGetUniformLocation( iProgramID, sVarName.GetChar() );
	if( iLoc == -1 )
	{
		FLog::PutError( "Unknown shader int variable name: %s", sVarName.GetChar() );
		return;
	}

	glUniform1i( iLoc, iInt );
}

void FGLShader::SetColor( const FString & sVarName, const FColor4F & cColor )
{
	GLuint iLoc = glGetUniformLocation( iProgramID, sVarName.GetChar() );
	if( iLoc == -1 )
	{
		FLog::PutError( "Unknown shader color variable name: %s", sVarName.GetChar() );
		return;
	}
	
	glUniform4f( iLoc, cColor.r, cColor.g, cColor.b, cColor.a );
}

void FGLShader::SetVector2F( const FString & sVarName, const FVector2F & vVec )
{
	GLuint iLoc = glGetUniformLocation( iProgramID, sVarName.GetChar() );
	if( iLoc == -1 )
	{
		FLog::PutError( "Unknown shader vector2d variable name: %s", sVarName.GetChar() );
		return;
	}

	glUniform2f( iLoc, vVec.x, vVec.y );
}

#endif