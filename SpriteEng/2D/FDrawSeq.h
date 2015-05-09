#ifndef __FDRAW_SEQ_H__
#define __FDRAW_SEQ_H__


class FPrimitive;

class FDrawSeq
{
public:
	virtual ~FDrawSeq(){}

	virtual void BeginBuildSeq() = 0;
	virtual void AddPrimitive( FPrimitive * lpPrim ) = 0;
	virtual void EndBuildSeq() = 0;
	virtual void DrawSeq() = 0;
};

#endif