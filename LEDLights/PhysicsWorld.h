#ifndef PHYSICSWORLD_H
#define PHYSICSWORLD_H
#include "../Bullet/src/btBulletDynamicsCommon.h"
//#include "LEDMatrix.h"
#include "PVRTVector.h"
#include <vector>
using namespace std;

/*struct BoundingBox{
	PVRTVec2 ul, ur, ll, lr;
	float x,y;
	BoundingBox(PVRTVec2 ul, PVRTVec2 ur,PVRTVec2 ll, PVRTVec2 lr){
		this->ul = ul;this->ll = ll;
		this->ur = ur;this->lr = lr;
		x=0;
		y=0;
	}
	void translate(float x, float y){
		this->x+=x;
		this->y+=y;
		ul.x+=x;ul.y+=y;
		ur.x+=x;ur.y+=y;
		ll.x+=x;ll.y+=y;
		lr.x+=x;lr.y+=y;
	}
};*/

class PhysicsWorld{
public:
	//PhysicsWorld();
	void* rayTest(float x,float y);
	void initPhysics();
	btBroadphaseInterface*	m_broadphase;
	btCollisionDispatcher*	m_dispatcher;
	btConstraintSolver*	m_solver;
	btDefaultCollisionConfiguration* m_collisionConfiguration;
	btDynamicsWorld* m_dynamicsWorld;
	//vector<BoundingBox> boxes;
};

struct  MyClosestRayResultCallback : public btCollisionWorld::ClosestRayResultCallback
{
const btCollisionShape * m_hitTriangleShape;
int                 m_hitTriangleIndex;
int                 m_hitShapePart;


   MyClosestRayResultCallback (const btVector3 & rayFrom,const btVector3 & rayTo)
      : btCollisionWorld::ClosestRayResultCallback(rayFrom, rayTo),
        m_hitTriangleShape(NULL),
        m_hitTriangleIndex(0),
        m_hitShapePart(0)
   {
   }

   virtual ~MyClosestRayResultCallback()
   {
   }

   virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult & rayResult, bool normalInWorldSpace)
   {
      if (rayResult.m_localShapeInfo)
      {
         m_hitTriangleShape = rayResult.m_collisionObject->getCollisionShape();
         m_hitTriangleIndex = rayResult.m_localShapeInfo->m_triangleIndex;
         m_hitShapePart = rayResult.m_localShapeInfo->m_shapePart;
      } else 
      {
         m_hitTriangleShape = NULL;
         m_hitTriangleIndex = 0;
         m_hitShapePart = 0;
      }
   return ClosestRayResultCallback::addSingleResult(rayResult,normalInWorldSpace);
   }
};

#endif