#include "myMesh.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <utility>
#include <GL/glew.h>
#include "myVector3D.h"

using namespace std;

myMesh::myMesh(void)
{
	/**** TODO ****/
}

myMesh::~myMesh(void)
{
	/**** TODO ****/
}

void myMesh::clear()
{
	for (unsigned int i = 0; i < vertices.size(); i++)
		if (vertices[i])
			delete vertices[i];
	for (unsigned int i = 0; i < halfedges.size(); i++)
		if (halfedges[i])
			delete halfedges[i];
	for (unsigned int i = 0; i < faces.size(); i++)
		if (faces[i])
			delete faces[i];

	vector<myVertex *> empty_vertices;
	vertices.swap(empty_vertices);
	vector<myHalfedge *> empty_halfedges;
	halfedges.swap(empty_halfedges);
	vector<myFace *> empty_faces;
	faces.swap(empty_faces);
}

void myMesh::checkMesh()
{
	vector<myHalfedge *>::iterator it;
	for (it = halfedges.begin(); it != halfedges.end(); it++)
	{
		if ((*it)->twin == NULL)
			break;
	}
	if (it != halfedges.end())
		cout << "Error! Not all edges have their twins!\n";
	else
		cout << "Each edge has a twin!\n";
}

bool myMesh::readFile(std::string filename)
{
	string s, t, u;
	vector<int> faceids;
	myHalfedge **hedges;

	ifstream fin(filename);
	if (!fin.is_open())
	{
		cout << "Unable to open file!\n";
		return false;
	}
	name = filename;

	map<pair<int, int>, myHalfedge *> twin_map;
	map<pair<int, int>, myHalfedge *>::iterator it;

	while (getline(fin, s))
	{
		stringstream myline(s);
		myline >> t;
		if (t == "g")
		{
		}
		else if (t == "v")
		{
			float x, y, z;
			myline >> x >> y >> z;
			cout << "v " << x << " " << y << " " << z << endl;

			myPoint3D *p = new myPoint3D(x, y, z);
			myVertex *v = new myVertex();
			v->point = p;
			vertices.push_back(v);
		}
		else if (t == "mtllib")
		{
		}
		else if (t == "usemtl")
		{
		}
		else if (t == "s")
		{
		}
		else if (t == "f")
		{
			faceids.clear();
			while (myline >> u)
				faceids.push_back(atoi((u.substr(0, u.find("/"))).c_str()) - 1);

			if (faceids.size() < 3)
				continue;

			hedges = new myHalfedge *[faceids.size()];
			for (unsigned int i = 0; i < faceids.size(); i++)
				hedges[i] = new myHalfedge();

			myFace *f = new myFace();
			f->adjacent_halfedge = hedges[0];

			for (unsigned int i = 0; i < faceids.size(); i++)
			{
				int iplusone = (i + 1) % faceids.size();
				int iminusone = (i - 1 + faceids.size()) % faceids.size();

				hedges[i]->source = vertices[faceids[i]];
				hedges[i]->adjacent_face = f;
				hedges[i]->next = hedges[iplusone];
				hedges[i]->prev = hedges[iminusone];

				it = twin_map.find(make_pair(faceids[iplusone], faceids[i]));
				if (it != twin_map.end())
				{
					hedges[i]->twin = it->second;
					it->second->twin = hedges[i];
				}
				else
					twin_map[make_pair(faceids[i], faceids[iplusone])] = hedges[i];

				if (vertices[faceids[i]]->originof == NULL)
					vertices[faceids[i]]->originof = hedges[i];

				halfedges.push_back(hedges[i]);
			}

			delete[] hedges;
			faces.push_back(f);
		}
	}

	checkMesh();
	normalize();

	return true;
}

void myMesh::computeNormals()
{
	for (unsigned int i = 0; i < faces.size(); i++)
		faces[i]->computeNormal();

	for (unsigned int i = 0; i < vertices.size(); i++)
		vertices[i]->computeNormal();

	
}

void myMesh::normalize()
{
	if (vertices.size() < 1)
		return;

	int tmpxmin = 0, tmpymin = 0, tmpzmin = 0, tmpxmax = 0, tmpymax = 0, tmpzmax = 0;

	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		if (vertices[i]->point->X < vertices[tmpxmin]->point->X)
			tmpxmin = i;
		if (vertices[i]->point->X > vertices[tmpxmax]->point->X)
			tmpxmax = i;

		if (vertices[i]->point->Y < vertices[tmpymin]->point->Y)
			tmpymin = i;
		if (vertices[i]->point->Y > vertices[tmpymax]->point->Y)
			tmpymax = i;

		if (vertices[i]->point->Z < vertices[tmpzmin]->point->Z)
			tmpzmin = i;
		if (vertices[i]->point->Z > vertices[tmpzmax]->point->Z)
			tmpzmax = i;
	}

	double xmin = vertices[tmpxmin]->point->X, xmax = vertices[tmpxmax]->point->X,
		   ymin = vertices[tmpymin]->point->Y, ymax = vertices[tmpymax]->point->Y,
		   zmin = vertices[tmpzmin]->point->Z, zmax = vertices[tmpzmax]->point->Z;

	double scale = (xmax - xmin) > (ymax - ymin) ? (xmax - xmin) : (ymax - ymin);
	scale = scale > (zmax - zmin) ? scale : (zmax - zmin);

	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		vertices[i]->point->X -= (xmax + xmin) / 2;
		vertices[i]->point->Y -= (ymax + ymin) / 2;
		vertices[i]->point->Z -= (zmax + zmin) / 2;

		vertices[i]->point->X /= scale;
		vertices[i]->point->Y /= scale;
		vertices[i]->point->Z /= scale;
	}
}

void myMesh::splitFaceTRIS(myFace *f, myPoint3D *p)
{
	/**** TODO ****/
}

void myMesh::splitEdge(myHalfedge *e1, myPoint3D *p)
{

	/**** TODO ****/
}

void myMesh::splitFaceQUADS(myFace *f, myPoint3D *p)
{
	/**** TODO ****/
}

void myMesh::subdivisionCatmullClark()
{
	/**** TODO ****/
}

void myMesh::simplify()
{
	/**** TODO ****/
}

void myMesh::simplify(myVertex *)
{
	/**** TODO ****/
}

void myMesh::triangulate()
{
	vector<myFace *> original_faces = faces;
	for (unsigned int i = 0; i < original_faces.size(); i++)
		triangulate(original_faces[i]);
}

// return false if already triangle, true otherwise.
bool myMesh::triangulate(myFace *f)
{
	int n = 0;
	myHalfedge *e = f->adjacent_halfedge;
	do {
		n++;
		e = e->next;
	} while (e != f->adjacent_halfedge);

	if (n == 3) return false;

	vector<myHalfedge *> old_edges;
	e = f->adjacent_halfedge;
	for (int i = 0; i < n; i++)
	{
		old_edges.push_back(e);
		e = e->next;
	}

	// Fan triangulation from v0: n-2 triangles, n-3 diagonal edges
	int nd = n - 3;
	vector<myHalfedge *> diag_in(nd), diag_out(nd);
	for (int k = 0; k < nd; k++)
	{
		diag_in[k] = new myHalfedge();
		diag_out[k] = new myHalfedge();

		diag_in[k]->source = old_edges[k + 2]->source;
		diag_out[k]->source = old_edges[0]->source;

		diag_in[k]->twin = diag_out[k];
		diag_out[k]->twin = diag_in[k];

		halfedges.push_back(diag_in[k]);
		halfedges.push_back(diag_out[k]);
	}

	for (int k = 0; k < n - 2; k++)
	{
		myFace *nf;
		if (k == 0)
			nf = f;
		else
		{
			nf = new myFace();
			faces.push_back(nf);
		}

		myHalfedge *e0 = (k == 0)     ? old_edges[0]     : diag_out[k - 1];
		myHalfedge *e1 = old_edges[k + 1];
		myHalfedge *e2 = (k == n - 3)  ? old_edges[n - 1] : diag_in[k];

		nf->adjacent_halfedge = e0;

		e0->next = e1;  e0->prev = e2;  e0->adjacent_face = nf;
		e1->next = e2;  e1->prev = e0;  e1->adjacent_face = nf;
		e2->next = e0;  e2->prev = e1;  e2->adjacent_face = nf;
	}

	return true;
}
