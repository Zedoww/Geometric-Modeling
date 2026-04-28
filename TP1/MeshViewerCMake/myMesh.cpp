#include "myMesh.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <utility>
#include <cmath>
#include <limits>
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
		if (!(myline >> t))
			continue;
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
	if (faces.empty() || halfedges.empty() || vertices.size() < 3)
		return;

	triangulate();

	myHalfedge *best = NULL;
	double best_len2 = std::numeric_limits<double>::max();

	for (unsigned int i = 0; i < halfedges.size(); i++)
	{
		myHalfedge *e = halfedges[i];
		if (e == NULL || e->twin == NULL || e->source == NULL || e->twin->source == NULL)
			continue;
		if (e > e->twin)
			continue;

		myPoint3D *p0 = e->source->point;
		myPoint3D *p1 = e->twin->source->point;
		double dx = p0->X - p1->X;
		double dy = p0->Y - p1->Y;
		double dz = p0->Z - p1->Z;
		double len2 = dx * dx + dy * dy + dz * dz;

		if (len2 < best_len2)
		{
			best_len2 = len2;
			best = e;
		}
	}

	if (best == NULL)
		return;

	simplify(best->source);
	
}

void myMesh::simplify(myVertex *)
{
	simplify();
}

void myMesh::revolution()
{
	vector<myPoint3D> profile;
	profile.push_back(myPoint3D(0.15, -0.20, 0.0));
	profile.push_back(myPoint3D(0.20, -0.25, 0.0));
	profile.push_back(myPoint3D(0.30, -0.25, 0.0));
	profile.push_back(myPoint3D(0.35, 0.00, 0.0));
	profile.push_back(myPoint3D(0.90, 0.0, 0.0));

	clear();

	const int slices = 20;
	const double two_pi = 6.28318530717958647692;
	const int profile_count = static_cast<int>(profile.size());

	vector<vector<myVertex *>> grid(slices, vector<myVertex *>(profile_count, NULL));
	for (int s = 0; s < slices; s++)
	{
		double angle = (two_pi * static_cast<double>(s)) / static_cast<double>(slices);
		double c = cos(angle);
		double si = sin(angle);
		for (int i = 0; i < profile_count; i++)
		{
			const myPoint3D &p = profile[i];
			myVertex *v = new myVertex();
			v->point = new myPoint3D(c * p.X + si * p.Z, p.Y, -si * p.X + c * p.Z);
			vertices.push_back(v);
			grid[s][i] = v;
		}
	}

	map<pair<myVertex *, myVertex *>, myHalfedge *> twin_map;
	for (int s = 0; s < slices; s++)
	{
		int sn = (s + 1) % slices;
		for (int i = 0; i < profile_count - 1; i++)
		{
			myVertex *v0 = grid[s][i];
			myVertex *v1 = grid[sn][i];
			myVertex *v2 = grid[sn][i + 1];
			myVertex *v3 = grid[s][i + 1];

			myFace *f = new myFace();
			myHalfedge *e0 = new myHalfedge();
			myHalfedge *e1 = new myHalfedge();
			myHalfedge *e2 = new myHalfedge();
			myHalfedge *e3 = new myHalfedge();

			e0->source = v0;
			e1->source = v1;
			e2->source = v2;
			e3->source = v3;

			e0->adjacent_face = f;
			e1->adjacent_face = f;
			e2->adjacent_face = f;
			e3->adjacent_face = f;

			e0->next = e1;
			e1->next = e2;
			e2->next = e3;
			e3->next = e0;

			e0->prev = e3;
			e1->prev = e0;
			e2->prev = e1;
			e3->prev = e2;

			if (v0->originof == NULL)
				v0->originof = e0;
			if (v1->originof == NULL)
				v1->originof = e1;
			if (v2->originof == NULL)
				v2->originof = e2;
			if (v3->originof == NULL)
				v3->originof = e3;

			f->adjacent_halfedge = e0;
			faces.push_back(f);
			halfedges.push_back(e0);
			halfedges.push_back(e1);
			halfedges.push_back(e2);
			halfedges.push_back(e3);

			myHalfedge *face_edges[4] = {e0, e1, e2, e3};
			for (int k = 0; k < 4; k++)
			{
				myVertex *a = face_edges[k]->source;
				myVertex *b = face_edges[k]->next->source;
				pair<myVertex *, myVertex *> rev_key = make_pair(b, a);
				map<pair<myVertex *, myVertex *>, myHalfedge *>::iterator it = twin_map.find(rev_key);
				if (it != twin_map.end())
				{
					face_edges[k]->twin = it->second;
					it->second->twin = face_edges[k];
				}
				else
					twin_map[make_pair(a, b)] = face_edges[k];
			}
		}
	}
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
	const double eps = 1e-10;
	int n = 0;
	myHalfedge *e = f->adjacent_halfedge;
	do
	{
		n++;
		e = e->next;
	} while (e != f->adjacent_halfedge);
	if (n == 3)
		return false;

	vector<myHalfedge *> hedges(n);
	vector<myVertex *> verts(n);
	e = f->adjacent_halfedge;
	for (int i = 0; i < n; i++)
	{
		hedges[i] = e;
		verts[i] = e->source;
		e = e->next;
	}

	myVector3D faceN(0, 0, 0);
	for (int i = 0; i < n; i++)
	{
		myPoint3D *a = verts[i]->point, *b = verts[(i + 1) % n]->point;
		faceN.dX += (a->Y - b->Y) * (a->Z + b->Z);
		faceN.dY += (a->Z - b->Z) * (a->X + b->X);
		faceN.dZ += (a->X - b->X) * (a->Y + b->Y);
	}
	if (faceN.length() < eps)
		return false;

	vector<int> nxt(n), prv(n);
	for (int i = 0; i < n; i++)
	{
		nxt[i] = (i + 1) % n;
		prv[i] = (i - 1 + n) % n;
	}

	int remaining = n;
	int cur = 0;
	while (remaining > 3)
	{
		bool found = false;
		int startIdx = cur;
		do
		{
			int p = prv[cur];
			int nx = nxt[cur];

			myVector3D v1 = *verts[cur]->point - *verts[p]->point;
			myVector3D v2 = *verts[nx]->point - *verts[cur]->point;
			if (v1.crossproduct(v2) * faceN > 0)
			{
				bool isEar = true;
				int test = nxt[nx];
				while (test != p)
				{
					myVector3D c0 = (*verts[cur]->point - *verts[p]->point).crossproduct(*verts[test]->point - *verts[p]->point);
					myVector3D c1 = (*verts[nx]->point - *verts[cur]->point).crossproduct(*verts[test]->point - *verts[cur]->point);
					myVector3D c2 = (*verts[p]->point - *verts[nx]->point).crossproduct(*verts[test]->point - *verts[nx]->point);
					if (c0 * faceN > eps && c1 * faceN > eps && c2 * faceN > eps)
					{
						isEar = false;
						break;
					}
					test = nxt[test];
				}
				if (isEar)
				{
					myHalfedge *d_in = new myHalfedge();
					myHalfedge *d_out = new myHalfedge();
					d_in->source = verts[nx];
					d_out->source = verts[p];
					d_in->twin = d_out;
					d_out->twin = d_in;
					halfedges.push_back(d_in);
					halfedges.push_back(d_out);
					myFace *nf = new myFace();
					faces.push_back(nf);
					nf->adjacent_halfedge = hedges[p];
					hedges[p]->next = hedges[cur];
					hedges[cur]->next = d_in;
					d_in->next = hedges[p];
					hedges[p]->prev = d_in;
					hedges[cur]->prev = hedges[p];
					d_in->prev = hedges[cur];
					hedges[p]->adjacent_face = nf;
					hedges[cur]->adjacent_face = nf;
					d_in->adjacent_face = nf;
					hedges[p] = d_out;
					nxt[p] = nx;
					prv[nx] = p;
					remaining--;
					cur = nx;
					found = true;
					break;
				}
			}
			cur = nxt[cur];
		} while (cur != startIdx);
		if (!found)
			break;
	}

	int i0 = cur;
	int i1 = nxt[i0];
	int i2 = nxt[i1];
	f->adjacent_halfedge = hedges[i0];
	hedges[i0]->next = hedges[i1];
	hedges[i1]->next = hedges[i2];
	hedges[i2]->next = hedges[i0];
	hedges[i0]->prev = hedges[i2];
	hedges[i1]->prev = hedges[i0];
	hedges[i2]->prev = hedges[i1];
	hedges[i0]->adjacent_face = f;
	hedges[i1]->adjacent_face = f;
	hedges[i2]->adjacent_face = f;
	return true;
}
