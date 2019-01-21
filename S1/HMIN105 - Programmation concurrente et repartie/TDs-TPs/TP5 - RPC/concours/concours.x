const STR_MAX_LEN = 512;

struct client {
	char nom[STR_MAX_LEN];
	char prenom[STR_MAX_LEN];
	char adresse[STR_MAX_LEN];
	float age;
};

typedef client liste_clients<>;

enum ACTION_CLIENT{
	AJOUTER,
	LISTER,
	SUPPRIMER,
	COMPTER
};

program CONCOURS_PROG {
	version CONCOURS_VERS {
		bool ajouter_client(client) = 1;
		liste_clients lister_clients() = 2;
		bool supprimer_client(string) = 3;
		int compter_clients(client) = 4;
	} = 1;
} = 1;