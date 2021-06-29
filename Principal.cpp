/**
* \file Principal.cpp
* \brief Fichier test pour le dictionnaire des synonymes
* \author IFT-2008
* \version 0.5
* \date avril 2021
*
* Travail pratique numéro 3
*
*/

#include "DicoSynonymes.h"
using namespace std;
using namespace TP3;

int main()
{
	int nb = 1;				//Choix de l'utilisateur dans le menu initialisé à 1.
	string radical;			//Chaîne de caractères représentant un radical.
	string flexion;			//Chaîne de caractères représentant une flexion d'un radical.
	string synonyme;		//Chaîne de caractères représentant un synonyme d'un radical.
	DicoSynonymes dicoTest;	//Le dictionnaire utilisé pour les tests de base.
	DicoSynonymes dicoSyn;	//Le dictionnaire utilisé pour le chargement à partir d'un fichier texte.
	ifstream EntreeFichier;	//Flux d'entrée


	cout << "Bienvenu dans votre dictionnaire de synonymes!" << endl;
	//Affichage du menu
	cout << "------------------ Menu --------------------------------" << endl;
	cout << "1 - Ajouter un radical au dictionnaire." << endl;
	cout << "2 - Ajouter une flexion a un radical." << endl;
	cout << "3 - Ajouter un synonyme a un radical." << endl;
	cout << "4 - Supprimer une flexion d'un radical." << endl;
	cout << "5 - Supprimer un synonyme d'un radical." << endl;
	cout << "6 - Supprimer un radical du dictionnaire." << endl;
	cout << "7 - Charger un dictionnaire a partir d'un fichier texte." << endl;
	cout << "8-  Transformer une chaîne en utilisant le dictionnaire." << endl;
	cout << "0 - Quitter." << endl;
	cout << "--------------------------------------------------------" << endl;

	while (nb != 0)
	{

		do
		{
			cout << endl;
			cout << "Entrer s.v.p. votre choix (0 a 8):? ";
			cin >> nb;
			if(nb <0 || nb>8)
			{
				cout << "***Option invalide!***\n";
			}
		}while(nb <0 || nb>8);

		try
		{
			switch (nb)
			{

			case 1: //Ajouter un radical au dictionnaire.
			{
				cout << "Entrez le radical a ajouter: " << endl;
				cin >> radical;
				dicoTest.ajouterRadical(radical);
				cout << "Le radical a ete ajoute !" << endl;
				cout << "Affichage du dictionnaire: " << endl << dicoTest << endl;
				break;
			}

			case 2: //Ajouter une flexion à un radical.
			{
				cout << "Entrez le radical dont vous voulez ajouter une flexion: " << endl;
				cin >> radical;
				cout << "Entrez la flexion a ajouter pour ce radical: " << endl;
				cin >> flexion;
				dicoTest.ajouterFlexion(radical, flexion);
				cout << "La flexion a ete ajoutee !" << endl;
				cout << "Affichage du dictionnaire: " << endl << dicoTest << endl;
				break;
			}

			case 3: //Ajouter un synonyme à un radical.
			{
				cout << "Entrez le radical dont vous voulez ajouter un synonyme: " << endl;
				cin >> radical;
				cout << "Entrez le synonyme a ajouter pour ce radical:" << endl;
				cin >> synonyme;
				int position = -1;  //Insérer un nouveau synonyme, n'oubliez pas de tester les autres cas !
				dicoTest.ajouterSynonyme(radical, synonyme, position);
				cout << "Le synonyme a ete ajoute !" << endl;
				cout << "Affichage du dictionnaire: " << endl << dicoTest << endl;
				break;
			}

			case 4: //Supprimer une flexion d'un radical.
			{
				cout << "Entrez le radical dont vous voulez enlever une flexion: " << endl;
				cin >> radical;
				cout << "Entrez la flexion a supprimer: " << endl;
				cin >> flexion;
				dicoTest.supprimerFlexion(radical, flexion);
				cout << "La flexion a ete supprimee !" << endl;
				cout << "Affichage du dictionnaire: " << endl << dicoTest << endl;
				break;
			}

			case 5: //Supprimer un synonyne d'un radical.
			{
				cout << "Entrez le radical dont vous voulez enlever un synonyme: " << endl;
				cin >> radical;
				cout << "Quel est le synonyme que vous voulez enlever?" << endl;
				cin >> synonyme;
				int position = 0; //Enlevez le premier synonyme, n'oubliez pas de tester les autres cas !
				dicoTest.supprimerSynonyme(radical, synonyme, position);
				cout << "Le synonyme a ete supprime !" << endl;
				cout << "Affichage du dictionnaire: " << endl << dicoTest << endl;
				break;
			}

			case 6: //Supprimer un radical du dictionnaire.
			{
				cout << "Entrez le radical a supprimer: " << endl;
				cin >> radical;
				dicoTest.supprimerRadical(radical);
				cout << "Le radical a ete supprime !" << endl;
				cout << "Affichage du dictionnaire: " << endl << dicoTest << endl;
				break;
			}

			case 7: //Charger un dictionnaire a partir d'un fichier texte.
			{
				EntreeFichier.open("PetitDico.txt", ios::in);
				if(EntreeFichier.is_open())
				{
					dicoSyn.chargerDicoSynonyme(EntreeFichier);
				}
				EntreeFichier.close();
				cout << "Le dictionnaire a ete charge !" << endl;
				cout << "Affichage du dictionnaire: " << endl << dicoSyn << endl;
				break;
			}

			case 8: //Transformer une chaîne en utilisant le dictionnaire.
			{
				string chaine = "Nous poursuivrons nos etudes plus lentement";
				cout << "Phrase a transformer: " << chaine << endl;
				string resultat = "";
				istringstream iss(chaine);
				string mot;
				while (iss >> mot)
				{
					try
					{
						radical = dicoSyn.rechercherRadical(mot);
						cout << "Radical trouve: " << radical << endl
								<< "Pour changer le mot " << mot << ", tappez la lettre o: ";
						char repch;
						cin >> repch;
						if (repch != 'o')
						{
							cout << "Le mot " << mot << " ne sera pas change" << endl;
							resultat += (mot + " ");
						}
						else
						{
							int nbsens = dicoSyn.getNombreSens(radical);
							int repin = 1;
							if (nbsens > 1)
							{
								for (int j = 0; j < nbsens; j++)
								{
									cout << "Sens #" << j + 1 << " : ";
									cout << dicoSyn.getSens(radical, j) << endl;
								}
								cout << "Quel sens choisissez-vous:? ";
								cin >> repin;
							}
							std::vector<std::string> synonymes = dicoSyn.getSynonymes(radical, repin-1);
							int nbSynonymes = 0;
							for (vector<string>::const_iterator it = synonymes.begin(); it != synonymes.end(); ++it)
							{
								nbSynonymes++;
								cout << "Synonyme #" << nbSynonymes << ": " << *it << endl;
							}
							cout << "Quel synonyme choisissez-vous:? ";
							cin >> repin;
							vector<string>::const_iterator it2 = synonymes.begin();
							for (int k = 0; k != repin - 1; k++)
								it2++;
							std::vector<std::string> flexions = dicoSyn.getFlexions(*it2);
							int nbFlexions = 0;
							for (vector<string>::const_iterator it = flexions.begin(); it != flexions.end(); ++it)
							{
								nbFlexions++;
								cout << "Flexion #" << nbFlexions << ": " << *it << endl;
							}
							cout << "Quelle flexion choisissez-vous?: ";
							cin >> repin;
							it2 = flexions.begin();
							for (int l = 0; l != repin - 1; l++)
								it2++;
							resultat += (*it2 + " ");
						}
					}
					catch (std::logic_error& e)
					{
						resultat += (mot + " ");
						mot.clear();
						continue;
					}
				}
				cout << "Voici le texte transforme: " << endl;
				cout << resultat << endl;
				break;
			}

			}
		}

		catch(exception & e)
		{
			std::cerr << "ERREUR: " << e.what() << std::endl;
		}
	}

	cout << "***Merci et au revoir !***" << endl;
	return 0;
}