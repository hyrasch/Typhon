#ifndef TYPHON_WORLD_H
#define TYPHON_WORLD_H

#include "rigidBody.h"
#include "forceGen.h"

namespace typhon {

	class World
	{
	public:
		struct BodyRegistration
		{
			RigidBody* body;
			BodyRegistration* next;
		};

		

		struct Car
		{
			RigidBody body;
			ForceRegistry registry;

			Vector3 massCenter = Vector3(0, 20, 10);

			Vector3 frontUpperLeft = massCenter - Vector3(0.5, -0.5 , 0.5);
			Vector3 frontUpperRight = massCenter - Vector3(0.5, -0.5, -0.5);
			Vector3 frontLowerRight = massCenter - Vector3(0.5, 0.5, -0.5);
			Vector3 frontLowerLeft = massCenter - Vector3(0.5, 0.5, 0.5);
			Vector3 backUpperLeft = massCenter - Vector3(-0.5, -0.5, 0.5);
			Vector3 backUpperRight = massCenter - Vector3(-0.5, -0.5, -0.5);
			Vector3 backLowerRight = massCenter - Vector3(-0.5, 0.5, -0.5);
			Vector3 backLowerLeft = massCenter - Vector3(-0.5, 0.5 , 0.5);

			void updatePos(World world);

		};

		World();

		Car myCar;

		BodyRegistration* firstBody;

		void startFrame();

		void runPhysics(real duration);

	};
}

#endif	TYPHON_WORLD_H
