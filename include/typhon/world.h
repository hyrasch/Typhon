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

			void reset(int z);
		};

		World();

		Car myCar;

		Car myCar2;

		BodyRegistration* firstBody;

		void startFrame();

		void runPhysics(real duration);

		

	};
}

#endif	TYPHON_WORLD_H
