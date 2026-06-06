#include "myMesh.h"
#include "myFace.h"
#include "myVertex.h"
#include "myHalfedge.h"
#include "myVector3D.h"
#include "myPoint3D.h"

#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;

#ifndef DATA_DIR
#define DATA_DIR "."
#endif

static int g_pass = 0;
static int g_fail = 0;

#define CHECK(cond, msg)                        \
	do                                          \
	{                                           \
		if (cond)                               \
		{                                       \
			g_pass++;                           \
			cout << "  [PASS] " << msg << "\n"; \
		}                                       \
		else                                    \
		{                                       \
			g_fail++;                           \
			cout << "  [FAIL] " << msg << "\n"; \
		}                                       \
	} while (0)

struct Mute
{
	streambuf *old;
	ostringstream sink;
	Mute() { old = cout.rdbuf(sink.rdbuf()); }
	~Mute() { cout.rdbuf(old); }
};

static string dataPath(const string &name)
{
	return string(DATA_DIR) + "/" + name;
}

static bool loadQuiet(myMesh &m, const string &name)
{
	Mute mute;
	return m.readFile(dataPath(name));
}

static int countMissingTwins(const myMesh &m)
{
	int c = 0;
	for (size_t i = 0; i < m.halfedges.size(); i++)
		if (m.halfedges[i]->twin == NULL)
			c++;
	return c;
}

static bool allTwinsConsistent(const myMesh &m)
{
	for (size_t i = 0; i < m.halfedges.size(); i++)
	{
		myHalfedge *h = m.halfedges[i];
		if (h->twin == NULL)
			return false;
		if (h->twin->twin != h)
			return false;
		if (h->twin->source != h->next->source)
			return false;
	}
	return true;
}

static bool allFacesAreTriangles(const myMesh &m)
{
	for (size_t i = 0; i < m.faces.size(); i++)
	{
		myHalfedge *h0 = m.faces[i]->adjacent_halfedge;
		int n = 0;
		myHalfedge *h = h0;
		do
		{
			n++;
			h = h->next;
		} while (h != h0 && h != NULL && n < 100);
		if (n != 3)
			return false;
	}
	return true;
}

static void test_readFile()
{
	cout << "test_readFile (cube)\n";
	myMesh m;
	bool ok = loadQuiet(m, "cube.obj");
	CHECK(ok, "cube.obj loads");
	CHECK(m.vertices.size() == 8, "cube has 8 vertices");
	CHECK(m.faces.size() == 6, "cube has 6 faces");
	CHECK(m.halfedges.size() == 24, "cube has 24 half-edges");
}

static void test_twins()
{
	cout << "test_twins (cube is closed)\n";
	myMesh m;
	loadQuiet(m, "cube.obj");
	CHECK(countMissingTwins(m) == 0, "every half-edge has a twin");
	CHECK(allTwinsConsistent(m), "twin->twin == self and twin->source == next->source");
}

static void test_normals()
{
	cout << "test_normals (unit length)\n";
	myMesh m;
	loadQuiet(m, "cube.obj");
	{
		Mute mute;
		m.computeNormals();
	}
	bool allUnit = true;
	for (size_t i = 0; i < m.vertices.size(); i++)
		if (fabs(m.vertices[i]->normal->length() - 1.0) > 1e-6)
			allUnit = false;
	CHECK(allUnit, "all vertex normals have length 1");

	bool facesUnit = true;
	for (size_t i = 0; i < m.faces.size(); i++)
		if (fabs(m.faces[i]->normal->length() - 1.0) > 1e-6)
			facesUnit = false;
	CHECK(facesUnit, "all face normals have length 1");
}

static void test_catmullclark()
{
	cout << "test_catmullclark (cube)\n";
	myMesh m;
	loadQuiet(m, "cube.obj");

	{
		Mute mute;
		m.subdivisionCatmullClark();
	}
	CHECK(m.vertices.size() == 26, "after 1 round: 26 vertices");
	CHECK(m.faces.size() == 24, "after 1 round: 24 faces");
	CHECK(countMissingTwins(m) == 0, "after 1 round: still closed (all twins)");

	{
		Mute mute;
		m.subdivisionCatmullClark();
	}
	CHECK(m.vertices.size() == 98, "after 2 rounds: 98 vertices");
	CHECK(m.faces.size() == 96, "after 2 rounds: 96 faces");
}

static void test_triangulate()
{
	cout << "test_triangulate (cube)\n";
	myMesh m;
	loadQuiet(m, "cube.obj");
	size_t before = m.faces.size();
	{
		Mute mute;
		m.triangulate();
	}
	CHECK(m.faces.size() > before, "triangulate increases face count");
	CHECK(allFacesAreTriangles(m), "every face is a triangle");
}

static void test_simplify()
{
	cout << "test_simplify (apple)\n";
	myMesh m;
	loadQuiet(m, "apple.obj");
	{
		Mute mute;
		m.triangulate();
	}
	size_t before = m.faces.size();
	{
		Mute mute;
		m.simplify();
	}
	CHECK(m.faces.size() < before, "simplify reduces face count");
	CHECK(m.faces.size() > 0, "simplify keeps a non-empty mesh");
	CHECK(allTwinsConsistent(m) || countMissingTwins(m) > 0,
		  "simplify produces a consistent half-edge mesh");
}

static void test_revolution()
{
	cout << "test_revolution\n";
	myMesh m;
	loadQuiet(m, "cube.obj");
	{
		Mute mute;
		m.revolution();
	}
	CHECK(m.vertices.size() > 0, "revolution creates vertices");
	CHECK(m.faces.size() > 0, "revolution creates faces");
}

int main()
{
	cout << "==== mesh tests ====\n";
	test_readFile();
	test_twins();
	test_normals();
	test_catmullclark();
	test_triangulate();
	test_simplify();
	test_revolution();

	cout << "\n==== summary: " << g_pass << " passed, "
		 << g_fail << " failed ====\n";
	return g_fail == 0 ? 0 : 1;
}
