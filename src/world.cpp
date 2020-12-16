#include <typhon/world.h>

using namespace typhon;

World::World() {
	myBox.init();

	myWall.init(Vector3(-1, 0, 0), wallSize, Vector3(-20,2,0));
	myWall2.init(Vector3(1, 0, 0), wallSize, Vector3(20, 2, 0));
	myWall3.init(Vector3(0, 0, 1), wallSize, Vector3(0, 2, -20));
	myWall4.init(Vector3(0, 0, -1), wallSize, Vector3(0, 2, 20));

	listObject.clear();
	listObject.push_back(myBox);
	listObject.push_back(myWall);
	listObject.push_back(myWall2);
	listObject.push_back(myWall3);
	listObject.push_back(myWall4);

	collisionData.firstContact = contacts;
	outOfSim = false;
}

void World::Box::init() {

	colBox.body.setMass(2.f);
	colBox.body.setDamping(0.8f, 0.8f);
	colBox.body.setPosition(0, 2, 0);
	colBox.body.setOrientation(1, 0, 0, 0);
	colBox.body.setRotation(0, 0, 0);
	colBox.body.setAcceleration(0, 0, 0);
	colBox.body.setVelocity(0, 0, 0);

	registry.registrations.clear();

	colBox.body.setInverseInertiaTensor(Matrix3(colBox.body.getMass() / 24.0f, 0, 0, 0, colBox.body.getMass() / 24.0f, 0, 0, 0, colBox.body.getMass() / 24.0f));
	colBox.body.calculateInertiaTensorWS();

	colBox.halfSize = Vector3(0.5,0.5,0.5);

	bSphere.centre = colBox.body.getPosition();
	colPrimitive = colBox;
}

void World::Box::reset() {

	colBox.body.setPosition(0, 2, 0);
	colBox.body.setOrientation(1, 0, 0, 0);
	colBox.body.setRotation(0, 0, 0);
	colBox.body.setAcceleration(0, 0, 0);
	colBox.body.setVelocity(0, 0, 0);

	registry.registrations.clear();

	colBox.body.setInverseInertiaTensor(Matrix3(colBox.body.getMass() / 24.0f, 0, 0, 0, colBox.body.getMass() / 24.0f, 0, 0, 0, colBox.body.getMass() / 24.0f));
	colBox.body.calculateInertiaTensorWS();
}

void World::Box::stop() {
	colBox.body.setMass(2.f);
	colBox.body.setDamping(0.8f, 0.8f);
	colBox.body.setRotation(0, 0, 0);
	colBox.body.setAcceleration(0, 0, 0);
	colBox.body.setVelocity(0, 0, 0);

	registry.registrations.clear();

	colBox.body.setInverseInertiaTensor(Matrix3(colBox.body.getMass() / 24.0f, 0, 0, 0, colBox.body.getMass() / 24.0f, 0, 0, 0, colBox.body.getMass() / 24.0f));
	colBox.body.calculateInertiaTensorWS();
}

void World::Wall::init(Vector3 normal, real offset, Vector3 centre)
{
	colPlane.normal = normal;
	colPlane.offset = offset;

	bSphere.centre = centre;
}

std::vector<int> World::CheckPotentialCollision()
{
	std::vector<int> potentialCollision;

	for (int i = 1; i < listObject.size(); i++)
	{
		if (getGridPos(listObject[i].bSphere) == getGridPos(listObject[0].bSphere))
		{
			potentialCollision.push_back(i);
		}
	}

	return potentialCollision;
}

void World::generateContacts()
{
	collisionData.reset(maxContacts);

	for (int i = 0; i < potentialContact.size(); i++)
	{
		if (collisionData.hasContactsLeft())
		{
			switch (potentialContact[i])
			{
			case 1:
			{
				if (collisionDetector.boxXhalfSpace(myBox.colBox, myWall.colPlane, &collisionData) != 0)
				{
					myBox.stop();
					outOfSim = true;
					for (int i = 0; i < collisionData.contactCount; i++)
					{
						std::cout << "Point d'impact" << std::endl;
						std::cout << "x :" << collisionData.firstContact[i].contactPoint.x << 
							"y :" << collisionData.firstContact[i].contactPoint.y <<
							"z :" << collisionData.firstContact[i].contactPoint.z << std::endl;

						std::cout << "Distance penetration :" << collisionData.firstContact[i].penetration << std::endl;
					}
				}
			}
			break;

			case 2:
			{
				if (collisionDetector.boxXhalfSpace(myBox.colBox, myWall2.colPlane, &collisionData) != 0)
				{
					myBox.stop();
					outOfSim = true;
					for (int i = 0; i < collisionData.contactCount; i++)
					{
						std::cout << "Point d'impact" << std::endl;
						std::cout << "x :" << collisionData.firstContact[i].contactPoint.x <<
							"y :" << collisionData.firstContact[i].contactPoint.y <<
							"z :" << collisionData.firstContact[i].contactPoint.z << std::endl;

						std::cout << "Distance penetration :" << collisionData.firstContact[i].penetration << std::endl;
					}
				}
			}
			break;

			case 3:
			{
				if (collisionDetector.boxXhalfSpace(myBox.colBox, myWall3.colPlane, &collisionData) != 0)
				{
					myBox.stop();
					outOfSim = true;
					for (int i = 0; i < collisionData.contactCount; i++)
					{
						std::cout << "Point d'impact" << std::endl;
						std::cout << "x :" << collisionData.firstContact[i].contactPoint.x <<
							"y :" << collisionData.firstContact[i].contactPoint.y <<
							"z :" << collisionData.firstContact[i].contactPoint.z << std::endl;

						std::cout << "Distance penetration :" << collisionData.firstContact[i].penetration << std::endl;
					}
				}
			}
			break;

			case 4:
			{
				if (collisionDetector.boxXhalfSpace(myBox.colBox, myWall4.colPlane, &collisionData) != 0)
				{
					myBox.stop();
					outOfSim = true;
					for (int i = 0; i < collisionData.contactCount; i++)
					{
						std::cout << "Point d'impact" << std::endl;
						std::cout << "x :" << collisionData.firstContact[i].contactPoint.x <<
							"y :" << collisionData.firstContact[i].contactPoint.y <<
							"z :" << collisionData.firstContact[i].contactPoint.z << std::endl;

						std::cout << "Distance penetration :" << collisionData.firstContact[i].penetration << std::endl;
					}
				}
			}
			break;
			}
		}
	}
}

Vector3 World::getGridPos(BoundingSphere bSphere)
{
	return Vector3((int)(bSphere.centre.x * grid.oneOverCellSize.x), (int)(bSphere.centre.y / grid.oneOverCellSize.y), (int)(bSphere.centre.z / grid.oneOverCellSize.z));
}

void World::Update(real duration) {
	listObject.clear();
	listObject.push_back(myBox);
	listObject.push_back(myWall);
	listObject.push_back(myWall2);
	listObject.push_back(myWall3);
	listObject.push_back(myWall4);

	myBox.bSphere.centre = myBox.colBox.body.getPosition();
	myBox.colPrimitive.body = myBox.colBox.body;
	myBox.colBox.body.clearAccumulators();
	myBox.registry.updateForces(duration);
	myBox.colBox.body.integrate(duration);
}
