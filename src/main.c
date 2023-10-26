/**
 * @file main.c
 * @brief Jeu du mastermind en C
 * @version 1.0
 * @date 2023-10-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */

/* Inclusions de base */

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

/* Inclusions supplémentaires */

#include <stdlib.h>
#include <time.h>

/* constantes */

#define NombreEssaies  10   /* nb de propositions maximales */
#define LongueurCode  5    	/* longueur du code */
const int NombreCouleurs = 4; /* pions de valeur 0 à NombreCouleurs-1 */

/* type reponse */

/**
 * @brief type pour les réponses
 * 
 * @struct s_rep
 */
struct s_rep {
	int bienPlacee;
	int bonneCouleur;
};
typedef struct s_rep reponse;

/* variables globales */

int Plateau[NombreEssaies][LongueurCode] = {{0}}; /* plateau des propositions */
reponse Rep[NombreEssaies] = {{.bienPlacee = 0, .bonneCouleur = 0}}; /* réponses associées */
bool modeIA = false; /* faire jouer l'IA ? */
int compteurCree = 0; /* compte les solutions généré par l'I.A. */

/* déclaration de fonctions */

void afficheTab(int tab[], int n); // affiche les valeurs d'un tableau sur une seule ligne
void affichePlateau(int ncoups); // affiche le plateau de jeu, sachant que ncoups ont été joués
void creeCombinaison(int tab[], int n); // remplit un tableau avec des valeurs aléatoires entre 0 et NombreCouleurs-1
void choixCombinaison(int tab[], int n); // remplit un tableau avec des valeurs indiquées par l'utilisateur
void choixCombinaisonIA(int tab[], int n, int nbcoups, int combiDejaCree[][LongueurCode], int NbMaxPossibilites); // remplit un tableau avec les valeurs indiquées pat l'IA
reponse calculeReponse(int code[], int propositionJoueur[], int n); // renvoie une réponse pour un code caché et une proposition
int powi(int a, int b); // fonction pow avec des int

/* programme principal */

int main(int argc, char *argv[]) {

	srand(time(NULL));	/* Initialisation  de l'aléatoire*/

	int NbMaxPossibilites = powi(NombreCouleurs, LongueurCode);
	int combiDejaCree[NbMaxPossibilites][LongueurCode];

	printf("Jeu du mastermind\n");

	printf("Choisissez votre mode de jeu (1 pour 1 joueur, 2 pour I.A.) :\n");
	int x;
    scanf("%d", &x);

	if (x==1){
		modeIA = false;
	} else if (x==2) {
		modeIA = true;
	}
	
    int code[LongueurCode];
	creeCombinaison(code, LongueurCode);
	printf("Code secret: ");
    afficheTab(code, LongueurCode);

	int compteur = 0;
	bool victoire = false;

	while (!victoire && compteur<NombreEssaies)
	{
		printf("Choisissez votre combinaison:\n");
		int combinaisonJoueur[LongueurCode];
		if (modeIA)
		{
			choixCombinaisonIA(combinaisonJoueur, LongueurCode, compteur, combiDejaCree, NbMaxPossibilites);
		} else {
			choixCombinaison(combinaisonJoueur, LongueurCode);
		}
		
		reponse reponseChoixJoueur = calculeReponse(code, combinaisonJoueur, LongueurCode);

		printf(
			"[i] Tentative %d : il y a %d bien Placé(s), %d bonne(s) Couleur(s) \n",
			compteur+1,
			reponseChoixJoueur.bienPlacee,
			reponseChoixJoueur.bonneCouleur
		);

		for (int i = 0; i < LongueurCode; i++)
		{
			Plateau[compteur][i] = combinaisonJoueur[i];
		}

		Rep[compteur] = reponseChoixJoueur;

		
		printf("[i] Le plateau actuel :\n");
		affichePlateau(compteur);

		compteur += 1;

		if (reponseChoixJoueur.bienPlacee == LongueurCode){
			victoire = true;
		};
	}

	if (victoire){
		printf("BRAVO ! Vous avez gagne en, %d coups \n", compteur);
	} else {
		printf("PERDU ... la combinaison était \n");
		afficheTab(code, LongueurCode);
	}

	return 0;
}

//////// Implémentation des fonctions auxiliaires

/**
 * @brief affiche les valeurs d'un tableau
 * 
 * @param[in, out] tab tableau d'entiers
 * @param[in] n taille de tab
 */
void afficheTab(int tab[], int n) {
	for (int i = 0; i < n; i += 1){
		printf("%d ",tab[i]);
	}
	printf("\n");
}

/**
 * @brief affiche le plateau de jeu
 * 
 * @param ncoups nombre de coups déjà joué
 */
void affichePlateau(int ncoups){
	assert(ncoups <= NombreEssaies); // progr. défensive
	for (int i = 0; i <= ncoups; i++)
	{
		printf("-> ");
		afficheTab(Plateau[i], LongueurCode);
	}
	
}


/**
 * @brief remplit aléatoirement tab
 * 
 * @param[in, out] tab tableau d'entiers
 * @param[in] n taille de tab
 */
void creeCombinaison(int tab[], int n) {
	for(int i = 0; i<n; i+=1){
		int randVal = rand();
		randVal %= NombreCouleurs;
		tab[i] = randVal;
	}
}

/**
 * @brief proposition manuelle 
 * 
 * @param[in, out] tab tableau d'entiers
 * @param[in] n taille de tab
 */
void choixCombinaison(int tab[], int n) {
	for (int i = 0; i < n; i++)
	{
		printf("Entrez un nombre entre 0 et %d :\n", NombreCouleurs-1);
		int x;
    	scanf("%d", &x);
		assert(x>=0);
		assert(x < NombreCouleurs);
		tab[i] = x;
	}
}

/**
 * @brief proposition de l'IA, sachant que ncoups ont été joués
 * 
 * @param[in, out] tab tableau d'entiers
 * @param[in] n taille de tab
 * @param ncoups nombre de coups déjà joué
 * @param combiDejaCree liste des combinaisons déjà généré par l'I.A.
 * @param NbMaxPossibilites Nombre maximum de code
 */
void choixCombinaisonIA(int tab[], int n, int ncoups, int combiDejaCree[][LongueurCode], int NbMaxPossibilites){
	bool valide = false;
	int combinaisonAleatoire[LongueurCode];
	int count = 0;
	
	while (!valide)
	{

		assert (compteurCree<=NbMaxPossibilites);

		creeCombinaison(combinaisonAleatoire, LongueurCode);

		bool restartVal = false;

		for (int i = 0; i < compteurCree; i++)
		{

			bool alreadyUsed = true;

			for (int j = 0; j < LongueurCode; j++)
			{
				if (combiDejaCree[i][j] != combinaisonAleatoire[j]){
					alreadyUsed = false;
				}
			}

			if (alreadyUsed)
			{
				restartVal = true;
			}
		}

		if (restartVal)
		{
			continue;
		}
		

		printf("[+] New Combi Found!");
		afficheTab(combinaisonAleatoire, LongueurCode);
		printf("[i] CountCreated is now: %d", compteurCree);
		printf("\n");

		for (int i = 0; i < LongueurCode; i++)
		{
			combiDejaCree[compteurCree][i] = combinaisonAleatoire[i];
		}

		compteurCree += 1;


		valide = true;
		for (int i = 0; i < ncoups; i++)
		{
			reponse calcRep = calculeReponse(combinaisonAleatoire, Plateau[i], LongueurCode);
			if (
				calcRep.bienPlacee != Rep[i].bienPlacee ||
				calcRep.bonneCouleur != Rep[i].bonneCouleur
			){
				valide = false;
				break;
			}
		}

		count += 1;
	}

	for (int i = 0; i < n; i++)
	{
		tab[i] = combinaisonAleatoire[i];
	}
}

/**
 * @brief calcule la réponse à donner
 * 
 * @param code Code secret
 * @param propositionJoueur Proposition du joueur
 * @param n taille des 2 tableaux
 * @return reponse 
 */
reponse calculeReponse(int code[], int propositionJoueur[], int n){
	assert(n == LongueurCode);
	reponse res;
	int codeTemp[LongueurCode];
	int propTemp[LongueurCode];

	for (int i = 0; i < LongueurCode; i++)
	{
		codeTemp[i] = code[i];
		propTemp[i] = propositionJoueur[i];

	}

	int nombreBienPlacee = 0;

	for (int i = 0; i < LongueurCode; i++)
	{
		if(codeTemp[i] == propTemp[i]){
			nombreBienPlacee += 1;
			codeTemp[i] = -1;
			propTemp[i] = -1;
		}
	}
	
	int nombreBonneCouleur = 0;

	for (int i = 0; i < LongueurCode; i++)
	{
		if(codeTemp[i] != -1){
			for (int j = 0; j < LongueurCode; j++)
			{
				if (codeTemp[i] == propTemp[i]){
					nombreBonneCouleur += 1;
					codeTemp[i] = -1;
					propTemp[i] = -1;
					break;
				}
			}
			
		}
	}

	assert(nombreBienPlacee+nombreBonneCouleur <= LongueurCode);

	res.bienPlacee = nombreBienPlacee;
	res.bonneCouleur = nombreBonneCouleur;

	return res;
}

/**
 * @brief Fonction "Power" avec deux entiers en entrée et un en sortie
 * 
 * @param a	entier, base
 * @param b entier, exposant
 * @return int 
 */
int powi(int a, int b){
	int acc = 1;
	for (int i = 0; i < b; i++)
	{
		acc *= a;
	}
	return acc;	
} 
