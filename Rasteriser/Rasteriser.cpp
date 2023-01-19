#include "Rasteriser.h"

Rasteriser app;

string Rasteriser::ModelPath()
{
	char buf[256];
	GetCurrentDirectoryA(256, buf);
	return std::string(buf) + '\\';
}

bool Rasteriser::Initialise()
{
	_modelpath="";
	_modelpath = ModelPath() + "\\cube.md2";

	if (!MD2Loader::LoadModel(_modelpath.c_str(), _model,
		&Model::AddPolygon,
		&Model::AddVertex))
	{
		return false;
	}
	_model;
	
	Camera temp(0, 0, 0, Vertex(0, 0, -50, 1)); 
	_camera = temp;
	return true;
}


void Rasteriser::DrawWireFrame(Bitmap &bitmap)
{

	//TODO : Create the DrawWireFrame method
	// here is some help how to implement this

	// 1. Get the Polygons from the _model and store it to a local list std::vector<Polygon3D>
	std::vector<Polygon3D> temppolygon(_model.GetPolygons());
	// 2. Get the polygons size (How many there are)
	int polygonSize = int(_model.GetPolygonCount());
	// 3. Get the current transformations from the _model and store it to a local list std::vector<Vertex>
	std::vector<Vertex> tempvertex(_model.GetTransform());
	// 4. Create a Pen while store the current pen
	 		HPEN hPen = CreatePen(PS_SOLID, 1, RGB(100, 12, 3));
			HGDIOBJ oldPen = SelectObject(bitmap.GetDC(), hPen);
	// 5. Make a loop for 0 till you reach the polygon size
	//    a. for each polygon get the Index of it and then the Vertex that corresponds to this index.
	//       You will have three Vertexes for each polygon
	//    b. Draw the polygon (triangle) using the LineTo function
	//		
			
			for (int i = 0; i < _model.GetPolygonCount(); i++)
			{
				bool culling = temppolygon[i].GetCull();
				if (culling == false && _frameCount >= 1040 && _frameCount < 1130)
				{
					int tempIndex0 = temppolygon[i].GetIndex(0);
					Vertex tempVertex0(tempvertex[tempIndex0]);
					int tempIndex1 = temppolygon[i].GetIndex(1);
					Vertex tempVertex1(tempvertex[tempIndex1]);
					int tempIndex2 = temppolygon[i].GetIndex(2);
					Vertex tempVertex2(tempvertex[tempIndex2]);

					MoveToEx(bitmap.GetDC(), tempVertex0.GetIntX(), tempVertex0.GetIntY(), NULL);
					LineTo(bitmap.GetDC(), tempVertex1.GetIntX(), tempVertex1.GetIntY());
					LineTo(bitmap.GetDC(), tempVertex2.GetIntX(), tempVertex2.GetIntY());
					LineTo(bitmap.GetDC(), tempVertex0.GetIntX(), tempVertex0.GetIntY());
				}
				else if (culling == true && _frameCount >= 1130 )
				{
					int tempIndex0 = temppolygon[i].GetIndex(0);
					Vertex tempVertex0(tempvertex[tempIndex0]);
					int tempIndex1 = temppolygon[i].GetIndex(1);
					Vertex tempVertex1(tempvertex[tempIndex1]);
					int tempIndex2 = temppolygon[i].GetIndex(2);
					Vertex tempVertex2(tempvertex[tempIndex2]);

					MoveToEx(bitmap.GetDC(), tempVertex0.GetIntX(), tempVertex0.GetIntY(), NULL);
					LineTo(bitmap.GetDC(), tempVertex1.GetIntX(), tempVertex1.GetIntY());
					LineTo(bitmap.GetDC(), tempVertex2.GetIntX(), tempVertex2.GetIntY());
					LineTo(bitmap.GetDC(), tempVertex0.GetIntX(), tempVertex0.GetIntY());
				}
				else if (_frameCount < 1040) {
					int tempIndex0 = temppolygon[i].GetIndex(0);
					Vertex tempVertex0(tempvertex[tempIndex0]);
					int tempIndex1 = temppolygon[i].GetIndex(1);
					Vertex tempVertex1(tempvertex[tempIndex1]);
					int tempIndex2 = temppolygon[i].GetIndex(2);
					Vertex tempVertex2(tempvertex[tempIndex2]);

					MoveToEx(bitmap.GetDC(), tempVertex0.GetIntX(), tempVertex0.GetIntY(), NULL);
					LineTo(bitmap.GetDC(), tempVertex1.GetIntX(), tempVertex1.GetIntY());
					LineTo(bitmap.GetDC(), tempVertex2.GetIntX(), tempVertex2.GetIntY());
					LineTo(bitmap.GetDC(), tempVertex0.GetIntX(), tempVertex0.GetIntY());
				}

			}
}
void Rasteriser::DrawSolid(Bitmap& bitmap)
{
	HDC hDc = bitmap.GetDC();
	const vector<Polygon3D>& polygons = _model.GetPolygons();
	const vector<Vertex>& vertices = _model.GetTransform();
	POINT points[3];

	for (int i = 0; i < _model.GetPolygonCount(); i++)
	{
		const Polygon3D& polygon = polygons[i];
		if (polygon.GetCull())
		{
			for (int j = 0; j < 3; j++)
			{
				points[j].x = (long)vertices[polygons[i].GetIndex(j)].GetX();
				points[j].y = (long)vertices[polygons[i].GetIndex(j)].GetY();
			}

			HPEN hPen = CreatePen(PS_SOLID, 1, polygon.GetColour());
			HGDIOBJ oldPen = SelectObject(hDc, hPen);
			HBRUSH brush = CreateSolidBrush(polygon.GetColour());
			HGDIOBJ old_brush = SelectObject(hDc, brush);

			Polygon(hDc, points, 3);

			SelectObject(hDc, oldPen);
			DeleteObject(hPen);
			SelectObject(hDc, old_brush);
			DeleteObject(brush);
		}
	}
}



void Rasteriser::GenerateProjectionMatrix(float d, float aspectRatio)
{
	// Perspective projection
	Matrix perspective{ d / aspectRatio, 0, 0, 0,
						0, d, 0, 0,
						0, 0, d, 0,
						0, 0, 1, 0 };
	_perspectiveTransform = perspective;
}

void Rasteriser::GenerateScreenMatrix(float d, int width, int height) // ScrenTransformation
{
	Matrix view{ float(width) / 2, 0, 0, float(width) / 2,
				 0, float(-height) / 2, 0, float(height) / 2,
				 0, 0, d / 2, d / 2,
				 0, 0, 0, 1 };
	_screenTransform = view;
}

void Rasteriser::Update(Bitmap& bitmap)
{

	float aspectRatio = float(float(bitmap.GetWidth()) / float(bitmap.GetHeight()));
	GenerateProjectionMatrix(1, aspectRatio);
	GenerateScreenMatrix(1, bitmap.GetWidth(), bitmap.GetHeight());
	// Displaying the code
	if (_frameCount < 50)
	{
		_phase = RenderPhase::Start;
		_displayText = L"Wireframe";
	}
	else if (_frameCount < 140)
	{
		_phase = RenderPhase::WireframeScaleX;
		_displayText = L"Wireframe: X Axis Scale";
		//
	}
	else if (_frameCount < 230)
	{
		_phase = RenderPhase::WireframeScaleY;
		_displayText = L"Wireframe: Y Axis Scale";
		//
	}
	else if (_frameCount < 320)
	{
		_phase = RenderPhase::WireframeScaleZ;
		_displayText = L"Wireframe: Z Axis Scale";
		//
	}
	else if (_frameCount < 410)
	{
		_phase = RenderPhase::WireframeScaleXYZ;
		_displayText = L"Wireframe: XYZ Axis Scale";
		//
	}
	else if (_frameCount < 500)
	{
		_phase = RenderPhase::WireframeRotateX;
		_displayText = L"Wireframe: X Axis Rotate";
		//
	}
	else if (_frameCount < 590)
	{
		_phase = RenderPhase::WireframeRotateY;
		_displayText = L"Wireframe: Y Axis Rotate";
		//
	}
	else if (_frameCount < 680)
	{
		_phase = RenderPhase::WireframeRotateZ;
		_displayText = L"Wireframe: Z Axis Rotate";
		//
	}
	else if (_frameCount < 770)
	{
		_phase = RenderPhase::WireframeTranslateX;
		_displayText = L"Wireframe: X Axis Translate";
		//
	}
	else if (_frameCount < 860)
	{
		_phase = RenderPhase::WireframeTranslateY;
		_displayText = L"Wireframe: Y Axis Translate";
		//
	}
	else if (_frameCount < 950)
	{
		_phase = RenderPhase::WireframeTranslateZ;
		_displayText = L"Wireframe: Z Axis Translate";
		//
	}
	else if (_frameCount < 1040)
	{
		_phase = RenderPhase::WireframeTranslateXYZ;
		_displayText = L"Wireframe: XYZ Axis Translate";
		//
	}
	else if (_frameCount < 1130)
	{

		_phase = RenderPhase::WireframeBackface;
		_displayText = L"Wireframe: Backfaces";
		//
	}
	else if (_frameCount < 1220)
	{

		_phase = RenderPhase::WireframeBackface;
		_displayText = L"Wireframe: Innerfaces";
	}
	else if (_frameCount < 1310)
	{
		_phase = RenderPhase::WireframeSolidflat;
		_displayText = L"Wireframe: Solid Flat";
	}
	_frameCount++;


}

void Rasteriser::Render(Bitmap &bitmap)   
{
	
	bitmap.Clear(RGB(0, 0, 0));
	switch (_phase)
	{
	case RenderPhase::Start:
	{
		_modelTransform = Matrix::YRotationMatrix(0);
		_model.ApplyTransformToLocalVertices(_modelTransform);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);
		//TODO: Create inside the model class a method that calculates (marks) the back faces and call it here to test it as _model.CalculateBackfaces(_camera)
		_model.CalculateBackfaces(_camera);
		DrawWireFrame(bitmap);
		break;
	}
	case RenderPhase::WireframeScaleX:
	{
		_model.ApplyTransformToLocalVertices(_modelTransform);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);
		_angle += 0.01f;
		_modelTransform = Matrix::ScalingMatrixX(_angle);
		DrawWireFrame(bitmap);
		break;
	}
	case RenderPhase::WireframeScaleY:
	{
		_model.ApplyTransformToLocalVertices(_modelTransform);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);
		_angle += 0.01f;
		_modelTransform = Matrix::ScalingMatrixY(_angle);
		DrawWireFrame(bitmap);
		break;
	}
	case RenderPhase::WireframeScaleZ:
	{
		_model.ApplyTransformToLocalVertices(_modelTransform);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);
		_angle += 0.01f;
		_modelTransform = Matrix::ScalingMatrixZ(_angle);
		DrawWireFrame(bitmap);
		break;
	}
	case RenderPhase::WireframeScaleXYZ:
	{
		_model.ApplyTransformToLocalVertices(_modelTransform);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);
		_angle -= 0.02f;
		_modelTransform = Matrix::ScalingMatrixXYZ(_angle, _angle, _angle);
		DrawWireFrame(bitmap);
		break;
	}
	case RenderPhase::WireframeRotateX:
	{
		_model.ApplyTransformToLocalVertices(_modelTransform);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);
		_angle += 0.01f;
		_modelTransform = Matrix::XRotationMatrix(_angle);
		DrawWireFrame(bitmap);
		break;
	}
	case RenderPhase::WireframeRotateY:
	{
		_model.ApplyTransformToLocalVertices(_modelTransform);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);
		_angle += 0.01f;
		_modelTransform = Matrix::YRotationMatrix(_angle);
		_model.CalculateBackfaces(_camera);
		DrawWireFrame(bitmap);
		break;
	}
	case RenderPhase::WireframeRotateZ:
	{
		_model.ApplyTransformToLocalVertices(_modelTransform);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);
		_angle += 0.01f;
		_modelTransform = Matrix::ZRotationMatrix(_angle);
		DrawWireFrame(bitmap);
		break;
	}
	case RenderPhase::WireframeRotateXYZ:
	{
		_model.ApplyTransformToLocalVertices(_modelTransform);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);
		_angle += 0.01f;
		_modelTransform = Matrix::XYZRotationMatrix(_angle, _angle, _angle);
		DrawWireFrame(bitmap);
		break;
	}
	case RenderPhase::WireframeTranslateX:
	{
		_model.ApplyTransformToLocalVertices(_modelTransform);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);
		_angle += 0.1f;
		_modelTransform = Matrix::TranslationMatrix(_angle, 0, 0);
		DrawWireFrame(bitmap);
		break;
	}
	case RenderPhase::WireframeTranslateY:
	{
		_model.ApplyTransformToLocalVertices(_modelTransform);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);
		_angle += 0.1f;
		_modelTransform = Matrix::TranslationMatrix(0, _angle, 0);
		DrawWireFrame(bitmap);
		break;
	}
	case RenderPhase::WireframeTranslateZ:
	{
		_model.ApplyTransformToLocalVertices(_modelTransform);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);
		_angle += 0.1f;
		_modelTransform = Matrix::TranslationMatrix(0, 0, _angle);
		DrawWireFrame(bitmap);
		break;
	}
	case RenderPhase::WireframeTranslateXYZ:
	{
		_model.ApplyTransformToLocalVertices(_modelTransform);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);
		_angle += 0.1f;
		_modelTransform = Matrix::TranslationMatrix(_angle, _angle, _angle);
		DrawWireFrame(bitmap);
		break;
	}
	case RenderPhase::WireframeBackface:
	{
		_angle += 0.01f;
		_modelTransform = Matrix::XYZRotationMatrix(_angle, _angle, _angle);
		_model.ApplyTransformToLocalVertices(_modelTransform);
		_model.CalculateBackfaces(_camera);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.Sort();
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);
		DrawWireFrame(bitmap);
		break;
	}
	case RenderPhase::WireframeInnerface:
	{
		_angle += 0.01f;
		_modelTransform = Matrix::XYZRotationMatrix(_angle, _angle, _angle);
		_model.ApplyTransformToLocalVertices(_modelTransform);
		_model.CalculateBackfaces(_camera);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.Sort();
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);
		DrawWireFrame(bitmap);
		break;
	}
	case RenderPhase::WireframeSolidflat:
	{
		_angle += 0.01f;
		_modelTransform = Matrix::XYZRotationMatrix(_angle, _angle, _angle);
		_model.ApplyTransformToLocalVertices(_modelTransform);
		_model.CalculateBackfaces(_camera);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.Sort();
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);
		DrawSolid(bitmap);
		break;
	}
	default:
		break;
	}
	DrawString(bitmap, _displayText.c_str());
	
}
void Rasteriser::DrawString(const Bitmap& bitmap, LPCTSTR text)
{
	HDC hdc = bitmap.GetDC();
	HFONT hFont, hOldFont;

	// Retrieve a handle to the variable stock font.  
	hFont = hFont = CreateFont(48, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Myfont"));

	// Select the variable stock font into the specified device context. 
	if (hOldFont = (HFONT)SelectObject(hdc, hFont))
	{
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkColor(hdc, RGB(0, 0, 0));

		// Display the text string.  
		TextOut(hdc, 10, 10, text, lstrlen(text));

		// Restore the original font.        
		SelectObject(hdc, hOldFont);
	}
	DeleteObject(hFont);
}
