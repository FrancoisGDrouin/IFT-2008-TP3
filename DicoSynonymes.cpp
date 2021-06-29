/**
* \file DicoSynonymes.cpp
* \brief Le code des opérateurs du DicoSynonymes.
* \author IFT-2008, François Gauthier-Drouin
* \version 0.5
* \date avril 2021
*
* Travail pratique numéro 3.
*
*/

#include "DicoSynonymes.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <algorithm>

namespace TP3
{

	// Méthode fournie
	void DicoSynonymes::chargerDicoSynonyme(std::ifstream& fichier)
	{
        if (!fichier.is_open())
            throw std::logic_error("DicoSynonymes::chargerDicoSynonyme: Le fichier n'est pas ouvert !");

		std::string ligne;
		std::string buffer;
		std::string radical;
		int cat = 1;

		while (!fichier.eof()) // tant qu'on peut lire dans le fichier
		{
            std::getline(fichier, ligne);

			if (ligne == "$")
			{
				cat = 3;
				std::getline(fichier, ligne);
			}
			if (cat == 1)
			{
				radical = ligne;
				ajouterRadical(radical);
				cat = 2;
			}
			else if (cat == 2)
			{
				std::stringstream ss(ligne);
				while (ss >> buffer)
					ajouterFlexion(radical, buffer);
				cat = 1;
			}
			else
			{
				std::stringstream ss(ligne);
				ss >> radical;
				ss >> buffer;
				int position = -1;
				ajouterSynonyme(radical, buffer, position);
				while (ss >> buffer)
					ajouterSynonyme(radical, buffer, position);
			}
		}
	}

    /**
     * \brief Constructeur du dictionnaire de synonymes
     *
     * \post On cree une instance du dictionnaire
     */

    DicoSynonymes::DicoSynonymes() :racine(0),nbRadicaux(0),groupesSynonymes()
    {
    }

    /**
     * \brief Constructeur de dictionnaire à partir d'un fichier
     *
     * \post Si le fichier est ouvert, l'instance de la classe a été initialisée à partir
      *        du fichier de dictionnaire. Sinon, on génère une classe vide.
     * \param[in] fichier un stream contenant les informations a mettre dans le dictionnaire
     */
    DicoSynonymes::DicoSynonymes(std::ifstream &fichier)
            :racine(0),nbRadicaux(0),groupesSynonymes()
    {
        chargerDicoSynonyme(fichier);
    }

    /**
     * \brief Destructeur du dictionnaire de synonymes
     *
     * \post Une instance de la classe est détruite.
     */
    DicoSynonymes::~DicoSynonymes()
    {
        destructeurDict(racine);
    }

    /**
 * \brief Ajouter un radical au dictionnaire des synonymes
  *\brief tout en s’assurant de maintenir l'équilibre de l'arbre.
 *
 * \param[in] motRadical radical qui doit etre ajoute
 */
    void DicoSynonymes::ajouterRadical(const std::string& motRadical)
    {
        ajouterNouveauNoeud(racine,motRadical);
    }

    /**
     * \brief Ajouter un radical au dictionnaire des synonymes
      *\brief tout en s’assurant de maintenir l'équilibre de l'arbre.
     *
     * \param[in] noeudCible noeud qu'on parcourt pour ajouter le radical
     * \param[in] rad radical a ajouter
     */
    void DicoSynonymes::ajouterNouveauNoeud(NoeudDicoSynonymes*& noeudCible, const std::string & rad) const
    {
        if(noeudCible!=nullptr)
        {
            noeudCible->hauteur++;
        }
        if(noeudCible==0)
        {
            noeudCible = new NoeudDicoSynonymes(rad);
            return;
        }
        ajouterNouveauNoeud(noeudCible->gauche,rad);
    }

    /**
     * \brief Donne toutes les flexions du mot entré en paramètre
     *
     * \param[in] radical du mot pour lequel on veut les flexions
     */
    std::vector<std::string> DicoSynonymes::getFlexions(std::string radical) const
    {
        std::vector<std::string> flexions;
        NoeudDicoSynonymes* noeudRadSaisi = retournerNoeudRadical(radical);
        for(auto flexionCourante : noeudRadSaisi->flexions)
        {
            flexions.push_back(flexionCourante);
        }
        return flexions;
    }

    /**
     * \brief Donne tous les synonymes du mot entré en paramètre du groupeSynonyme du parametre position
     *
     * \param[in] radical du mot pour lequel on veut les synonymes
     */
    std::vector<std::string> DicoSynonymes::getSynonymes(std::string radical, int position) const
    {
        std::vector<std::string> synonymes;
        NoeudDicoSynonymes* noeudRadSaisi = retournerNoeudRadical(radical);
        for(auto synonymeCourant
        : groupesSynonymes[noeudRadSaisi->appSynonymes[position]])
        {
            synonymes.push_back(synonymeCourant->radical);
        }
        return synonymes;
    }

    /**
     * \brief Retourne un réel entre 0 et 1 qui représente le degré de similitude entre mot1 et mot2 où
      *        0 représente deux mots complètement différents et 1 deux mots identiques.
     *
     * \param[in] mot saisi
     */
    std::string DicoSynonymes::rechercherRadical(const std::string& mot) const
    {
        float resultat=0;
        std::string motChoisi;
        if(estVide())
        {
            throw std::logic_error("L'arbre est vide.");
        }
        NoeudDicoSynonymes* noeudActuel = racine;
        do
        {
            if(similitude(noeudActuel->radical,mot)>=resultat)
            {
                resultat = similitude(noeudActuel->radical,mot);
                motChoisi = noeudActuel->radical;
            }
            noeudActuel = noeudActuel->gauche;
        } while (noeudActuel != nullptr);

        if(resultat>=0.9)
        {
            return motChoisi;
        }
        else
        {
            throw std::logic_error("Ce mot n'apparaît pas dans la liste des flexions du radical.");
        }
    }

    /**
 * \brief Retourne un réel entre 0 et 1 qui représente le degré de similitude entre mot1 et mot2 où
      *        0 représente deux mots complétement différents et 1 deux mots identiques.
 * \param[in] mot1 premier mot
 * \param[in] mot2 deuxieme mot
 */

    float DicoSynonymes::similitude(const std::string& mot1, const std::string& mot2) const
    {
        int i,j;
        const std::size_t l1 = mot1.size();
        const std::size_t l2 = mot2.size();

        std::vector<std::vector<unsigned int>> d(l1+1, std::vector<unsigned int>(l2+1));
        d[0][0] = 0;

        for(i=0;i<=l1;i++)
        d[i][0] = i;
        for(j=0;j<=l2;j++)
        d[0][i] = i;

        for (i=1;j<=l1;j++)
        {
            for(i=1;i<=l2;i++)
            {
                for(j=1;j<=l2;j++)
                {
                    d[i][j] = std::min({d[i - 1][j] + 1, d[i][j-1]+1,d[i][j]+(mot1[i-1] == mot2[j-1] ? 0 : 1)});
                }
            }
        }
        return (float)(1 - (d[l1][l2]/(std::max(l1,l2))));
    }

    /**
     * \brief Ajouter une flexion (motFlexion) d'un radical (motRadical) à sa liste de flexions.
     *
     * \param[in] motRadical radical a ajouter une flexion
     * \param[in] motFlexion flexion a ajouter
     */

    void DicoSynonymes::ajouterFlexion(const std::string& motRadical, const std::string& motFlexion)
    {
        NoeudDicoSynonymes* noeudActuel = retournerNoeudRadical(motRadical);
        if(noeudActuel != nullptr)
        {
            for(auto flexionsExistantes: noeudActuel->flexions)
            {
                if(motFlexion==flexionsExistantes)
                {
                    throw std::logic_error("motFlexion existe déjà ou motRadical n'existe pas");
                }
            }
            noeudActuel->flexions.push_back(motFlexion);
        }
    }

    /**
     * \brief Supprimer un radical du dictionnaire des synonymes
      *\brief tout en s’assurant de maintenir l'équilibre de l'arbre.
     *
     * \param[in] motRadical le radical a enlever
     */

    void DicoSynonymes::supprimerRadical(const std::string& motRadical)
    {
        NoeudDicoSynonymes* noeudActuel=racine;
        std::string radicalActuel=rechercherRadical(motRadical);

        if(racine->radical==radicalActuel)
        {
            NoeudDicoSynonymes* noeudAncien=racine;
            delete noeudAncien;
            racine=racine->gauche;
            return;
        }

        do
        {
            if(noeudActuel->gauche->radical==radicalActuel)
            {
                NoeudDicoSynonymes* noeudAncien=noeudActuel->gauche;
                noeudActuel->gauche=noeudActuel->gauche->gauche;
                delete noeudAncien;
                return;
            }
            noeudActuel=noeudActuel->gauche;
        } while(noeudActuel!=nullptr);
    }

    /**
     * \brief Supprimer une flexion (motFlexion) d'un radical
      *\brief   (motRadical) de sa liste de flexions.
     *
     * \param[in] motRadical le radical duquel on retire une flexion
     * \param[in] motFlexion la flexion a enlever
     */

    void DicoSynonymes::supprimerFlexion(const std::string& motRadical, const std::string& motFlexion)
    {
        NoeudDicoSynonymes* noeudActuel=retournerNoeudRadical(motRadical);
        if(noeudActuel != nullptr)
        {
            noeudActuel->flexions.remove(motFlexion);
        }
    }

    /**
     * \brief Retirer motSynonyme faisant partie du numéro de groupe numGroupe du motRadical.
     *
     * \param[in] motRadical le radical concerne
     * \param[in] motSynonyme le synonyme a retirer
     * \param[in] numGroupe le numero du groupe auquel on enleve le synonyme
     */

    void DicoSynonymes::supprimerSynonyme(const std::string& motRadical, const std::string& motSynonyme, int& numGroupe)
    {
        NoeudDicoSynonymes* noeudActuel=retournerNoeudRadical(motRadical);
        std::list<NoeudDicoSynonymes*> synonymes = groupesSynonymes[noeudActuel->appSynonymes[numGroupe]];
        std::list<NoeudDicoSynonymes*>::iterator synonymeEnleve;
        for(auto element: synonymes)
        {
            if(element->radical==motSynonyme)
            {
                synonymes.remove(element);
                break;
            }
        }
        return;
    }

    /**
     * \brief Vérifier si le dictionnaire est vide
     *
     */
    bool DicoSynonymes::estVide() const
    {
        return racine==0;
    }


    /**
     * \brief Retourne le nombre de radicaux dans le dictionnaire
     *
     */
    int DicoSynonymes::nombreRadicaux() const
    {
        return racine->hauteur;
    }

    /**
         * \brief Donne le nombre de cellules de appSynonymes.
         *
         * \param[in] radical le radical pour lequel on veut connaître le nombre de cellules
         */

    int DicoSynonymes::getNombreSens(std::string radical) const
    {
        NoeudDicoSynonymes* noeudActuel=retournerNoeudRadical(radical);
        return noeudActuel->appSynonymes.size();
    }

    /**
     * \brief Donne un du mot radical
     *
     * \param[in] radical le radical duquel on veut savoir le sens
     * \param[in] position le numero du sens
     */
    std::string DicoSynonymes::getSens(std::string radical, int position) const
    {
        NoeudDicoSynonymes* noeudActuel=retournerNoeudRadical(radical);
        return groupesSynonymes[noeudActuel->appSynonymes[position]].front()->radical;
    }

    /**
     * \brief Fonction qui retourne le noeud d'un radical trouve
     *
     * \param[in] radical le radical qu'on veut obtenir
     */

    DicoSynonymes::NoeudDicoSynonymes* DicoSynonymes::retournerNoeudRadical(const std::string & radical) const
    {
        NoeudDicoSynonymes* noeudActuel=racine;
        std::string radicalActuel=rechercherRadical(radical);

        do
        {
            if(noeudActuel->radical==radicalActuel)
            {
                return noeudActuel;
            }
            noeudActuel=noeudActuel->gauche;
        } while(noeudActuel!=nullptr);
    }

    //Ajouter un synonyme (motSynonyme) d'un radical (motRadical) à un de ses groupes de synonymes.
    //Si numGroupe vaut –1, le synonyme est ajouté dans un nouveau groupe de synonymes
    //et retourne le numéro de ce nouveau groupe dans numgroupe par référence.
    //Erreur si motSynonyme est déjà dans la liste des synonymes du motRadical.
    //Erreur si numGroupe n'est pas correct ou motRadical n'existe pas.

    /**
     * \brief Ajouter un synonyme (motSynonyme) d'un radical (motRadical)
      *\brief  à un de ses groupes de synonymes.
     *
     * \param[in] motRadical le radical qu'on veut ajouter un synonyme
     * \param[in] motSynonyme le synonyme a ajouter
     * \param[in] numGroupe le numero du groupe auquel ajouter le synonyme
     */

    void DicoSynonymes::ajouterSynonyme(const std::string& motRadical, const std::string& motSynonyme, int& numGroupe)

    {
        NoeudDicoSynonymes* noeudVise;
        NoeudDicoSynonymes* noeudActuel=retournerNoeudRadical(motRadical);

        if(numGroupe>-1)
        {
            if (numGroupe>noeudActuel->appSynonymes.size()-1)
            {
                throw std::logic_error("numGroupe n'est pas correct ou motRadical n'existe pas");
            }
            for (auto synonyme : groupesSynonymes[noeudActuel->appSynonymes[numGroupe]])
            {
                if (motSynonyme==synonyme->radical)
                {
                    throw std::logic_error("motSynonyme est déjà dans la liste des synonymes du motRadical");
                }
            }
        }
        try
        {
            NoeudDicoSynonymes* noeudVise=retournerNoeudRadical(motSynonyme);
        }
        catch(const std::exception)
        {
            ajouterRadical(motSynonyme);
        }

        if(numGroupe<-1)
        {
            groupesSynonymes[noeudActuel->appSynonymes[numGroupe]].push_back(noeudVise);
        }

        if(numGroupe=-1)
        {
            std::list<NoeudDicoSynonymes*>* synonymesActuels = new std::list<NoeudDicoSynonymes*>();
            synonymesActuels->push_back(retournerNoeudRadical(motSynonyme));
            groupesSynonymes.push_back(*synonymesActuels);
            noeudActuel->appSynonymes.push_back(groupesSynonymes.size()-1);
        }
    }

    /**
 * \brief fonction recursive de destructeur du dictionnaire de synonymes
 *
 * \post Suppression du noeud saisi
 * \param[in] noeud le noeud a detruire
 */
    void DicoSynonymes::destructeurDict(NoeudDicoSynonymes *& noeud)
    {
        if (noeud!=0)
        {
            destructeurDict(noeud->gauche);
            destructeurDict(noeud->droit);
            delete noeud;
        }
    }

}//Fin du namespace