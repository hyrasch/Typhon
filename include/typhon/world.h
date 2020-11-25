#ifndef TYPHON_WORLD_H
#define TYPHON_WORLD_H

#include "rigidBody.h"
#include "forceGen.h"

namespace typhon {

	class World
	{
	public:

		struct Car
		{
			RigidBody body;
			ForceRegistry registry;

			void reset(int z);
		};

		World();

		void Update(real duration);

		Car myCar;

		Car myCar2;
	};
}

#endif	TYPHON_WORLD_H
