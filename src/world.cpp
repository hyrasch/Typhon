#include <typhon/world.h>

using namespace typhon;

World::World() {
	myBox.init();
	myBox2.init();

	myWall.init(Vector3(1, 0, 0), wallSize);
	myWall2.init(Vector3(-1, 0, 0), wallSize);
	myWall3.init(Vector3(0, 0, 1), wallSize);
	myWall4.init(Vector3(0, 0, -1), wallSize);

	myBox2.colBox.body.position = Vector3(0, 2, 200);
	myBox2.bSphere.centre = myBox2.colBox.body.getPosition();
	myBox2.colPrimitive = myBox2.colBox;

	listObject.clear();
	listObject.push_back(myBox);
	listObject.push_back(myBox2);
	listObject.push_back(myWall);
	listObject.push_back(myWall2);
	listObject.push_back(myWall3);
	listObject.push_back(myWall4);
}

/*void World::Car::reset(int z) {
	body.setMass(2.f);
	body.setDamping(0.8f, 0.8f);
	body.setPosition(0, 2, z);
	body.setOrientation(1, 0, 0, 0);
	body.setRotation(0, 0, 0);
	body.setAcceleration(0, 0, 0);
	body.setVelocity(0, 0, 0);

	registry.registrations.clear();

	body.setInverseInertiaTensor(Matrix3(body.getMass() / 24.0f, 0, 0, 0, body.getMass() / 24.0f, 0, 0, 0, body.getMass() / 24.0f));
	body.calculateInertiaTensorWS();
}*/

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

void World::Wall::init(Vector3 normal, real offset)
{
	colPlane.normal = normal;
	colPlane.offset = offset;

	bSphere.centre = Vector3(-normal.x*offset, -normal.y*offset, -normal.z*offset);
}

void World::Wall::reset(int index) {

	switch (index)
	{
	case 1:
	{
		
	}
		break;

	case 2:
	{
		
	}
		break;

	case 3:
	{
		
	}
		break;

	case 4:
	{
		
	}
		break;

	case 5:
	{

	}

	}

}

std::vector<std::vector<World::Object>> World::CheckPotentialCollision()
{
	std::vector<std::vector<Object>> potentialCollision;
	std::vector<Object> bodyPair;

	for (int i = 0; i < listObject.size(); i++)
	{
		for (int j = 0; j < listObject.size(); j++)
		{
			if (i != j)
			{
				if (grid.getLocationIndex(listObject[i].bSphere.centre) == grid.getLocationIndex(listObject[j].bSphere.centre))
				{
					std::cout << i << j << std::endl;
					myBox.stop();
					myBox2.stop();
					bodyPair.push_back(listObject[i]);
					bodyPair.push_back(listObject[j]);
					potentialCollision.push_back(bodyPair);
					bodyPair.clear();
				}
			}
		}
	}

	return potentialCollision;
}

void World::Update(real duration) {
	listObject.clear();
	listObject.push_back(myBox);
	listObject.push_back(myBox2);
	listObject.push_back(myWall);
	listObject.push_back(myWall2);
	listObject.push_back(myWall3);
	listObject.push_back(myWall4);

	myBox.bSphere.centre = myBox.colBox.body.getPosition();
	myBox.colPrimitive = myBox.colBox;
	myBox.colBox.body.clearAccumulators();
	myBox.registry.updateForces(duration);
	myBox.colBox.body.integrate(duration);

	myBox2.bSphere.centre = myBox2.colBox.body.getPosition();
	myBox2.colPrimitive = myBox2.colBox;
	myBox2.colBox.body.clearAccumulators();
	myBox2.registry.updateForces(duration);
	myBox2.colBox.body.integrate(duration);
}