#ifndef TYPHON_WORLD_H
#define TYPHON_WORLD_H

#include "rigidBody.h"
#include "forceGen.h"
#include "collision.h"

#include <list>
#include <vector>
#include <iostream>

namespace typhon {
	

	class World
	{
	public:
		/*struct Car
		{
			RigidBody body;
			ForceRegistry registry;

			void reset(int z);
		};*/

		struct Object {
			BoundingSphere bSphere;
			ColPrimitive colPrimitive;
			Vector3 GridPos;
		};

		struct Box : public Object
		{
			ColBox colBox;
			ForceRegistry registry;

			void init();
			void reset();
			void stop();
		};

		struct Wall : public Object
		{
			ColPlane colPlane;

			void init(Vector3 normal, real offset, Vector3 centre);
		};

		struct Grid
		{
			Vector3 origin = Vector3(-40, 0, 40);

			unsigned int xExtent = 40;
			unsigned int yExtent = 40;
			unsigned int zExtent = 40;

			Object* locations;
			Vector3 oneOverCellSize = Vector3(1,1,1);

			unsigned int getLocationIndex(const Vector3& bSphere)
			{
				Vector3 square = bSphere.componentProduct(oneOverCellSize);
				return (unsigned int)(square.x) + xExtent * (unsigned int)(square.z) + xExtent * zExtent * (unsigned int)(square.y);
			}


		};

		World();

		std::vector<Object> listObject;

		int wallSize = 20;

		Box myBox;

		Wall myWall;
		Wall myWall2;
		Wall myWall3;
		Wall myWall4;
		
		Grid grid;

		const static unsigned maxContacts = 256;
		CollisionData collisionData;
		CollisionDetector collisionDetector;
		Contact contacts[maxContacts];

		std::vector<int> potentialContact;
		
		bool outOfSim;

		std::vector<int> CheckPotentialCollision();
		void generateContacts();
		Vector3 getGridPos(BoundingSphere bsphere);

		void Update(real duration);
	};
}

#endif	TYPHON_WORLD_H
