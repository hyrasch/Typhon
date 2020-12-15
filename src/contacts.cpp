#include <typhon/contacts.h>
#include <memory.h>
#include <assert.h>

using namespace typhon;


void Contact::setBodyData(RigidBody one, RigidBody two,
    real friction, real restitution)
{
    Contact::rb[0] = &one;
    Contact::rb[1] = &two;
    Contact::friction = friction;
    Contact::restitution = restitution;
}