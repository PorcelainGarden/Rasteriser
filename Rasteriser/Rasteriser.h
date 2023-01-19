#pragma once
#define _USE_MATH_DEFINES
#include <vector>
#include <time.h>
#include <math.h>
#include "Framework.h"
#include "Vertex.h"
#include "Matrix.h"
#include "Model.h"
#include "Polygon3d.h"
#include "MD2Loader.h"
#include "Camera.h"
#include "Vector3D.h"

enum class RenderPhase
{
	Start,
	WireframeScaleX,
	WireframeScaleY,
	WireframeScaleZ,
	WireframeScaleXYZ,
	WireframeRotateX,
	WireframeRotateY,
	WireframeRotateZ,
	WireframeRotateXYZ,
	WireframeTranslateX,
	WireframeTranslateY,
	WireframeTranslateZ,
	WireframeTranslateXYZ,
	WireframeBackface,
	WireframeInnerface,
	WireframeSolidflat

};
class Rasteriser : public Framework
{
public:
	string ModelPath();
	bool Initialise();
	void Update(Bitmap &bitmap);
	void Render(Bitmap &bitmap);

	void DrawWireFrame(Bitmap &bitmap);
	void DrawSolid(Bitmap &bitmap);
	void GenerateProjectionMatrix(float d, float aspectRatio);
	void GenerateScreenMatrix(float d, int width, int height);
	void Rasteriser::BackfaceCulling(Bitmap &bitmap);
	void DrawString(const Bitmap& bitmap, LPCTSTR text);
	float x=0;
private:
	Model _model;
	Camera _camera;
	Matrix _modelTransform;
	Matrix _perspectiveTransform;
	Matrix _screenTransform;
	string _modelpath;
	float _angle;

	// For displaying purposes
	int _frameCount{ 0 };
	std::wstring _displayText = L"";
	RenderPhase _phase = RenderPhase::Start;

	
};

