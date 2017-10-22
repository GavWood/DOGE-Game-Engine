////////////////////////////////////////////////////////////////////////////////
/// DyBody.cpp

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

#include "DyBody.h"
#include "DyImpl.h"

#include "SgNode.h"
#include "SgCollision.h"
#include "SgRigidBody.h"
#include "BtString.h"

////////////////////////////////////////////////////////////////////////////////
// Constructor

DyBody::DyBody() : m_mass(0),
				   m_pNode(BtNull),
				   m_pRigidBody(BtNull)
{
	m_isActive = BtTrue;
}

////////////////////////////////////////////////////////////////////////////////
// CreateEmpty

void DyBody::Create()
{
	m_pCompound = new btCompoundShape();
}

////////////////////////////////////////////////////////////////////////////////
// Create

void DyBody::Create( BtFloat mass )
{
	// Set the mass
	m_mass = mass;

	// Create a new motion state
	m_pMotionState = new btDefaultMotionState();

	// Calculate the local inertia tensor
	btVector3 v3Tensor;
	m_pCompound->calculateLocalInertia( mass, v3Tensor );

	m_pRigidBody = new btRigidBody( mass,
									m_pMotionState,
									m_pCompound,
									v3Tensor );
	m_pRigidBody->setFriction( 0 );
	m_pRigidBody->setRestitution( 0 );
	m_pRigidBody->setActivationState( DISABLE_DEACTIVATION );
	m_pRigidBody->setUserPointer( this );

	// Add the rigid body
	DyImpl::AddRigidBody( m_pRigidBody );
}

////////////////////////////////////////////////////////////////////////////////
// CreateSphere

void DyBody::CreateSphere( BtFloat mass, const MtVector3 &v3Dimension )
{
	// Set the mass
	m_mass = mass;

	// Create a new motion state
	m_pMotionState = new btDefaultMotionState();

	// Create a new compound shape
	m_pCompound = new btCompoundShape();

	// Create the box
	btCollisionShape* pSphereShape = new btSphereShape( v3Dimension.x * 0.5f );

	// Add the box to the compound shape
	btTransform m4Transform;
	m4Transform.setIdentity();
	m_pCompound->addChildShape( m4Transform, pSphereShape );

	// Calculate the local inertia tensor
	btVector3 v3Tensor;
	m_pCompound->calculateLocalInertia( mass, v3Tensor );

	m_pRigidBody = new btRigidBody( mass,
		m_pMotionState,
		m_pCompound,
		v3Tensor );
	m_pRigidBody->setFriction( 0.2f );
	m_pRigidBody->setRestitution( 0 );
	m_pRigidBody->setActivationState( DISABLE_DEACTIVATION );
	m_pRigidBody->setUserPointer( this );

	// Add the rigid body
	DyImpl::AddRigidBody( m_pRigidBody );
}

void DyBody::SetActive( BtBool isActive )
{
	m_isActive = isActive;
	//m_pRigidBody->
}

////////////////////////////////////////////////////////////////////////////////
// CreateBox

void DyBody::CreateBox( BtFloat mass, const MtVector3 &v3Dimension )
{
	// Set the mass
	m_mass = mass;

	// Create a new motion state
	m_pMotionState = new btDefaultMotionState();

	// Create a new compound shape
	m_pCompound = new btCompoundShape();

	// Make a box
	btVector3 halfExtents( v3Dimension.x * 0.5f, v3Dimension.y * 0.5f, v3Dimension.z * 0.5f );

	// Create the box
	m_pBoxShape = new btBoxShape( halfExtents );

	// Add the box to the compound shape
	btTransform m4Transform;
	m4Transform.setIdentity();
	m_pCompound->addChildShape( m4Transform, m_pBoxShape );

	// Calculate the local inertia tensor
	btVector3 btTensor( 0, 0, 0 );
	if( mass != 0 )
	{
		m_pCompound->calculateLocalInertia( mass, btTensor);
	}

	m_pRigidBody = new btRigidBody( mass,
									m_pMotionState,
									m_pCompound,
									btTensor);
	m_pRigidBody->setFriction( 0.2f );
	m_pRigidBody->setRestitution( 0 );
	m_pRigidBody->setActivationState( DISABLE_DEACTIVATION );
	m_pRigidBody->setUserPointer( this );

	m_v3Dimension = v3Dimension;

	// Add the rigid body
	DyImpl::AddRigidBody( m_pRigidBody );
}

////////////////////////////////////////////////////////////////////////////////
// SetMass

void DyBody::SetMass( BtFloat mass, const MtVector3& v3Tensor, const MtVector3& v3Gravity )
{
	m_mass = mass;

	// Set the mass
	m_pRigidBody->setMassProps( mass, btVector3( v3Tensor.x, v3Tensor.y, v3Tensor.z ) );
	m_pRigidBody->setGravity( btVector3( v3Gravity.x, v3Gravity.y, v3Gravity.z ) );
}

////////////////////////////////////////////////////////////////////////////////
// CreateCylinder

void DyBody::CreateCylinder( BtFloat mass, const MtVector3 &v3Dimension )
{
	// Create the dimensions
	btVector3 halfExtents( v3Dimension.x, v3Dimension.y, v3Dimension.z );

	// Create the cylinder
	btCylinderShape* pCylinderShape = new btCylinderShape( halfExtents );

	// Create a compound shape
	m_pCompound = new btCompoundShape();

	// Add the cylinder to the compound shape
	btTransform m4Transform;
	m4Transform.setIdentity();
	m_pCompound->addChildShape( m4Transform, pCylinderShape );

	// Calculate the local inertia tensor
	btVector3 v3Tensor;
	m_pCompound->calculateLocalInertia( mass, v3Tensor );

	// Set the motion state
	m_pMotionState = new btDefaultMotionState( m4Transform );

	// Create the new rigid body
	m_pRigidBody = new btRigidBody( mass, m_pMotionState, m_pCompound, v3Tensor );

	m_pRigidBody->setFriction( 0.2f );
	m_pRigidBody->setRestitution( 0 );
	m_pRigidBody->setActivationState( DISABLE_DEACTIVATION );
	m_pRigidBody->setUserPointer( this );

	// Add the rigid body
	DyImpl::AddRigidBody( m_pRigidBody );
}

////////////////////////////////////////////////////////////////////////////////
// Create

void DyBody::Create( std::vector<MtVector3> vertex, MtMatrix4 &m4LocalTransform )
{
	m_pNode = BtNull;

	BtStrCopy( m_name, 256, "polygon_soup" );

	// Create a compound shape
	m_pCompound = new btCompoundShape();
	
	// Create a convex hull shape
	btConvexHullShape* pConvexHullShape = new btConvexHullShape();

	BtU32 numVertex = (BtU32)vertex.size();

	for( BtU32 iVertex=0; iVertex<numVertex; ++iVertex )
	{
		MtVector3 &vertex3 = vertex[iVertex];

		// Construct each point
		btVector3 v3Point( vertex3.x, vertex3.y, vertex3.z );

		// Add each point
		pConvexHullShape->addPoint( v3Point );
	}

	// Add the mesh shape to the compound shape
	btTransform m4Transform;
	m4Transform.setFromOpenGLMatrix( &m4LocalTransform[0][0] );
	m_pCompound->addChildShape( m4Transform, pConvexHullShape );

	BtFloat mass = 0;

	// Calculate the local inertia tensor
	btVector3 v3Tensor;
	m_pCompound->calculateLocalInertia( mass, v3Tensor );

	// Set the motion state
	m_pMotionState = new btDefaultMotionState( m4Transform );

	// Create the new rigid body
	m_pRigidBody = new btRigidBody( mass, m_pMotionState, m_pCompound, v3Tensor );

	m_pRigidBody->setFriction( 0.2f );

	m_pRigidBody->setRestitution( 0 );

	// Disable deactivation
	m_pRigidBody->setActivationState( DISABLE_DEACTIVATION );

	// Set the user pointer
	m_pRigidBody->setUserPointer( this );

	// Disable deactivation
	m_pRigidBody->setActivationState( DISABLE_DEACTIVATION );

	// Add the rigid body
	DyImpl::AddRigidBody( m_pRigidBody );

	int a=0;
	a++;
}

////////////////////////////////////////////////////////////////////////////////
// Create

void DyBody::Create( SgNode* pNode )
{
	m_pNode = pNode;

	BtStrCopy( m_name, 256, pNode->GetTitle() );

	SgRigidBody* pRigidBody = pNode->pRigidBody();

	BtAssert( pRigidBody )

	SgCollision* pCollision = pNode->pCollision();

	BtAssert( pCollision );

	// Create a compound shape
	m_pCompound = new btCompoundShape();

	// Cache the number of shapes
	BtU32 numShapes = pCollision->NumShapes();

	if( numShapes > 0 )
	{
		for( BtU32 iShape=0; iShape<numShapes; ++iShape )
		{
			// Cache each shape
			const SgCollisionShape* pShape = pCollision->pShape( iShape );

			// Check the shape type
			switch( pShape->shapeType() )
			{
			case SgCT_Cylinder :
				{
					// Cache the box
					SgCollisionCylinder* pCylinder = (SgCollisionCylinder*) pShape;

					// Create the dimensions
					btVector3 halfExtents( pCylinder->m_pFileData->m_radius, pCylinder->m_pFileData->m_radius, pCylinder->m_pFileData->m_height );

					// Create the cylinder
					btCylinderShape* pCylinderShape = new btCylinderShape( halfExtents );

					// Add the cylinder to the compound shape
					btTransform m4Transform;
					m4Transform.setFromOpenGLMatrix( &pCylinder->m_pFileData->m_m4Transform[0][0] );
					m_pCompound->addChildShape( m4Transform, pCylinderShape );
				}
				break;

			case SgCT_Box :
				{
					// Cache the box
					SgCollisionBox* pBox = (SgCollisionBox*) pShape;

					BaSgCollisionBox *pFileData = pBox->m_pFileData;

					// Create the dimensions
					btVector3 halfExtents( pFileData->m_v3Dimension.x, pFileData->m_v3Dimension.y, pFileData->m_v3Dimension.z );

					// Create the box
					btCollisionShape* pBoxShape = new btBoxShape( halfExtents );

					// Add the box to the compound shape
					btTransform m4Transform;
					m4Transform.setFromOpenGLMatrix( &pFileData->m_m4Transform[0][0] );
					m_pCompound->addChildShape( m4Transform, pBoxShape );
				}
				break;

			case SgCT_Sphere :
				{
					// Cache the sphere
					SgCollisionSphere* pSphere = (SgCollisionSphere*) pShape;

					BaSgCollisionSphere *pFileData = pSphere->m_pFileData;

					// Create the sphere
					btCollisionShape* pSphereShape = new btSphereShape( pFileData->m_radius );

					// Add the sphere to the compound shape
					btTransform m4Transform;
					m4Transform.setFromOpenGLMatrix( &pFileData->m_m4Transform[0][0] );
					m_pCompound->addChildShape( m4Transform, pSphereShape );
				}
				break;

			case SgCT_Mesh :
				{
					// Cache the mesh
					SgCollisionMesh* pCollisionMesh = (SgCollisionMesh*) pShape;

					BaSgCollisionMesh *pFileData = pCollisionMesh->m_pFileData;

					// Cache the number of index
					BtU32 numIndex = pFileData->m_endIndex - pFileData->m_startIndex;

					// Cache the number of vertex
					BtU32 numVertex = pFileData->m_endVertex - pFileData->m_startVertex;

					// Cache the number of faces
					BtU32 numFaces = numIndex / 3;

					// Cache the vertex
					BtFloat* pVertex = pCollision->pVertex() + ( pFileData->m_startVertex * 4 );

					// Cache the index
					BtU32* pIndex = pCollision->pIndex() + pFileData->m_startIndex;

					BtU32 SizeOfIndex = sizeof( BtU32 );
					BtU32 SizeOfVertex = sizeof( BtFloat ) * 4;

					btTriangleIndexVertexArray* pMesh = new btTriangleIndexVertexArray( numFaces,
						(int*) pIndex,
						SizeOfIndex * 3,
						numVertex,
						pVertex,
						SizeOfVertex );

					btBvhTriangleMeshShape* pMeshShape = new btBvhTriangleMeshShape( pMesh, BtTrue );

					pMeshShape->setLocalScaling( btVector3( 1.0f, 1.0f, 1.0f ) );
					pMeshShape->setMargin( 0.02f );

					// Add the mesh shape to the compound shape
					btTransform m4Transform;
					m4Transform.setFromOpenGLMatrix( &pFileData->m_m4Transform[0][0] );
					m_pCompound->addChildShape( m4Transform, pMeshShape );
				}
				break;

			case SgCT_ConvexHull :
				{
					// Cache the mesh
					SgCollisionHull* pCollisionHull = (SgCollisionHull*) pShape;

					BaSgCollisionMesh *pMesh = pCollisionHull->m_pFileData;

					// Cache the number of vertex
					BtU32 numVertex = pMesh->m_endVertex - pMesh->m_startVertex;

					// Cache the vertex
					BtFloat* pVertex = pCollision->pVertex();

					// Create a convex hull shape
					btConvexHullShape* pConvexHullShape = new btConvexHullShape();

					BtU32 iIndex = 0;

					for( BtU32 iVertex=0; iVertex<numVertex; ++iVertex )
					{
						// Construct each point
						btVector3 v3Point( pVertex[iIndex + 0], pVertex[iIndex + 1], pVertex[iIndex + 2] );

						// Increment the index
						iIndex += 4;

						// Add each point
						pConvexHullShape->addPoint( v3Point );
					}

					// Add the mesh shape to the compound shape
					btTransform m4Transform;
					m4Transform.setFromOpenGLMatrix( &pMesh->m_m4Transform[0][0] );
					m_pCompound->addChildShape( m4Transform, pConvexHullShape );
				}
				break;
                    
            case SgCT_Plane :
            case SgCT_Max :
                break;
			}
		}
	}

	BtFloat mass = pRigidBody->GetMass();

	// Calculate the local inertia tensor
	btVector3 v3Tensor;
	m_pCompound->calculateLocalInertia( mass, v3Tensor );

	btTransform m4Transform;
	m4Transform.setFromOpenGLMatrix( &pNode->GetWorldTransform()[0][0] );

	// Set the motion state
	m_pMotionState = new btDefaultMotionState( m4Transform );

	// Create the new rigid body
	m_pRigidBody = new btRigidBody( mass, m_pMotionState, m_pCompound, v3Tensor );

	// Set the friction
	m_pRigidBody->setFriction( pRigidBody->Friction() );

	// Set the friction
	m_pRigidBody->setRestitution( pRigidBody->Restitution() );

	// Disable deactivation
	m_pRigidBody->setActivationState( DISABLE_DEACTIVATION );

	// Set the user pointer
	m_pRigidBody->setUserPointer( this );

	// Disable deactivation
	m_pRigidBody->setActivationState( DISABLE_DEACTIVATION );

	// Add the rigid body
	DyImpl::AddRigidBody( m_pRigidBody );

	int a=0;
	a++;
}

////////////////////////////////////////////////////////////////////////////////
// GetVecI

const MtVector3 DyBody::GetVecI() const
{
	MtMatrix3 m3Rotation = GetRotation();
	MtVector3 v3Col0 = m3Rotation.Row0();
	return v3Col0;
}

////////////////////////////////////////////////////////////////////////////////
// GetVecJ

const MtVector3 DyBody::GetVecJ() const
{
	MtMatrix3 m3Rotation = GetRotation();
	MtVector3 v3Col1 = m3Rotation.Row1();
	return v3Col1;
}

////////////////////////////////////////////////////////////////////////////////
// GetVecK

const MtVector3 DyBody::GetVecK() const
{
	MtMatrix3 m3Rotation = GetRotation();
	MtVector3 v3Col2 = m3Rotation.Row2();
	return v3Col2;
}

////////////////////////////////////////////////////////////////////////////////
// Destroy

void DyBody::Destroy()
{
	if( m_pRigidBody )
	{
		// Clear the user pointer
		m_pRigidBody->setUserPointer( BtNull );

		// Remove the rigid body
		DyImpl::RemoveRigidBody( m_pRigidBody );

		if( m_pBoxShape )
		{
			delete m_pBoxShape;
			m_pBoxShape = BtNull;
		}

		if(m_pCompound)
		{
			delete m_pCompound;
			m_pCompound = BtNull;
		}

		if(m_pMotionState)
		{
			delete m_pMotionState;
			m_pMotionState = BtNull;
		}

		if(m_pRigidBody)
		{
			delete m_pRigidBody;
			m_pRigidBody = BtNull;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// SetNoCollision

void DyBody::SetNoCollision()
{
	m_pRigidBody->setCollisionFlags( m_pRigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE );
}

////////////////////////////////////////////////////////////////////////////////
// SetUserPointer

void DyBody::SetUserPointer( void* userPointer )
{
	m_pRigidBody->setUserPointer( userPointer );
}

////////////////////////////////////////////////////////////////////////////////
// ClearForces

void DyBody::ClearForces()
{
	m_pRigidBody->clearForces();
}

////////////////////////////////////////////////////////////////////////////////
// ApplyWorldForce

void DyBody::ApplyWorldForce( const MtVector3& v3Force, MtVector3& v3LocalPosition )
{
	m_pRigidBody->applyForce( btVector3( v3Force.x, v3Force.y, v3Force.z ),
							  btVector3( v3LocalPosition.x, v3LocalPosition.y, v3LocalPosition.z ) );

	m_pRigidBody->activate( BtTrue );
}

////////////////////////////////////////////////////////////////////////////////
// GetLocalXInWorld

MtVector3 DyBody::GetLocalXInWorld()
{
	MtMatrix3 m4Rotation = GetWorldTransform().GetRotation();
	MtVector3 v3Row = m4Rotation.Row0();
	return v3Row;
}

////////////////////////////////////////////////////////////////////////////////
// GetLocalYInWorld

MtVector3 DyBody::GetLocalYInWorld()
{
	MtMatrix3 m4Rotation = GetWorldTransform().GetRotation();
	MtVector3 v3Row = m4Rotation.Row1();
	return v3Row;
}

////////////////////////////////////////////////////////////////////////////////
// GetLocalZInWorld

MtVector3 DyBody::GetLocalZInWorld()
{
	MtMatrix3 m4Rotation = GetWorldTransform().GetRotation();
	MtVector3 v3Row = m4Rotation.Row2();
	return v3Row;
}

////////////////////////////////////////////////////////////////////////////////
// ApplyWorldImpulse

void DyBody::ApplyWorldImpulse( const MtVector3& v3WorldImpulse, const MtVector3& v3LocalPosition )
{
	m_pRigidBody->applyImpulse( btVector3( v3WorldImpulse.x, v3WorldImpulse.y, v3WorldImpulse.z ),
								btVector3( v3LocalPosition.x, v3LocalPosition.y, v3LocalPosition.z ) );
}

////////////////////////////////////////////////////////////////////////////////
// ApplyLocalForce

void DyBody::ApplyLocalForce( const MtVector3& v3LocalForce, const MtVector3& v3LocalPosition )
{
	MtVector3 v3WorldForce = v3LocalForce * GetRotation();
	MtVector3 v3Position   = v3LocalPosition * GetRotation();

	m_pRigidBody->applyForce( btVector3( v3WorldForce.x, v3WorldForce.y, v3WorldForce.z ),
							  btVector3( v3Position.x, v3Position.y, v3Position.z ) );
}

////////////////////////////////////////////////////////////////////////////////
// ApplyLocalImpulse

void DyBody::ApplyLocalImpulse( const MtVector3& v3LocalImpulse, MtVector3& v3RelativePosition )
{
	MtVector3 v3WorldImpulse = v3LocalImpulse * GetRotation();
	MtVector3 v3Position     = v3RelativePosition * GetRotation();

	m_pRigidBody->applyImpulse( btVector3( v3WorldImpulse.x, v3WorldImpulse.y, v3WorldImpulse.z ),
								btVector3( v3Position.x, v3Position.y, v3Position.z ) );
}

////////////////////////////////////////////////////////////////////////////////
// ApplyTorque

void DyBody::ApplyTorque( const MtVector3& v3WorldTorque )
{
	m_pRigidBody->applyTorque( btVector3( v3WorldTorque.x, v3WorldTorque.y, v3WorldTorque.z ) );
}

////////////////////////////////////////////////////////////////////////////////
// GetWorldTransform

MtMatrix4 DyBody::GetWorldTransform() const
{
	MtMatrix4 m4Transform;
	btTransform transform;
	transform = m_pRigidBody->getWorldTransform();
	transform.getOpenGLMatrix( &m4Transform[0][0] );
	return m4Transform;
}

////////////////////////////////////////////////////////////////////////////////
// Rotation

MtMatrix3 DyBody::GetRotation() const
{
	return GetWorldTransform().GetRotation();
}

////////////////////////////////////////////////////////////////////////////////
// GetInverseInertiaTensor

MtMatrix3 DyBody::GetInverseInertiaTensor() const
{
	MtMatrix3 m3InverseInertiaTensor;
	btMatrix3x3 transform;
	transform = m_pRigidBody->getInvInertiaTensorWorld();
	transform.getOpenGLSubMatrix( &m3InverseInertiaTensor[0][0] );
	return m3InverseInertiaTensor;
}

////////////////////////////////////////////////////////////////////////////////
// SetWorldTransform

void DyBody::SetWorldTransform( const MtMatrix4& m4WorldTransform )
{
	BtAssert( m_pRigidBody != BtNull );

	btTransform transform;
	transform.setFromOpenGLMatrix( &m4WorldTransform[0][0] );
	m_pRigidBody->setWorldTransform( transform );
}

////////////////////////////////////////////////////////////////////////////////
// GetLocalVelocity

MtVector3 DyBody::GetLocalVelocity() const
{
    const btVector3 b3Velocity = m_pRigidBody->getLinearVelocity();
	MtVector3 v3Velocity( b3Velocity.x(), b3Velocity.y(), b3Velocity.z() );
	v3Velocity = v3Velocity * GetRotation().GetTranspose();
	return v3Velocity;
}

////////////////////////////////////////////////////////////////////////////////
// GetLocalVelocity

MtVector3 DyBody::GetLocalVelocity(const MtVector3& v3LocalPosition) const
{
	MtVector3 v3LocalPosition2 = v3LocalPosition * GetRotation();
	btVector3 position = btVector3(v3LocalPosition2.x, v3LocalPosition2.y, v3LocalPosition2.z);	 // ??
	const btVector3 b3Velocity = m_pRigidBody->getVelocityInLocalPoint(position);
	MtVector3 v3Velocity(b3Velocity.x(), b3Velocity.y(), b3Velocity.z());
	v3Velocity = v3Velocity * GetRotation().GetTranspose();
	return v3Velocity;
}

////////////////////////////////////////////////////////////////////////////////
// GetWorldVelocity

MtVector3 DyBody::GetWorldVelocity() const
{
    const btVector3 b3Velocity = m_pRigidBody->getLinearVelocity();
	MtVector3 v3Velocity( b3Velocity.x(), b3Velocity.y(), b3Velocity.z() );
	return v3Velocity;
}

////////////////////////////////////////////////////////////////////////////////
// GetWorldPosition

MtVector3 DyBody::GetWorldPosition( const MtVector3& v3LocalPosition ) const
{
	MtMatrix4 m4Transform = GetWorldTransform();
	MtVector3 v3Position = v3LocalPosition * m4Transform;
	return v3Position;
}

////////////////////////////////////////////////////////////////////////////////
// WorldVelocity

MtVector3 DyBody::GetWorldVelocity( const MtVector3& v3LocalPosition ) const
{
	btVector3 position = btVector3( v3LocalPosition.x, v3LocalPosition.y, v3LocalPosition.z );
    const btVector3 b3Velocity = m_pRigidBody->getVelocityInLocalPoint( position );
	MtVector3 v3Velocity( b3Velocity.x(), b3Velocity.y(), b3Velocity.z() );
	return v3Velocity;
}

////////////////////////////////////////////////////////////////////////////////
// SetLocalVelocity

void DyBody::SetLocalVelocity( const MtVector3& v3Velocity )
{
	MtVector3 v3LocalVelocity = v3Velocity * GetRotation();
	btVector3 b3Velocity( v3LocalVelocity.x, v3LocalVelocity.y, v3LocalVelocity.z );
	m_pRigidBody->setLinearVelocity( b3Velocity );
}

////////////////////////////////////////////////////////////////////////////////
// SetLocalVelocity

void DyBody::SetWorldVelocity( const MtVector3& v3Velocity )
{
	btVector3 b3Velocity( v3Velocity.x, v3Velocity.y, v3Velocity.z );
	m_pRigidBody->setLinearVelocity( b3Velocity );
}

////////////////////////////////////////////////////////////////////////////////
// GetLocalAngularVelocity

MtVector3 DyBody::GetLocalAngularVelocity() const
{
	MtVector3 v3LocalVelocity = GetWorldAngularVelocity() * GetRotation().GetTranspose();
	return v3LocalVelocity;
}

////////////////////////////////////////////////////////////////////////////////
// SetLocalAngularVelocity

void DyBody::SetLocalAngularVelocity( MtVector3 v3Velocity )
{
	v3Velocity *= GetRotation();
	btVector3 b3Velocity( v3Velocity.x, v3Velocity.y, v3Velocity.z );
	m_pRigidBody->setAngularVelocity( b3Velocity );
}

////////////////////////////////////////////////////////////////////////////////
// GetWorldAngularVelocity

MtVector3 DyBody::GetWorldAngularVelocity() const
{
	btVector3 b3WorldAngularVelocity = m_pRigidBody->getAngularVelocity();
	
	MtVector3 v3WorldVelocity = MtVector3( b3WorldAngularVelocity.getX(),
										   b3WorldAngularVelocity.getY(),
										   b3WorldAngularVelocity.getZ() );

	return v3WorldVelocity;
}

////////////////////////////////////////////////////////////////////////////////
// SetWorldAngularVelocity

void DyBody::SetWorldAngularVelocity( const MtVector3& v3Velocity )
{
	btVector3 b3Velocity( v3Velocity.x, v3Velocity.y, v3Velocity.z );
	m_pRigidBody->setAngularVelocity( b3Velocity );
}

////////////////////////////////////////////////////////////////////////////////
// Speed

BtFloat DyBody::GetSpeed() const
{
	return GetLocalVelocity().z;
}

////////////////////////////////////////////////////////////////////////////////
// Position

MtVector3 DyBody::GetPosition() const
{
	return GetWorldTransform().GetTranslation();
}

////////////////////////////////////////////////////////////////////////////////
// CenterOfMass

MtVector3 DyBody::GetCenterOfMass() const
{	
	btVector3 v3Point = m_pRigidBody->getCenterOfMassPosition();
	return MtVector3( v3Point.getX(), v3Point.getY(), v3Point.getZ() );
}

////////////////////////////////////////////////////////////////////////////////
// SetFriction

void DyBody::SetFriction( BtFloat friction )
{
	m_pRigidBody->setFriction( friction );
}

////////////////////////////////////////////////////////////////////////////////
// SetRestitution

void DyBody::SetRestitution( BtFloat restitution )
{
	m_pRigidBody->setRestitution( restitution );
}

////////////////////////////////////////////////////////////////////////////////
// SetDamping

void DyBody::SetDamping( BtFloat damping )
{
	m_pRigidBody->setDamping( damping, damping );
}

////////////////////////////////////////////////////////////////////////////////
// SetDamping

void DyBody::SetDamping( BtFloat lindamping, BtFloat angdamping  )
{
	m_pRigidBody->setDamping( lindamping, angdamping );
}

////////////////////////////////////////////////////////////////////////////////
// Stop

void DyBody::Stop()
{
	m_contacts.Empty();
	ClearForces();
	SetWorldAngularVelocity( MtVector3( 0, 0, 0 ) );
	SetWorldVelocity( MtVector3( 0, 0, 0 ) );
	m_pRigidBody->activate( BtTrue );
	m_pRigidBody->clearForces();
}

////////////////////////////////////////////////////////////////////////////////
// Update

void DyBody::Update()
{
	// Cache the dynamics world
	btDynamicsWorld* pDynamicsWorld = DyImpl::pWorld()->m_pDynamicsWorld;

	int numManifolds = pDynamicsWorld->getDispatcher()->getNumManifolds();

	m_contacts.Empty();

	for( BtU32 iManifold=0; iManifold<numManifolds; iManifold++ )
	{
		btPersistentManifold* contactManifold = pDynamicsWorld->getDispatcher()->getManifoldByIndexInternal( iManifold );
		const btCollisionObject* obA = static_cast<const btCollisionObject*>(contactManifold->getBody0());
		const btCollisionObject* obB = static_cast<const btCollisionObject*>(contactManifold->getBody1());

		BtU32 numContacts = contactManifold->getNumContacts();

		for( BtU32 iContact=0; iContact<numContacts; iContact++ )
		{
			btManifoldPoint const& pt = contactManifold->getContactPoint( iContact );

			if( ( m_pRigidBody == obA ) || ( m_pRigidBody == obB ) )
			{
				DyContact contact;
				btVector3 position;

				if( m_pRigidBody == obA )
				{
					position = pt.getPositionWorldOnA();
				}
				else if( m_pRigidBody == obB )
				{
					position = pt.getPositionWorldOnB();
				}
//				position -= m_pRigidBody->getCenterOfMassPosition();
//				position = position * m_pRigidBody->getOrientation().inverse();

				contact.m_v3Position = MtVector3( position.getX(), position.getY(), position.getZ() );

				if( m_pRigidBody == obA )
				{
					contact.m_pBody = (DyBody*)obB->getUserPointer();
				}
				else
				{
					contact.m_pBody = (DyBody*)obA->getUserPointer();
				}
				m_contacts.Add( contact );
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// GetDimension

MtVector3 DyBody::GetDimension()
{
	return m_v3Dimension;
}

////////////////////////////////////////////////////////////////////////////////
// Render

void DyBody::Render()
{
}
