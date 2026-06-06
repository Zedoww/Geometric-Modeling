#include "myVertex.h"
#include "myVector3D.h"
#include "myHalfedge.h"
#include "myFace.h"

myVertex::myVertex(void)
{
	point = NULL;
	originof = NULL;
	normal = new myVector3D(1.0,1.0,1.0);
}

myVertex::~myVertex(void)
{
	if (normal) delete normal;
}

void myVertex::computeNormal()
{
	myVector3D sum(0.0, 0.0, 0.0);
	myHalfedge *start = originof;
	bool boundary = false;

	myHalfedge *h = start;
	do
	{
		if (h->adjacent_face != NULL && h->adjacent_face->normal != NULL)
			sum += *h->adjacent_face->normal;
		if (h->twin == NULL) { boundary = true; break; }
		h = h->twin->next;
	} while (h != NULL && h != start);

	if (boundary && start->prev != NULL)
	{
		h = start->prev->twin;
		while (h != NULL && h != start)
		{
			if (h->adjacent_face != NULL && h->adjacent_face->normal != NULL)
				sum += *h->adjacent_face->normal;
			if (h->prev == NULL) break;
			h = h->prev->twin;
		}
	}

	*normal = sum;
	if (normal->length() > 1e-12)
		normal->normalize();
}
