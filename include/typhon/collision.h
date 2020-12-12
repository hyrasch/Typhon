#ifndef TYPHON_COLLISION_H
#define TYPHON_COLLISION_H

#include "contacts.h"

namespace typhon {
	// Primitive des collisions
	class ColPrimitive
	{
	public:
		// Le rigidbody associé à la collision
		RigidBody* body;
		// La matrice d'offset à partir de la matrice du rigidbody
		Matrix4 offset;

		Vector3 getAxis(unsigned i) const {
			return transform.getAxisVector(i);
		}

		const Matrix4& getTransform()const {
			return transform;
		}

	private:
		Matrix4 transform;
	};

	// Primitive sphérique
	class ColSphere : public ColPrimitive
	{
	public:
		// Rayon de la sphère
		real radius;
	};

	class ColPlane :public ColPrimitive
	{
	public:
		// Normale du plan
		Vector3 normal;
		// Offset p/r à l'origine
		real offset;
	};

	class ColBox :public ColPrimitive
	{
	public:
		// Moitié de dimensions permettant de retrouver tous les sommets
		Vector3 halfSize;
	};

	// Structure contenant les informations essentielles lors de collision(s)
	struct CollisionData
	{
		// Premier contact de la liste (sert d'accès)
		Contact* firstContact;
		// Tableau des contacts
		Contact* contacts;
		// Nombre de contacts restant
		int contactsLeft;
		// Nombre total de contacts enregistrés
		unsigned contactCount;
		// Valeur de friction sortant d'une collision
		real friction;
		// Valeur de restitution sortant d'une collision
		real restitution;

		// Reste-t-il des contacts à gérer
		bool hasContactsLeft() { return contactsLeft > 0; }
		// Ajoute à la structure une quantité donnée de contact
		void addContacts(unsigned nb) {
			// Mise à jour des données
			contactsLeft -= nb;
			contactCount += nb;

			// Déplacement dans le tableau
			contacts += nb;
		}
	};

	// Détecteur de collision contenant des méthodes de génération de "collision data"
	// typiques entre primitives
	class CollisionDetector
	{
	public:
		// Collision entre 2 sphères
		unsigned sphereXsphere(const ColSphere& first, const ColSphere& sec, CollisionData* data);
		// Collision entre sphère et semi plan
		unsigned sphereXhalfSpace(const ColSphere& sphere, const ColPlane& plane, CollisionData* data);
		// Collision entre boîte et semi plan
		unsigned boxXhalfSpace(const ColBox& box, const ColPlane& plane, CollisionData* data);
	};
}

#endif TYPHON_COLLISION_H
