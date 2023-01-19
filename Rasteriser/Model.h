#pragma once

#include <vector>
#include "Polygon3D.h"
#include "Vertex.h"
#include "Matrix.h"
#include "Camera.h"
#include <algorithm>
#include "stdafx.h"
#include <math.h>



class Model
{
public:
	Model();
	Model(const Model& m);
	~Model();

	//Accessors and mutators
	const std::vector<Polygon3D>& GetPolygons() const;
	const std::vector<Vertex>& GetVertices()const;
	const std::vector<Vertex>& GetTransform()const;

	
	size_t GetPolygonCount() const;
	size_t GetVertexCount() const;

	void AddVertex(float x, float y, float z);
	void AddPolygon(int i0, int i1, int i2);

	void ApplyTransformToLocalVertices(const Matrix &transform);
	void ApplyTransformToTransformedVertices(const Matrix &transform);

	void Dehomogenize();

	void CalculateBackfaces(Camera camera);

	void Sort(void);

	void CalculateVertexNormals();




private:
	std::vector<Polygon3D> _polygons;
	std::vector<Vertex> _vertices;
	std::vector<Vertex> _transform;
	bool _boolStatus = false;
	
};