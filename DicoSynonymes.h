/**
* \file DicoSynonymes.h
* \brief Interface du type DicoSynonymes
* \author IFT-2008
* \version 0.5
* \date avril 2021
*
* Travail pratique numéro 3
*
*/

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <queue>

#ifndef _DICOSYNONYMES__H
#define _DICOSYNONYMES__H

namespace TP3
{

   /**
   * \class DicoSynonymes
   *
   * \brief classe représentant un dictionnaire des synonymes
   *
   */
   class DicoSynonymes
   {
   public:

      /*
      *\brief     Constructeur
      *
      *\post      Une instance vide de la classe a été initialisée
      *
      */
      DicoSynonymes();

      /*
      *\brief  Constructeur de dictionnaire à partir d'un fichier
      *
      *\pre    Il y a suffisament de mémoire
      *\pre    Le fichier est ouvert au préalable
      *
      *\post   Si le fichier est ouvert, l'instance de la classe a été initialisée à partir
      *        du fichier de dictionnaire. Sinon, on génère une classe vide.
      *
      *\exception bad_alloc s'il n'y a pas assez de mémoire
      * Cette méthode appelle chargerDicoSynonyme !
      */
      DicoSynonymes(std::ifstream &fichier);


      /*
      *\brief     Destructeur.
      *
      *\post      Une instance de la classe est détruite.
      *
      */
      ~DicoSynonymes();

      /*
      *\brief     Ajouter un radical au dictionnaire des synonymes
      *\brief     tout en s’assurant de maintenir l'équilibre de l'arbre.
      *
      *\pre    Il y a suffisament de mémoire.
      *
      *\post   Le mot est ajouté au dictionnaire des synonymes.
      *
      *\exception bad_alloc si il n'y a pas suffisament de mémoire.
      *\exception logic_error si le radical existe déjà.
      */
      void ajouterRadical(const std::string& motRadical);

      /*
      *\brief  Ajouter une flexion (motFlexion) d'un radical (motRadical) à sa liste de flexions.
      *
      *\pre    Il y a suffisament de mémoire.
      *
      *\post   La flexion est ajoutée au dictionnaire des synonymes.
      *
      *\exception bad_alloc si il n'y a pas suffisament de mémoire.
      *\exception logic_error si motFlexion existe déjà ou motRadical n'existe pas.
      */
      void ajouterFlexion(const std::string& motRadical, const std::string& motFlexion);

      /*
      *\brief  Ajouter un synonyme (motSynonyme) d'un radical (motRadical)
      *\brief  à un de ses groupes de synonymes.
      *
      *\pre    Le radical se trouve déjà dans le dictionnaire
      *\       Il y a suffisament de mémoire
      *
      *\post   Le synonyme est ajouté au dictionnaire des synonymes
      *\       Si numGroupe vaut –1, le synonyme est ajouté dans un nouveau groupe de synonymes
      *\       et retourne le numéro de ce nouveau groupe dans numgroupe par référence.
      *
      *\exception bad_alloc si il n'y a pas suffisament de mémoire
      *\exception logic_error si motSynonyme est déjà dans la liste des synonymes du motRadical
      *\exception logic_error si numGroupe n'est pas correct ou motRadical n'existe pas.
      */
      void ajouterSynonyme(const std::string& motRadical, const std::string& motSynonyme, int& numGroupe);

      /*
      *\brief     Supprimer un radical du dictionnaire des synonymes
      *\brief     tout en s’assurant de maintenir l'équilibre de l'arbre.
      *
      *\pre    motRadical fait parti de l'arbre.
      *
      *\post   Si le radical appartient au dictionnaire, on l'enlève et on équilibre.
      *\       Il faut libérer la liste des synonymes
      *\       du radical en question ainsi que sa liste des flexions.
      *\       Il faut aussi s’assurer d’éliminer tout pointeur
      *\       pointant sur le nœud contenant le radical.
      *
      *\exception logic_error si l'arbre est vide ou motRadical n'existe pas.
      */
      void supprimerRadical(const std::string& motRadical);

      /*
      *\brief   Supprimer une flexion (motFlexion) d'un radical
      *\brief   (motRadical) de sa liste de flexions.
      *
      *\pre    motRadical et motFlexion font parti de l'arbre.
      *
      *\post   La flexion est enlevée du dictionnaire des synonymes.
      *
      *\exception logic_error si si l'arbre est vide ou
      *\exception motFlexion n'existe pas ou motRadical n'existe pas.
      */
      void supprimerFlexion(const std::string& motRadical, const std::string& motFlexion);

      /*
      *\brief   Retirer motSynonyme faisant partie du numéro de groupe numGroupe du motRadical.
      *
      *\pre    motRadical et motSynonyme existent et motRadical
      *\pre    a une appartenance au groupe numGroupe
      *
      *\post      Le synonyme est enlevé du dictionnaire des synonymes.
      *
      *\exception logic_error si motSynonyme ou motRadical ou numGroupe n'existent pas.
      */
      void supprimerSynonyme(const std::string& motRadical, const std::string& motSynonyme, int& numGroupe);

      /*
      *\brief     Vérifier si le dictionnaire est vide
      *
      *\post      Le dictionnaire est inchangé
      *
      */
      bool estVide() const;

      /**
      * \brief Retourne le nombre de radicaux dans le dictionnaire
      *
      * \post Le dictionnaire reste inchangé.
      *
      */
      int nombreRadicaux() const;

      /**
      * \brief Affiche à l'écran l'arbre niveau par niveau de façon à voir si l'arbre
      * est bien balancé.
      *
      * \post Le dictionnaire reste inchangé.
      *	Ne touchez pas s.v.p. à cette méthode !
      */
      friend std::ostream& operator<<(std::ostream& out, const DicoSynonymes& d)
      {
    	  if (d.racine != 0)
    	  {
    		  std::queue<NoeudDicoSynonymes*> file;
    		  std::queue<std::string> fileNiveau;

    		  NoeudDicoSynonymes * noeudDicotemp;
    		  std::string niveauTemp;

    		  int hg = 0, hd = 0;

    		  file.push(d.racine);
    		  fileNiveau.push("1");
    		  while (!file.empty())
    		  {
    			  noeudDicotemp = file.front();
    			  niveauTemp = fileNiveau.front();
    			  out << noeudDicotemp->radical;
    			  if (noeudDicotemp->gauche == 0) hg = -1; else hg = noeudDicotemp->gauche->hauteur;
    			  if (noeudDicotemp->droit == 0) hd = -1; else hd = noeudDicotemp->droit->hauteur;
    			  out << ", " << hg - hd;
    			  out << ", " << niveauTemp;
    			  out << std::endl;
    			  file.pop();
    			  fileNiveau.pop();
    			  if (noeudDicotemp->gauche != 0)
    			  {
    				  file.push(noeudDicotemp->gauche);
    				  fileNiveau.push(niveauTemp + ".1");
    			  }
    			  if (noeudDicotemp->droit != 0)
    			  {
    				  file.push(noeudDicotemp->droit);
    				  fileNiveau.push(niveauTemp + ".2");
    			  }
    		  }
    	  }
    	  return out;
      }

      /**
      * \brief Charge le dictionnaire de synonymes
      *
      * \pre p_fichier est ouvert
      *
      * \post Le dictionnaire est initialisé.
      *
      * \exception invalid_argument si p_fichier n'est pas ouvert
      *
      */
      void chargerDicoSynonyme(std::ifstream & p_fichier);

      /**
      * \brief Retourne le radical du mot entré en paramètre
      *
      * \pre mot est dans la liste des flexions d'un radical
      *
      * \post Le dictionnaire reste inchangé.
      * \post Le radical est retournée.
      *
      * \exception logic_error si l'arbre est vide
      * \exception logic_error si la flexion n'est pas dans la liste de flexions du radical
      *
      */
      std::string rechercherRadical(const std::string& mot) const;

      /**
      * \brief Retourne un réel entre 0 et 1 qui représente le degré de similitude entre mot1 et mot2 où
      *        0 représente deux mots complétement différents et 1 deux mots identiques.
      *        Vous pouvez utiliser par exemple la distance de Levenshtein, mais ce n'est pas obligatoire !
      *
      * \post Un réel entre 0 et 1 est retourné
      * \post Le dictionnaire reste inchangé.
      *
      */
      float similitude(const std::string& mot1, const std::string& mot2) const;


      /**
      * \brief Donne le nombre de cellules de appSynonymes.
      *
      *
      * \post Le nombre de cellules de appSynonymes pour le radical entré en paramètre est retourné
      * \post Le dictionnaire reste inchangé.
      *
      */
      int getNombreSens(std::string radical) const;


      /**
      * \brief Donne le premier synonyme du groupe de synonyme de l'emplacement entrée en paramètre.
      *
      *
      * \post un string correspondant au premier synonyme est imprimé
      * \post Le dictionnaire reste inchangé.
      *
      */
      std::string getSens(std::string radical, int position) const;


      /**
      * \brief Donne tous les synonymes du mot entré en paramètre du groupeSynonyme du parametre position
      *
      *
      * \post un vecteur est retourné avec tous les synonymes
      * \post Le dictionnaire reste inchangé.
      *
      */
      std::vector<std::string> getSynonymes(std::string radical, int position) const;

      /**
      * \brief Donne toutes les flexions du mot entré en paramètre
      *
      *
      * \post un vecteur est retourné avec toutes les flexions
      * \post Le dictionnaire reste inchangé.
      *
      */
      std::vector<std::string> getFlexions(std::string radical) const;



   private:

      /**
      * \class NoeudDicoSynonymes
      *
      * \brief classe représentant un noeud dans un dictionnaire (arbre AVL)
      *
      */
      class NoeudDicoSynonymes
      {
      public:
         std::string radical;             		// Un radical qui est un élément fondamental d'un mot, auquel s'ajoutent suffixes et préfixes.
         std::list<std::string> flexions;   	// Une liste chaînée de chaîne de caractères représentant les flexions du radical.
         std::vector<int> appSynonymes;   		// Un tableau contenant les indices (entiers) des groupes de synonymes auquel appartient le radical.
         int hauteur;                    		// La hauteur de ce noeud (afin de maintenir l'équilibre de l'arbre AVL).
         NoeudDicoSynonymes *gauche, *droit;	// Les pointeurs sur les enfants du noeud.
         NoeudDicoSynonymes(const std::string& m) : radical(m), hauteur(0), gauche(0), droit(0) { }
      };



      NoeudDicoSynonymes *racine;                           // pointeur vers la racine de l'arbre des radicaux.
      int nbRadicaux;                                       // Nombre de noeuds dans l'arbre
      std::vector< std::list<NoeudDicoSynonymes*> > groupesSynonymes;   // Tableau de groupes de synonymes dont chaque élément est une liste
      	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	// de pointeurs sur des noeuds de l'arbre représentant les radicaux.

       void ajouterNouveauNoeud(NoeudDicoSynonymes*& noeudCible,const std::string& rad) const;
        NoeudDicoSynonymes* retournerNoeudRadical(const std::string& radical) const;
       void destructeurDict(NoeudDicoSynonymes*& noeud);
   };
}//Fin du namespace

#endif



