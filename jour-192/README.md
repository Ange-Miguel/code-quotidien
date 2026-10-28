# Jour 192 — Mini-interpréteur de commandes (shell)

**Langue :** Java

## Le problème
Comment fonctionne un terminal comme bash ou l'invite de commandes de Windows ? Il lit une ligne, la découpe en commande et arguments (en respectant les guillemets), exécute, garde un historique et gère les erreurs. Comprendre cela aide à mieux utiliser le terminal, et à écrire la console de commande d'un appareil embarqué.

## La solution
Un shell sur un système de fichiers virtuel en mémoire (des Map imbriquées) : pwd, ls, cd (avec .. et chemins absolus), mkdir, touch, echo (avec redirection > vers un fichier), cat, rm, history et help. Chaque commande est une entrée d'une table (nom → fonction). Un analyseur découpe la ligne en tenant compte des guillemets. La démo exécute un script ; --interactif lit au clavier.

## Ce qu'on apprend
L'analyse d'une ligne de commande, les arbres de répertoires, la résolution de chemins relatifs et absolus, et la table de commandes.

## Lancer
```bash
javac Main.java
java Main
```

## Exemple de sortie
```
/ $ pwd
/
/ $ mkdir projets cours
/ $ cd projets
/projets $ mkdir covoiturage
/projets $ echo "Covoiturage étudiant" > covoiturage/README
/projets $ ls covoiturage
README
/projets $ cat covoiturage/README
Covoiturage étudiant
/projets $ cd ../cours
/cours $ touch electronique.txt
/cours $ ls
electronique.txt
/cours $ cd /
```
