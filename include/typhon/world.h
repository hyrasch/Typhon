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

			void reset(int index);
		};

		struct Grid
		{
			Vector3 origin = Vector3(0, 0, 0);

			unsigned int xExtent = 100;
			unsigned int yExtent = 100;
			unsigned int zExtent = 100;

			Object* locations;
			Vector3 oneOverCellSize = Vector3(0.1, 0.1, 0.1);

			unsigned int getLocationIndex(const Vector3& bSphere)
			{
				Vector3 square = bSphere.componentProduct(oneOverCellSize);
				return (unsigned int)(square.x) + xExtent * (unsigned int)(square.z) + xExtent * zExtent * (unsigned int)(square.y);
			}
		};

		World();

		std::vector<Object> listObject;

		Box myBox;
		Box myBox2;
		
		Grid grid;

		/*Car myCar;
		Car myCar2;*/

		std::vector<std::vector<Object>> CheckPotentialCollision();

		void Update(real duration);
	};
}

#endif	TYPHON_WORLD_H
