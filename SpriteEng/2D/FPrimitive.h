#ifndef __FPRIMITIVE_H__
#define __FPRIMITIVE_H__


#include "..\Base\FObject.h"
#include "..\Math\FVector2D.h"


class FScene;
class FGroupSeq;
class FSerialSeq;

class FPrimitive : public FObject
{
	friend class FGroupSeq;
	friend class FSerialSeq;
	UI32 iLayer;
	bool bAnimated;

protected:
	FScene * lpScene;
	F32 fAlpha, fScale;
	bool bShow;

	enum
	{
		ID_POINT = 0,
		ID_LINE,
		ID_FILL_QUAD,
		ID_WF_QUAD
	};

	enum
	{
		DRAW_INDEXED = 0x00000001,
		DRAW_NEED_COUNTING = 0x00000002
	};

	virtual void DrawCallback( UI32 iPrimCount, void * lpData, UI32 * lpIndeces ) {}//вызывается в DrawSeq для отрисовки батча, когда добавлены все данные
	virtual UI32 AddDataToSeq( void * lpData, UI32 * lpIndeces, UI32 iStartVert ) { return 0; }//добавление данных для отрисовки в DrawSeq в цикле повсем примитивам одного PRIM_ID, в конце вызывается DrawCallback
	virtual UI32 GetDataSizePerInst()const { return 0; }//кол-во байт данных на отрисовку одного экземляра примитива данного типа
	virtual UI32 GetIndexCount()const { return 0; }//кол-во индексов на примитив, используется в случае если притив индексированый иначе возвращаем 0(либо любое число)
	virtual UI32 GetVertexCount()const { return 0; }//кол-во вертексов на примитив
	virtual UI32 DrawType()const { return 0; }//тип отрисовки примитива просто массив вершин, либо индексированый массив вершин

	//FObject * CreateObject( void * lpPlacement, UI32 iArgC, va_list lpArgs ) ;

	FPrimitive();
	FPrimitive( UI32 iLayer, FScene * lpScene );
	FPrimitive( const FPrimitive & );
	FPrimitive & operator = ( const FPrimitive & );

public:
	~FPrimitive();

	virtual void Draw( const FRect & rViewport );//в дааном методе добавляем примитив в DrawSeq и можем выполнить еще какие либо действия
	void Update( F32 fDTime );

	UI32 GetLayer()const;
	inline bool IsVisible()const
	{
		return bShow;
	}
	inline bool IsAnimated()const
	{
		return bAnimated;
	}
	void ChangeLayer( UI32 iLayer );
	void SetAlpha( F32 fAlpha );
	void SetAnimated( bool bAnimated );
	void Show( bool bShow );
	void Scale( F32 fScale );
	virtual void Move( const FVector2F & vDPos );
	virtual PRIM_ID GetID()const;
	void UpToFrontView();
	void DownToBackView();

	static void InitMetaData();
};


#define MAKE_PRIM_FRIEND( TYPE ) friend class FPrimitive;\
								TYPE() {}

#endif