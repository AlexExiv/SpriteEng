//
// Header file to libExt
//
// Author: Alex V. Boreskoff <alexboreskoff@mtu-net.ru>, <steps3d@narod.ru>
//

#ifndef __LIB_EXT__
#define __LIB_EXT__

#ifdef  _WIN32
    #include    <windows.h>
#else
    #include	<stdint.h>
    #define GLX_GLXEXT_LEGACY
#endif

#include    <GL/gl.h>
#include    <GL/glu.h>
#include    "glext.h"


#ifdef  _WIN32
    #include    "wglext.h"
#else
    #include    <GL/glx.h>
    #include    <GL/glxext.h>
#endif

bool    isExtensionSupported ( const char * ext );
void    assertExtensionsSupported ( const char * extList );
void    initExtensions       ();
void    printfInfo           ();                // print info about card, driver, version & etc

const char * getGeneralExtensions  ();			// get list of platform-independant (GL) extensions
const char * getPlatformExtensions ();			// get list of extensions for current platform (WGL or GLX)

struct FDrawElementsIndirect
{
    GLuint count;
    GLuint primCount;
    GLuint firstIndex;
    GLuint baseVertex;
    GLuint reservedMustBeZero;
};


extern PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements;
extern PFNGLDRAWELEMENTSINDIRECTPROC glDrawElementsIndirect;
extern PFNGLDRAWELEMENTSBASEVERTEXPROC glDrawElementsBaseVertex;
extern PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC glMultiDrawElementsBaseVertex;
extern PFNGLDRAWELEMENTSINSTANCEDEXTPROC glDrawElementsInstanced;

extern PFNGLGETRENDERBUFFERPARAMETERIVPROC glGetRenderbufferParameteriv;
//#ifdef  _WIN32
extern  PFNGLACTIVETEXTUREARBPROC               glActiveTexture;
extern  PFNGLCLIENTACTIVETEXTUREARBPROC         glClientActiveTexture;
extern PFNGLTEXSTORAGE2DPROC glTexStorage2D;
extern  PFNGLMULTITEXCOORD1FARBPROC             glMultiTexCoord1f;
extern  PFNGLMULTITEXCOORD1FVARBPROC            glMultiTexCoord1fv;
extern  PFNGLMULTITEXCOORD2FARBPROC             glMultiTexCoord2f;
extern  PFNGLMULTITEXCOORD2FVARBPROC            glMultiTexCoord2fv;
extern  PFNGLMULTITEXCOORD3FARBPROC             glMultiTexCoord3f;
extern  PFNGLMULTITEXCOORD3FVARBPROC            glMultiTexCoord3fv;
extern  PFNGLMULTITEXCOORD4FARBPROC             glMultiTexCoord4f;
extern  PFNGLMULTITEXCOORD4FVARBPROC            glMultiTexCoord4fv;
//#else
//    #define  glActiveTextureARB         glActiveTexture
//    #define  glClientActiveTextureARB   glClientActiveTexture
//#endif
                                        // fog coord function
extern  PFNGLFOGCOORDFEXTPROC                      glFogCoordf;

                                        // secondary color functions
extern  PFNGLSECONDARYCOLOR3FPROC               glSecondaryColor3f;
extern  PFNGLSECONDARYCOLOR3FVPROC              glSecondaryColor3fv;
extern  PFNGLSECONDARYCOLORPOINTERPROC          glSecondaryColorPointer;

                                    // register combiners functions
extern  PFNGLCOMBINERPARAMETERFVNVPROC          glCombinerParameterfvNV;
extern  PFNGLCOMBINERPARAMETERIVNVPROC          glCombinerParameterivNV;
extern  PFNGLCOMBINERPARAMETERFNVPROC           glCombinerParameterfNV;
extern  PFNGLCOMBINERPARAMETERINVPROC           glCombinerParameteriNV;
extern  PFNGLCOMBINERINPUTNVPROC                glCombinerInputNV;
extern  PFNGLCOMBINEROUTPUTNVPROC               glCombinerOutputNV;
extern  PFNGLFINALCOMBINERINPUTNVPROC           glFinalCombinerInputNV;

                                    // VBO functions
extern  PFNGLBINDBUFFERARBPROC                  glBindBuffer;
extern  PFNGLDELETEBUFFERSARBPROC               glDeleteBuffers;
extern  PFNGLGENBUFFERSARBPROC                  glGenBuffers;
extern  PFNGLISBUFFERARBPROC                    glIsBuffer;
extern  PFNGLBUFFERDATAARBPROC                  glBufferData;
extern  PFNGLBUFFERSUBDATAARBPROC               glBufferSubData;
extern  PFNGLCOPYBUFFERSUBDATAPROC glCopyBufferSubData;
extern  PFNGLGETBUFFERSUBDATAARBPROC            glGetBufferSubData;
extern  PFNGLMAPBUFFERARBPROC                   glMapBuffer;
extern  PFNGLUNMAPBUFFERARBPROC                 glUnmapBuffer;
extern  PFNGLGETBUFFERPARAMETERIVARBPROC        glGetBufferParameteriv;
extern  PFNGLGETBUFFERPOINTERVARBPROC           glGetBufferPointerv;

#ifdef  _WIN32
extern  PFNWGLGETEXTENSIONSSTRINGARBPROC        wglGetExtensionsString;

                                    // p-buffer functions
extern  PFNWGLCREATEPBUFFERARBPROC              wglCreatePbuffer;
extern  PFNWGLGETPBUFFERDCARBPROC               wglGetPbufferDC;
extern  PFNWGLRELEASEPBUFFERDCARBPROC           wglReleasePbufferDC;
extern  PFNWGLDESTROYPBUFFERARBPROC             wglDestroyPbuffer;
extern  PFNWGLQUERYPBUFFERARBPROC               wglQueryPbuffer;

                                    // WGL_ARB_pixel_format
extern  PFNWGLCHOOSEPIXELFORMATARBPROC          wglChoosePixelFormat;

                                    // WGL_ARB_render_texture
extern  PFNWGLBINDTEXIMAGEARBPROC               wglBindTexImage;
extern  PFNWGLRELEASETEXIMAGEARBPROC            wglReleaseTexImage;
extern  PFNWGLSETPBUFFERATTRIBARBPROC           wglSetPbufferAttrib;

                                    // WGL_EXT_swap_control
extern  PFNWGLSWAPINTERVALEXTPROC               wglSwapInterval;
extern  PFNWGLGETSWAPINTERVALEXTPROC            wglGetSwapInterval;
#else
                                    // GLX_SGIX_pbuffer
extern  PFNGLXCREATEGLXPBUFFERSGIXPROC          glXCreateGLXPbufferSGIX;
extern  PFNGLXDESTROYGLXPBUFFERSGIXPROC         glXDestroyGLXPbufferSGIX;
extern  PFNGLXQUERYGLXPBUFFERSGIXPROC           glXQueryGLXPbufferSGIX;

                                    // GLX_SGIX_fbconfig
extern  PFNGLXCHOOSEFBCONFIGSGIXPROC            glXChooseFBConfigSGIX;
extern  PFNGLXCREATECONTEXTWITHCONFIGSGIXPROC   glXCreateContextWithConfigSGIX;
#endif

                                    // NV-occlusion query extension
extern  PFNGLGENOCCLUSIONQUERIESNVPROC          glGenOcclusionQueriesNV;
extern  PFNGLBEGINOCCLUSIONQUERYNVPROC          glBeginOcclusionQueryNV;
extern  PFNGLENDOCCLUSIONQUERYNVPROC            glEndOcclusionQueryNV;
extern  PFNGLGETOCCLUSIONQUERYIVNVPROC          glGetOcclusionQueryivNV;

                                    // ARB_vertex_program extension
extern  PFNGLGENPROGRAMSARBPROC                 glGenPrograms;
extern  PFNGLDELETEPROGRAMSARBPROC              glDeletePrograms;
extern  PFNGLBINDPROGRAMARBPROC                 glBindProgram;
extern  PFNGLISPROGRAMARBPROC                   glIsProgram;

extern  PFNGLPROGRAMSTRINGARBPROC               glProgramString;
extern  PFNGLGETPROGRAMIVARBPROC                glGetProgramiv;

extern  PFNGLVERTEXATTRIB4FARBPROC              glVertexAttrib4f;
extern  PFNGLVERTEXATTRIB4FVARBPROC             glVertexAttrib4fv;
extern  PFNGLVERTEXATTRIB3FARBPROC              glVertexAttrib3f;
extern  PFNGLVERTEXATTRIB3FVARBPROC             glVertexAttrib3fv;

extern  PFNGLVERTEXATTRIBPOINTERARBPROC         glVertexAttribPointer;
extern  PFNGLENABLEVERTEXATTRIBARRAYARBPROC     glEnableVertexAttribArray;
extern  PFNGLDISABLEVERTEXATTRIBARRAYARBPROC    glDisableVertexAttribArray;
extern PFNGLVERTEXATTRIBDIVISORARBPROC glVertexAttribDivisor;

extern  PFNGLPROGRAMLOCALPARAMETER4FARBPROC     glProgramLocalParameter4f;
extern  PFNGLPROGRAMLOCALPARAMETER4FVARBPROC    glProgramLocalParameter4fv;
extern  PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC  glGetProgramLocalParameterfv;

extern  PFNGLPROGRAMENVPARAMETER4FARBPROC       glProgramEnvParameter4f;
extern  PFNGLPROGRAMENVPARAMETER4FVARBPROC      glProgramEnvParameter4fv;
extern  PFNGLGETPROGRAMENVPARAMETERFVARBPROC    glGetProgramEnvParameterfv;

                                    // EXT_texture3D
extern  PFNGLTEXIMAGE3DEXTPROC                  glTexImage3D;
extern PFNGLCOPYTEXSUBIMAGE3DEXTPROC glCopyTexSubImage3D;
extern PFNGLTEXSUBIMAGE3DEXTPROC glTexSubImage3D;

                                    // ARB_texture_compression
extern  PFNGLCOMPRESSEDTEXIMAGE3DARBPROC        glCompressedTexImage3D;
extern  PFNGLCOMPRESSEDTEXIMAGE2DARBPROC        glCompressedTexImage2D;
extern  PFNGLCOMPRESSEDTEXIMAGE1DARBPROC        glCompressedTexImage1D;
extern  PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC     glCompressedTexSubImage3D;
extern  PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC     glCompressedTexSubImage2D;
extern  PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC     glCompressedTexSubImage1D;
extern  PFNGLGETCOMPRESSEDTEXIMAGEARBPROC       glGetCompressedTexImage;

                                    // ARB_point_parameters
extern  PFNGLPOINTPARAMETERFARBPROC             glPointParameterf;
extern  PFNGLPOINTPARAMETERFVARBPROC            glPointParameterfv;

                                    // ARB_occlusion_query
extern  PFNGLGENQUERIESARBPROC                  glGenQueries;
extern  PFNGLDELETEQUERIESARBPROC               glDeleteQueries;
extern  PFNGLISQUERYARBPROC                     glIsQuery;
extern  PFNGLBEGINQUERYARBPROC                  glBeginQuery;
extern  PFNGLENDQUERYARBPROC                    glEndQuery;
extern  PFNGLGETQUERYIVARBPROC                  glGetQueryiv;
extern  PFNGLGETQUERYOBJECTIVARBPROC            glGetQueryObjectiv;
extern  PFNGLGETQUERYOBJECTUIVARBPROC           glGetQueryObjectuiv;

                                    // GLSL-specific extensions
                                    // GL_ARB_shader_object
extern  PFNGLDELETEOBJECTARBPROC                glDeleteObject;
extern  PFNGLGETHANDLEARBPROC                   glGetHandle;
extern  PFNGLDETACHOBJECTARBPROC                glDetachObject;
extern  PFNGLCREATESHADEROBJECTARBPROC          glCreateShaderObject;
extern  PFNGLSHADERSOURCEARBPROC                glShaderSource;
extern  PFNGLCOMPILESHADERARBPROC               glCompileShader;
extern  PFNGLCREATEPROGRAMOBJECTARBPROC         glCreateProgramObject;
extern  PFNGLATTACHOBJECTARBPROC                glAttachObject;
extern  PFNGLLINKPROGRAMARBPROC                 glLinkProgram;
extern  PFNGLUSEPROGRAMOBJECTARBPROC            glUseProgramObject;
extern  PFNGLVALIDATEPROGRAMARBPROC             glValidateProgram;
extern  PFNGLUNIFORM1FARBPROC                   glUniform1f;
extern  PFNGLUNIFORM2FARBPROC                   glUniform2f;
extern  PFNGLUNIFORM3FARBPROC                   glUniform3f;
extern  PFNGLUNIFORM4FARBPROC                   glUniform4f;
extern  PFNGLUNIFORM1IARBPROC                   glUniform1i;
extern  PFNGLUNIFORM2IARBPROC                   glUniform2i;
extern  PFNGLUNIFORM3IARBPROC                   glUniform3i;
extern  PFNGLUNIFORM4IARBPROC                   glUniform4i;
extern  PFNGLUNIFORM1FVARBPROC                  glUniform1fv;
extern  PFNGLUNIFORM2FVARBPROC                  glUniform2fv;
extern  PFNGLUNIFORM3FVARBPROC                  glUniform3fv;
extern  PFNGLUNIFORM4FVARBPROC                  glUniform4fv;
extern  PFNGLUNIFORM1IVARBPROC                  glUniform1iv;
extern  PFNGLUNIFORM2IVARBPROC                  glUniform2iv;
extern  PFNGLUNIFORM3IVARBPROC                  glUniform3iv;
extern  PFNGLUNIFORM4IVARBPROC                  glUniform4iv;
extern  PFNGLUNIFORMMATRIX2FVARBPROC            glUniformMatrix2fv;
extern  PFNGLUNIFORMMATRIX3FVARBPROC            glUniformMatrix3fv;
extern  PFNGLUNIFORMMATRIX4FVARBPROC            glUniformMatrix4fv;
extern  PFNGLGETOBJECTPARAMETERFVARBPROC        glGetObjectParameterfv;
extern  PFNGLGETOBJECTPARAMETERIVARBPROC        glGetObjectParameteriv;
extern  PFNGLGETINFOLOGARBPROC                  glGetInfoLog;
extern  PFNGLGETATTACHEDOBJECTSARBPROC          glGetAttachedObjects;
extern  PFNGLGETUNIFORMLOCATIONARBPROC          glGetUniformLocation;
extern  PFNGLGETACTIVEUNIFORMARBPROC            glGetActiveUniform;
extern  PFNGLGETUNIFORMFVARBPROC                glGetUniformfv;
extern  PFNGLGETUNIFORMIVARBPROC                glGetUniformiv;
extern  PFNGLGETSHADERSOURCEARBPROC             glGetShaderSource;

                                        // GL_ARB_vertex_shader
extern  PFNGLBINDATTRIBLOCATIONARBPROC          glBindAttribLocation;
extern  PFNGLGETACTIVEATTRIBARBPROC             glGetActiveAttrib;
extern  PFNGLGETATTRIBLOCATIONARBPROC           glGetAttribLocation;
extern  PFNGLGETVERTEXATTRIBFVARBPROC           glGetVertexAttribfv;

                                        // EXT_stencil_two_side
extern  PFNGLACTIVESTENCILFACEEXTPROC           glActiveStencilFace;

										// EXT_depth_bounds_test
extern	PFNGLDEPTHBOUNDSEXTPROC					glDepthBounds;

										// EXT_framebuffer_object
extern	PFNGLISRENDERBUFFEREXTPROC						glIsRenderbuffer;
extern	PFNGLBINDRENDERBUFFEREXTPROC					glBindRenderbuffer;
extern	PFNGLDELETERENDERBUFFERSEXTPROC					glDeleteRenderbuffers;
extern	PFNGLGENRENDERBUFFERSEXTPROC					glGenRenderbuffers;
extern	PFNGLRENDERBUFFERSTORAGEEXTPROC					glRenderbufferStorage;
extern	PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC			glGetRenderbufferParameteriv;
extern	PFNGLISFRAMEBUFFEREXTPROC						glIsFramebuffer;
extern	PFNGLBINDFRAMEBUFFEREXTPROC						glBindFramebuffer;
extern	PFNGLDELETEFRAMEBUFFERSEXTPROC					glDeleteFramebuffers;
extern	PFNGLGENFRAMEBUFFERSEXTPROC						glGenFramebuffers;
extern	PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC				glCheckFramebufferStatus;
extern	PFNGLFRAMEBUFFERTEXTURE1DEXTPROC				glFramebufferTexture1D;
extern	PFNGLFRAMEBUFFERTEXTURE2DEXTPROC				glFramebufferTexture2D;
extern	PFNGLFRAMEBUFFERTEXTURE3DEXTPROC				glFramebufferTexture3D;
extern PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture;
extern	PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC				glFramebufferRenderbuffer;
extern	PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC	glGetFramebufferAttachmentParameteriv;
extern	PFNGLGENERATEMIPMAPEXTPROC						glGenerateMipmap;

extern PFNGLBLENDEQUATIONPROC glBlendEquation;
extern PFNGLGENERATEMIPMAPEXTPROC glGenerateMipmapEXT;
extern PFNGLDRAWBUFFERSPROC glDrawBuffers;

#ifndef	GL_VERSION_1_3										// ARB_transpose matrix
extern	PFNGLLOADTRANSPOSEMATRIXFPROC					glLoadTransposeMatrixfARB;
extern	PFNGLLOADTRANSPOSEMATRIXDPROC					glLoadTransposeMatrixdARB;
extern	PFNGLMULTTRANSPOSEMATRIXFPROC					glMultTransposeMatrixfARB;
extern	PFNGLMULTTRANSPOSEMATRIXDPROC					glMultTransposeMatrixdARB;
#endif

#endif
