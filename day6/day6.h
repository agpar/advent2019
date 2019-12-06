#define TABLE_SIZE 20

typedef struct OrbitList {
	char child[4];
	struct OrbitList *next;
} OrbitList;

typedef struct HashedOrbits {
	char parent[4];
	OrbitList *orbits;
	struct HashedOrbits *next;
} HashedOrbits;
