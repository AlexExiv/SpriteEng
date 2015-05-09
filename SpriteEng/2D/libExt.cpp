#include "libExt.h"
#include "..\types.h"

PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements = NULL;
PFNGLDRAWELEMENTSINDIRECTPROC glDrawElementsIndirect = NULL;
PFNGLDRAWELEMENTSBASEVERTEXPROC glDrawElementsBaseVertex = NULL;
PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC glMultiDrawElementsBaseVertex = NULL;
PFNGLDRAWELEMENTSINSTANCEDEXTPROC glDrawElementsInstanced = NULL;

PFNGLGENBUFFERSARBPROC glGenBuffers = NULL;
PFNGLBINDBUFFERARBPROC	glBindBuffer = NULL;
PFNGLDELETEBUFFERSARBPROC	glDeleteBuffers = NULL;
PFNGLBUFFERSUBDATAARBPROC	glBufferSubData = NULL;
PFNGLCOPYBUFFERSUBDATAPROC glCopyBufferSubData = NULL;
PFNGLBUFFERDATAARBPROC	glBufferData = NULL;
PFNGLGETBUFFERSUBDATAARBPROC	glGetBufferSubData = NULL;
PFNGLACTIVETEXTUREARBPROC glActiveTexture = NULL;
PFNGLBLENDEQUATIONPROC glBlendEquation = NULL;
PFNGLGENERATEMIPMAPEXTPROC glGenerateMipmap = NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC         glClientActiveTexture = NULL;
PFNGLMULTITEXCOORD2FARBPROC             glMultiTexCoord2f = NULL;

PFNGLTEXSTORAGE2DPROC glTexStorage2D = NULL;
PFNGLCOPYTEXSUBIMAGE3DEXTPROC glCopyTexSubImage3D = NULL;
PFNGLTEXIMAGE3DEXTPROC                  glTexImage3D = NULL;
PFNGLTEXSUBIMAGE3DEXTPROC glTexSubImage3D = NULL;
PFNGLCOMPRESSEDTEXIMAGE3DARBPROC        glCompressedTexImage3D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC     glCompressedTexSubImage3D = NULL;

PFNGLDELETEOBJECTARBPROC                glDeleteObject = NULL;
PFNGLCREATEPROGRAMOBJECTARBPROC         glCreateProgramObject = NULL;
PFNGLCREATESHADEROBJECTARBPROC          glCreateShaderObject = NULL;
PFNGLDETACHOBJECTARBPROC                glDetachObject = NULL;
PFNGLSHADERSOURCEARBPROC                glShaderSource = NULL;
PFNGLCOMPILESHADERARBPROC               glCompileShader = NULL;
PFNGLATTACHOBJECTARBPROC                glAttachObject = NULL;
PFNGLLINKPROGRAMARBPROC                 glLinkProgram = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC            glUseProgramObject = NULL;
PFNGLVALIDATEPROGRAMARBPROC             glValidateProgram = NULL;
PFNGLUNIFORM1FARBPROC                   glUniform1f = NULL;
PFNGLUNIFORM2FARBPROC                   glUniform2f = NULL;
PFNGLUNIFORM3FARBPROC                   glUniform3f = NULL;
PFNGLUNIFORM4FARBPROC                   glUniform4f = NULL;
PFNGLUNIFORM1IARBPROC                   glUniform1i = NULL;
PFNGLUNIFORM2IARBPROC                   glUniform2i = NULL;
PFNGLUNIFORM3IARBPROC                   glUniform3i = NULL;
PFNGLUNIFORM4IARBPROC                   glUniform4i = NULL;
PFNGLUNIFORM1FVARBPROC                  glUniform1fv = NULL;
PFNGLUNIFORM2FVARBPROC                  glUniform2fv = NULL;
PFNGLUNIFORM3FVARBPROC                  glUniform3fv = NULL;
PFNGLUNIFORM4FVARBPROC                  glUniform4fv = NULL;
PFNGLUNIFORM1IVARBPROC                  glUniform1iv = NULL;
PFNGLUNIFORM2IVARBPROC                  glUniform2iv = NULL;
PFNGLUNIFORM3IVARBPROC                  glUniform3iv = NULL;
PFNGLUNIFORM4IVARBPROC                  glUniform4iv = NULL;
PFNGLUNIFORMMATRIX2FVARBPROC            glUniformMatrix2fv = NULL;
PFNGLUNIFORMMATRIX3FVARBPROC            glUniformMatrix3fv = NULL;
PFNGLUNIFORMMATRIX4FVARBPROC            glUniformMatrix4fv = NULL;
PFNGLGETOBJECTPARAMETERFVARBPROC        glGetObjectParameterfv = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC        glGetObjectParameteriv = NULL;
PFNGLGETINFOLOGARBPROC                  glGetInfoLog = NULL;
PFNGLGETATTACHEDOBJECTSARBPROC          glGetAttachedObjects = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC          glGetUniformLocation = NULL;
PFNGLGETACTIVEUNIFORMARBPROC            glGetActiveUniform = NULL;
PFNGLGETUNIFORMFVARBPROC                glGetUniformfv = NULL;
PFNGLGETUNIFORMIVARBPROC                glGetUniformiv = NULL;
PFNGLGETSHADERSOURCEARBPROC             glGetShaderSource = NULL;
PFNGLCOMPRESSEDTEXIMAGE2DARBPROC        glCompressedTexImage2D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC     glCompressedTexSubImage2D = NULL;
PFNGLGETCOMPRESSEDTEXIMAGEARBPROC       glGetCompressedTexImage = NULL;

PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture = NULL;
PFNGLISRENDERBUFFEREXTPROC						glIsRenderbuffer = NULL;
PFNGLBINDRENDERBUFFEREXTPROC					glBindRenderbuffer = NULL;
PFNGLDELETERENDERBUFFERSEXTPROC					glDeleteRenderbuffers = NULL;
PFNGLGENRENDERBUFFERSEXTPROC					glGenRenderbuffers = NULL;
PFNGLRENDERBUFFERSTORAGEEXTPROC					glRenderbufferStorage = NULL;
PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC			glGetRenderbufferParameteriv = NULL;
PFNGLISFRAMEBUFFEREXTPROC						glIsFramebuffer = NULL;
PFNGLBINDFRAMEBUFFEREXTPROC						glBindFramebuffer = NULL;
PFNGLDELETEFRAMEBUFFERSEXTPROC					glDeleteFramebuffers = NULL;
PFNGLGENFRAMEBUFFERSEXTPROC						glGenFramebuffers = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC				glCheckFramebufferStatus = NULL;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC				glFramebufferRenderbuffer = NULL;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC	glGetFramebufferAttachmentParameteriv = NULL;
PFNGLDRAWBUFFERSPROC glDrawBuffers = NULL;

PFNGLVERTEXATTRIBPOINTERARBPROC         glVertexAttribPointer = NULL;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC     glEnableVertexAttribArray = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYARBPROC    glDisableVertexAttribArray = NULL;
PFNGLVERTEXATTRIBDIVISORARBPROC glVertexAttribDivisor = NULL;
PFNGLBINDATTRIBLOCATIONARBPROC          glBindAttribLocation = NULL;
PFNGLGETACTIVEATTRIBARBPROC             glGetActiveAttrib = NULL;
PFNGLGETATTRIBLOCATIONARBPROC           glGetAttribLocation = NULL;
PFNGLGETVERTEXATTRIBFVARBPROC           glGetVertexAttribfv = NULL;

bool    isExtensionSupported ( const char * ext )
{
	const CHAR_ * lpExtension = (const CHAR_ *)glGetString( GL_EXTENSIONS );
	return true;
}

void    assertExtensionsSupported ( const char * extList )
{
}

void    initExtensions()
{
	if( !isExtensionSupported( "GL_ARB_draw_indirect" ) )
		return;

	glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSWINPROC)wglGetProcAddress( "glDrawRangeElements" );
	glDrawElementsIndirect = (PFNGLDRAWELEMENTSINDIRECTPROC)wglGetProcAddress( "glDrawElementsIndirect" );
	glDrawElementsBaseVertex = (PFNGLDRAWELEMENTSBASEVERTEXPROC)wglGetProcAddress( "glDrawElementsBaseVertex" );
	glMultiDrawElementsBaseVertex = (PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC)wglGetProcAddress( "glMultiDrawElementsBaseVertex" );
	glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDEXTPROC)wglGetProcAddress( "glDrawElementsInstanced" );

	glGenBuffers = (PFNGLGENBUFFERSARBPROC)wglGetProcAddress( "glGenBuffersARB" );
	glBindBuffer = (PFNGLBINDBUFFERARBPROC)wglGetProcAddress( "glBindBufferARB" );
	glDeleteBuffers = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress( "glDeleteBuffersARB" );
	glBufferSubData = (PFNGLBUFFERSUBDATAARBPROC)wglGetProcAddress( "glBufferSubDataARB" );
	glCopyBufferSubData = (PFNGLCOPYBUFFERSUBDATAPROC)wglGetProcAddress( "glCopyBufferSubData" );
	glBufferData = (PFNGLBUFFERDATAARBPROC)wglGetProcAddress( "glBufferDataARB" );
	glGetBufferSubData = (PFNGLGETBUFFERSUBDATAARBPROC)wglGetProcAddress( "glGetBufferSubDataARB" );
	glActiveTexture = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress( "glActiveTextureARB" );
	glClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREARBPROC)wglGetProcAddress( "glClientActiveTextureARB" );
	glBlendEquation = (PFNGLBLENDEQUATIONPROC)wglGetProcAddress( "glBlendEquation" );
	glGenerateMipmap = (PFNGLGENERATEMIPMAPEXTPROC)wglGetProcAddress( "glGenerateMipmapEXT" );
	glMultiTexCoord2f = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress( "glMultiTexCoord2fARB" );

	glDeleteObject = (PFNGLDELETEOBJECTARBPROC)wglGetProcAddress( "glDeleteObjectARB" );
	glCreateProgramObject = (PFNGLCREATEPROGRAMOBJECTARBPROC)wglGetProcAddress( "glCreateProgramObjectARB" );
	glCreateShaderObject = (PFNGLCREATESHADEROBJECTARBPROC)wglGetProcAddress( "glCreateShaderObjectARB" );
	glDetachObject = (PFNGLDETACHOBJECTARBPROC)wglGetProcAddress( "glDetachObjectARB" );
	glShaderSource = (PFNGLSHADERSOURCEARBPROC)wglGetProcAddress( "glShaderSourceARB" );
	glCompileShader = (PFNGLCOMPILESHADERARBPROC)wglGetProcAddress( "glCompileShaderARB" );
	glAttachObject = (PFNGLATTACHOBJECTARBPROC)wglGetProcAddress( "glAttachObjectARB" );
	glLinkProgram = (PFNGLLINKPROGRAMARBPROC)wglGetProcAddress( "glLinkProgramARB" );
	glUseProgramObject = (PFNGLUSEPROGRAMOBJECTARBPROC)wglGetProcAddress( "glUseProgramObjectARB" );
	glValidateProgram = (PFNGLVALIDATEPROGRAMARBPROC)wglGetProcAddress( "glValidateProgramARB" );
	glUniform1f = (PFNGLUNIFORM1FARBPROC)wglGetProcAddress( "glUniform1fARB" );
	glUniform2f = (PFNGLUNIFORM2FARBPROC)wglGetProcAddress( "glUniform2fARB" );
	glUniform3f = (PFNGLUNIFORM3FARBPROC)wglGetProcAddress( "glUniform3fARB" );
	glUniform4f = (PFNGLUNIFORM4FARBPROC)wglGetProcAddress( "glUniform4fARB" );
	glUniform1i = (PFNGLUNIFORM1IARBPROC)wglGetProcAddress( "glUniform1iARB" );
	glUniform2i = (PFNGLUNIFORM2IARBPROC)wglGetProcAddress( "glUniform2iARB" );
	glUniform3i = (PFNGLUNIFORM3IARBPROC)wglGetProcAddress( "glUniform3iARB" );
	glUniform4i = (PFNGLUNIFORM4IARBPROC)wglGetProcAddress( "glUniform4iARB" );
	glUniform1fv = (PFNGLUNIFORM1FVARBPROC)wglGetProcAddress( "glUniform1fvARB" );
	glUniform2fv = (PFNGLUNIFORM2FVARBPROC)wglGetProcAddress( "glUniform2fvARB" );
	glUniform3fv = (PFNGLUNIFORM3FVARBPROC)wglGetProcAddress( "glUniform3fvARB" );
	glUniform4fv = (PFNGLUNIFORM4FVARBPROC)wglGetProcAddress( "glUniform4fvARB" );
	glUniform1iv = (PFNGLUNIFORM1IVARBPROC)wglGetProcAddress( "glUniform1ivARB" );
	glUniform2iv = (PFNGLUNIFORM2IVARBPROC)wglGetProcAddress( "glUniform2ivARB" );
	glUniform3iv = (PFNGLUNIFORM3IVARBPROC)wglGetProcAddress( "glUniform3ivARB" );
	glUniform4iv = (PFNGLUNIFORM4IVARBPROC)wglGetProcAddress( "glUniform4ivARB" );
	glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVARBPROC)wglGetProcAddress( "glUniformMatrix2fvARB" );
	glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVARBPROC)wglGetProcAddress( "glUniformMatrix3fvARB" );
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVARBPROC)wglGetProcAddress( "glUniformMatrix4fvARB" );
	glGetObjectParameterfv = (PFNGLGETOBJECTPARAMETERFVARBPROC)wglGetProcAddress( "glGetObjectParameterfvARB" );
	glGetObjectParameteriv = (PFNGLGETOBJECTPARAMETERIVARBPROC)wglGetProcAddress( "glGetObjectParameterivARB" );
	glGetInfoLog = (PFNGLGETINFOLOGARBPROC)wglGetProcAddress( "glGetInfoLogARB" );
	glGetAttachedObjects = (PFNGLGETATTACHEDOBJECTSARBPROC)wglGetProcAddress( "glGetAttachedObjectsARB" );
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONARBPROC)wglGetProcAddress( "glGetUniformLocationARB" );
	glGetActiveUniform = (PFNGLGETACTIVEUNIFORMARBPROC)wglGetProcAddress( "glGetActiveUniformARB" );
	glGetUniformfv = (PFNGLGETUNIFORMFVARBPROC)wglGetProcAddress( "glGetUniformfvARB" );
	glGetUniformiv = (PFNGLGETUNIFORMIVARBPROC)wglGetProcAddress( "glGetUniformivARB" );
	glGetShaderSource = (PFNGLGETSHADERSOURCEARBPROC)wglGetProcAddress( "glGetShaderSourceARB" );

	glTexStorage2D = (PFNGLTEXSTORAGE2DPROC)wglGetProcAddress( "glTexStorage2D" );
	glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DARBPROC)wglGetProcAddress( "glCompressedTexImage2DARB" );
	glCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC)wglGetProcAddress( "glCompressedTexSubImage2DARB" );
	glCompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3DARBPROC)wglGetProcAddress( "glCompressedTexImage3DARB" );
    glCompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC)wglGetProcAddress( "glCompressedTexSubImage3DARB" );

	glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)wglGetProcAddress( "glFramebufferTexture" );
	glIsRenderbuffer = (PFNGLISRENDERBUFFEREXTPROC)wglGetProcAddress( "glIsRenderbuffer" );
	glBindRenderbuffer = (PFNGLBINDRENDERBUFFEREXTPROC)wglGetProcAddress( "glBindRenderbuffer" );
	glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSEXTPROC)wglGetProcAddress( "glDeleteRenderbuffers" );
	glGenRenderbuffers = (PFNGLGENRENDERBUFFERSEXTPROC)wglGetProcAddress( "glGenRenderbuffers" );
	glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEEXTPROC)wglGetProcAddress( "glRenderbufferStorage" );
	glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC)wglGetProcAddress( "glGetRenderbufferParameteriv" );
	glIsFramebuffer = (PFNGLISFRAMEBUFFEREXTPROC)wglGetProcAddress( "glIsFramebuffer" );
	glBindFramebuffer = (PFNGLBINDFRAMEBUFFEREXTPROC)wglGetProcAddress( "glBindFramebuffer" );
	glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSEXTPROC)wglGetProcAddress( "glDeleteFramebuffers" );
	glGenFramebuffers = (PFNGLGENFRAMEBUFFERSEXTPROC)wglGetProcAddress( "glGenFramebuffers" );
	glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress( "glCheckFramebufferStatus" );
	glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)wglGetProcAddress( "glFramebufferRenderbuffer" );
	glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC)wglGetProcAddress( "glGetFramebufferAttachmentParameteriv" );

	glGetCompressedTexImage = (PFNGLGETCOMPRESSEDTEXIMAGEARBPROC)wglGetProcAddress( "glGetCompressedTexImageARB" );
	glCopyTexSubImage3D = (PFNGLCOPYTEXSUBIMAGE3DEXTPROC)wglGetProcAddress( "glCopyTexSubImage3DEXT" );
    glTexImage3D = (PFNGLTEXIMAGE3DEXTPROC)wglGetProcAddress( "glTexImage3DEXT" );
	glTexSubImage3D = (PFNGLTEXSUBIMAGE3DEXTPROC)wglGetProcAddress( "glTexSubImage3DEXT" );;

	glDrawBuffers = (PFNGLDRAWBUFFERSPROC)wglGetProcAddress( "glDrawBuffers" );

	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERARBPROC)wglGetProcAddress( "glVertexAttribPointer" );
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYARBPROC)wglGetProcAddress( "glEnableVertexAttribArray" );
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYARBPROC)wglGetProcAddress( "glDisableVertexAttribArray" );
	glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORARBPROC)wglGetProcAddress( "glVertexAttribDivisor" );
	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONARBPROC)wglGetProcAddress( "glBindAttribLocation" );
	glGetActiveAttrib = (PFNGLGETACTIVEATTRIBARBPROC)wglGetProcAddress( "glGetActiveAttrib" );
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONARBPROC)wglGetProcAddress( "glGetAttribLocation" );
	glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVARBPROC)wglGetProcAddress( "glGetVertexAttribfv" );

}

void    printfInfo()
{
}

const char * getGeneralExtensions  ()
{
	return NULL;
}

const char * getPlatformExtensions ()
{
	return NULL;
}
