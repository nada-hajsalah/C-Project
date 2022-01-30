#Indications pour faire tourner le programme

1ére Etape : Run fichier  makefile en executant commande: make

2éme Etape: - Si vous voulez utiliser les identifiants des stations, la commande à exécuter est la suivante:
`./my_project "./data/s.csv" "data/c.csv" By_ID Non_Station_Debut Nom_Station_Fin` 
            - Si vous voulez utiliser les noms des stations, la commande à exécuter est la suivante
`./my_project "./data/s.csv" "data/c.csv" BY_Name Non_Station_Debut Nom_Station_Fin` 

Exemple d'exécution en utilisant les noms des stations: 
`./my_project "./data/s.csv" "data/c.csv" BY_Name Varenne simplon`



Résultat obtenu est le suivant:

```
**** Meuilleur chemin  **** 

 Meilleur matching de  Varenne est Varenne
 Meilleur matching de  simplon est Simplon

La meilleure façon Varenne (ligne 13) to Simplon (ligne 4) : 

Prends la ligne 13 (CHATILLON - MONTROUGE <-> ST-DENIS-UNIVERSITE/LES COURTILLES) - Aller
A partir de Varenne d'aller à Place de Clichyen 758 s
Va à Place de Clichy, ligne 2 avec  temps du trajet :75 s
Prends la ligne 2 (NATION <-> PORTE DAUPHINE) - Aller
A partir de Place de Clichy d'aller à Barbès-Rochechouarten 463 s
Va à Barbès-Rochechouart, ligne 4 avec  temps du trajet :91 s
Prends la ligne 4 (PORTE DE CLIGNANCOURT <-> MAIRIE DE MONTROUGE) - Retour
A partir de Barbès-Rochechouart to Simplon avec temps du trajet:281 s
******Temps pour arriver au destination ****** 


 Temps: 1668 secondes

```

