# But
Créer un moteur physique dont les objets sont des corps rigides et dont la résolution de collisions se fait de manière itérative à l’aide d’impulsions. Ce but sera atteint graduellement au fil de la session. La conception se fera en quatre (4) phases itératives :

Phase 1 : construction d’un moteur élémentaire de gestion de particules;

Phase 2 : gestion d’objets formés de plusieurs masses par l’ajout de différentes forces et contraintes;

Phase 3 : gestion des corps rigides par l’ajout de la physique rotationnelle;

Phase 4 : ajout d’un système élémentaire de résolution de collisions.

# Exigences
Le moteur doit être codé en C++ et utiliser le paradigme de programmation orienté objet. Bien que le rendu graphique ne soit pas priorisé, il est fortement conseillé d’incorporer une interface graphique à votre projet. Peut importe la solution choisie, elle doit être indépendante de la plateforme. Ex: OpenGL, Free Glut, SDL, SFML. Voir https://en.cppreference.com/w/cpp/links/libs pour une liste exhaustive.

Le projet doit se faire en équipe de trois (3) ou quatre (4) personnes. Tout plagiat entraînera automatique la note de zéro (0).

Pondération et évaluation
Chaque phase sera évaluée indépendamment lors de quatre (4) présentations orales se déroulant au bureau du professeur . Le code source sera également évalué et devra répondre aux critères de bonnes pratiques de programmation : code lisible, structuré et bien commenté.

La pondération de chaque phase est de 25%.

Livrable et échéanciers
Pour chaque phase, vous devez remettre :

Un document Identification.txt contenant vos noms, prénoms et codes permanents

Vos fichiers sources;

Un fichier ReadMe.txt expliquant comment compiler et faire fonctionner votre programme;

Tout autre fichier nécessaire à l’exécution de votre programme

Un journal de développement, format PDF, d’environ une (1) à deux (2) pages expliquant les difficultés rencontrées, les astuces de programmation que vous avez appris ainsi que la justification des différents choix faits durant le développement.

Ces documents doivent être placés dans un dossier et remis directement dans le dépôt Moodle approprié.

Les dates de remise sont indiquées dans le plan de cours.

# Description des différentes phases du projet
# Phase 1

Le but de cette phase est de créer un moteur physique simple de gestion de particules à partir des notions vues au chapitres 1 et 2. En particulier, vous devez :

Implémenter une classe Vecteur3D avec toutes les méthodes vues au chapitre 1: norme, norme au carré, normalisation, multiplication par un scalaire, addition, soustraction, produit par composantes, produit scalaire, produit vectoriel, 

Implémenter une classe Particule comprenant des accesseurs pour l’attribut InverseMasse.

Implémenter un intégrateur afin de mettre à jour la position et la vélocité de chaque particule à chaque frame. Celui-ci peut être implémenté en tant que méthode void integrer(float temps) de la classe Particule.

Réaliser un petit jeu de tir balistique où on peut choisir entre différents projectiles (e.g. balles, boulets, laser, boule de feu) et effectuer un tir. Chaque projectile doit avoir sa propre vélocité initiale et sa propre masse. Le frottement doit être négligeable (près de 1).

Remarque: vous pouvez calculer la durée d'une frame de deux façons: soit en utilisant une valeur fixe (e.g. 60 fps), soit en mesurant la longueur de chaque frame afin de l'appliquer à la prochaine. Notez que la seconde méthode est préférable car reproductible.


# Phase 2

Le but de cette phase est de poursuivre le développement du moteur physique réalisé lors de la phase 1 en lui ajoutant les notions vues au chapitres 3, 4 et 5. Ainsi, le moteur permettra la gestion d’amas de particules. En particulier, vous devez :

Implémenter, à l’aide d’une interface, des générateurs pour les forces suivantes : gravité, friction et ressorts. Notez que tous les types de ressorts vus en classe doivent être implémentés.

Implémenter un système de détection de collisions. Ce système devra détecter les types de collisions suivants : interpénétrations, contacts au repos, tiges et câbles.

Implémenter un système de résolution de collisions basé sur les impulsions.

Réaliser un petit jeu où l’on déplace un amas de particule, i.e. blob. Chaque particule est représentée par un cercle. Les particules sont reliées entre elles en utilisant des ressorts classiques. Par contre, ces liens ont une limite d’élasticité au-delà de laquelle ils se comportent comme des câbles.

Vous aurez des points boni si votre blob peut se séparer en plusieurs morceaux et se fusionner.

Exemple de blob : le jeu Loco Roco de SCE Japan Studio.

Suggestion d’implémentation : construire une classe World représentant le game world et qui contient, entre autre chose, un std ::vector avec toutes les particules actives dans le jeu. Ensuite, à chaque frame, pour chaque particule, appeler le générateur de forces, intégrer et accumuler les contacts. Finalement, passer ces contacts au système de résolution de collisions.

# Phase 3

Le but de cette phase est de spécialiser le moteur physique des phases 1 et 2 par l’ajout de la physique rotationnelle, implémentant ainsi les notions vues au chapitres 6, 7 et 8. Ce faisant, le moteur permettra la gestion des corps rigides. Notez toutefois que la gestion des collisions n’est pas à implémenter dans cette phase. En particulier, vous devez :

Implémenter une classe CorpsRigide avec les attributs et méthodes pertinentes.

Implémenter les classes Matrix3, Matrix4 et Quaternion avec les attributs et méthodes pertinentes.

Implémenter un intégrateur physique complet permettant la mise à jour des objets de type CorpsRigide.

Réaliser deux courtes démos :

une première où un objet de forme irrégulière est lancé en l’air avec des vélocités linéaires et angulaires non nulles. Le centre de masse ainsi que le mouvement de rotation doivent être clairement visibles.

Une deuxième où deux boîtes représentant deux voitures entrent en collision. Cette collision doit obligatoirement entraîner une rotation pour au moins une des deux voitures. Vous pouvez « hardcoder » le moment de la collision et le point d’impact.

# Phase 4

Le but de cette phase est d’ajouter au moteur physique de la phase 3 un système simple de détection de collisions, implémentant ainsi les notions vues aux chapitres 9 et 10. Ce faisant, le moteur permettra la détection de collisions entre une boîte et un plan. Notez toutefois que vous n’avez pas à implémenter un système de gestion de collisions.

Pour cette phase, vous devez produire une démo où une boîte rectangulaire est lancée dans une pièce rectangulaire fermée. La pièce doit être assez grande pour bien voir le déplacement de la boîte. Les vélocités linéaire et angulaire initiales doivent être non nulles. Lorsque la boîte entre en collision avec un des murs, la démo s’arrête. Vous devez alors générer la collision, c’est-à-dire produire le CollisionData approprié et en afficher le contenu (i.e. point d’impact, normale et interpénétration).

Pour ce faire, vous devez, en particulier:

implémenter un système de détection de collisions (broad phase) à l’aide d’une des structures vues en classe (e.g. octree, BSP, BVH, etc.). Il est conseillé d’utiliser une sphère comme volume englobant.

implémenter un système de génération de collisions (narrow phase) boîte-plan tel que vu en classe.

implémenter toutes les classes pertinentes à la réalisation de ce projet : ocTree, Contact, CollisionData, Primitive, Plane, Sphere, Box, etc.
