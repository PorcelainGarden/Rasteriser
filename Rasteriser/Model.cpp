#include "Model.h"

Model::Model()
{
	_polygons.clear();
	_vertices.clear();
	_transform.clear();

}


//Copy Constructor
Model::Model(const Model& m)
{
	_polygons = m.GetPolygons();
	_vertices = m.GetVertices();
	_transform = m.GetTransform();

}

Model::~Model()
{

}

//Accessors and mutators
const std::vector<Polygon3D>& Model::GetPolygons() const
{
	return _polygons;
}

const std::vector<Vertex>& Model::GetVertices() const
{
	return _vertices;
}

const std::vector<Vertex>& Model::GetTransform() const
{
	return _transform;
}


size_t Model::GetPolygonCount() const
{
	return _polygons.size();
}

size_t Model::GetVertexCount() const
{
	return _vertices.size();
}

void Model::AddVertex(float x, float y, float z)
{
	Vertex temp(x, y, z, 1);
	_vertices.push_back(temp);
}

void Model::AddPolygon(int i0, int i1, int i2)
{
	Polygon3D temp(i0, i1, i2);
	_polygons.push_back(temp);
}

void Model::ApplyTransformToLocalVertices(const Matrix &transform)
{
	_transform.clear();
	int verticesSize = int(_vertices.size());
	for (int i = 0; i < verticesSize; i++)
	{
		_transform.push_back(transform * _vertices[i]);
	}
}

void Model::ApplyTransformToTransformedVertices(const Matrix &transform)
{
	int transformSize = int(_transform.size());
	for (int i = 0; i < transformSize; i++)
	{
		_transform[i] = transform * _transform[i];
	}
}

void Model::Dehomogenize()
{
	int transformSize = int(_transform.size());
	for (int i = 0; i < transformSize; i++)
	{
		_transform[i].Dehomogenize();
	}
}

void Model::CalculateBackfaces(Camera camera)
{
	//TODO : Similar to the philosofy of drawwireframe, create a method that for each triangle will calculate if this triangle is culled or not
	int polygonSize = int(_polygons.size());
	for (int i = 0; i < polygonSize; i++)
	{
		//Get the indices
		int index0 = _polygons[i].GetIndex(0);
		int index1 = _polygons[i].GetIndex(1);
		int index2 = _polygons[i].GetIndex(2);

		//Get the vertices
		Vertex vertex0 = _transform.at(index0);
		Vertex vertex1 = _transform.at(index1);
		Vertex vertex2 = _transform.at(index2);

		//Contructing vectors by subtracting vertices
		Vector3D vectorA = vertex1.subVector(vertex0);
		Vector3D vectorB = vertex2.subVector(vertex1);

		//Calculating the normal (cross product) between the two vectors
		Vector3D normal = vectorA.CrossProduct(vectorB);

		//Creating an eye-vector by subtracting the camera position from vertex0
		Vector3D eyeVector = vertex0.subVector(camera.GetPos());

		// Calculate the dot product and check it
		// If <0 the mark the polygon as culled. See the code bellow
		bool cull;
			if (float dotProduct = normal.DotProduct(eyeVector) < 0)
			{
				//Flag this polygon for culling
				cull = true;
				_polygons[i].SetCull(cull);
			}
			else
			{
				//Flag the polygon to not be culled any more, so it can be displayed when it rotates round
				cull = false;
				_polygons[i].SetCull(cull);
			}
			_polygons[i].SetNormal(normal);
	}
}



bool polygonOrder(Polygon3D polygon1, Polygon3D polygon2)
{
	return polygon1.GetAverageZ() > polygon2.GetAverageZ();
}

void Model::Sort(void)
{
	//TODO : Create a method for sorting the polygons
	int polygonSize = int(_polygons.size());

	for (int i = 0; i < polygonSize; i++)
	{
		// Gets polygon index
		int index1 = _polygons[i].GetIndex(0);
		int index2 = _polygons[i].GetIndex(1);
		int index3 = _polygons[i].GetIndex(2);
		
		// Gets polygon vertex
		Vertex vertex1 = _transform[index1];
		Vertex vertex2 = _transform[index2];
		Vertex vertex3 = _transform[index3];

		// Calculates average value of every single polygon vertex
		_polygons[i].SetAverageZ((vertex1.GetZ() + vertex2.GetZ() + vertex3.GetZ()) / 3);
	}
	std::sort(_polygons.begin(), _polygons.end(), polygonOrder);
}





