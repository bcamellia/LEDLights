#include "PhysicsWorld.h"
#include "LEDMatrix.h"

void PhysicsWorld::initPhysics(){
m_collisionConfiguration = new btDefaultCollisionConfiguration();
	//m_collisionConfiguration->setConvexConvexMultipointIterations();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);

	/*btVoronoiSimplexSolver* simplex = new btVoronoiSimplexSolver();
	btMinkowskiPenetrationDepthSolver* pdSolver = new btMinkowskiPenetrationDepthSolver();


	btConvex2dConvex2dAlgorithm::CreateFunc* convexAlgo2d = new btConvex2dConvex2dAlgorithm::CreateFunc(simplex,pdSolver);
	
	m_dispatcher->registerCollisionCreateFunc(CONVEX_2D_SHAPE_PROXYTYPE,CONVEX_2D_SHAPE_PROXYTYPE,convexAlgo2d);
	m_dispatcher->registerCollisionCreateFunc(BOX_2D_SHAPE_PROXYTYPE,CONVEX_2D_SHAPE_PROXYTYPE,convexAlgo2d);
	m_dispatcher->registerCollisionCreateFunc(CONVEX_2D_SHAPE_PROXYTYPE,BOX_2D_SHAPE_PROXYTYPE,convexAlgo2d);
	m_dispatcher->registerCollisionCreateFunc(BOX_2D_SHAPE_PROXYTYPE,BOX_2D_SHAPE_PROXYTYPE,new btBox2dBox2dCollisionAlgorithm::CreateFunc());
	*/
	m_broadphase = new btDbvtBroadphase();
	//m_broadphase = new btSimpleBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
	m_solver = sol;
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
	//m_dynamicsWorld->getSolverInfo().m_erp = 1.f;
	//m_dynamicsWorld->getSolverInfo().m_numIterations = 4;

	m_dynamicsWorld->setGravity(btVector3(0,0,0));

}

void* PhysicsWorld::rayTest(float x, float y){
	btVector3 from(x,y,1);
	btVector3 to(x,y,-1);
	MyClosestRayResultCallback rayResult(from, to);
	m_dynamicsWorld->rayTest(from,to,rayResult);
	if(rayResult.hasHit()){//&&static_cast<pinNode*>(rayResult.m_collisionObject->getUserPointer())->ob==obj){
		return rayResult.m_collisionObject->getUserPointer();
	}
	return 0;

}