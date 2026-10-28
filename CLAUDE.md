# Règles du dépôt code-quotidien

Routine : chaque jour, un seul programme, le prochain jour non coché de `PLAN.md`.

1. Lire `PLAN.md`, prendre la première ligne `[ ]` (jour N, langue, problème).
2. Si la dernière ligne cochée date d'aujourd'hui (voir `git log`), ne rien faire.
3. Créer `jour-NNN/` avec :
   - le code dans la langue indiquée (C : `gcc -Wall -Wextra` sans warning ; Java : une classe `Main` ou nommée selon le problème ; Python 3 standard, sans dépendance externe) ;
   - des commentaires en français, 40 à 200 lignes de code, entrées validées ;
   - un `README.md` : titre, langue, le problème, la solution, ce qu'on apprend, comment lancer.
4. Compiler et exécuter avec plusieurs entrées (y compris une entrée invalide). Ne jamais pousser un code qui ne compile pas ou qui plante.
5. Ne rien laisser compilé dans le dépôt.
6. Cocher la ligne dans `PLAN.md` (`[ ]` → `[x]`).
7. Commit et push directement sur `main` :
   - auteur : `Ange-Miguel <jonathanecommerce9@gmail.com>` ;
   - message : `Jour NNN : <problème>` ;
   - **aucune** ligne `Co-Authored-By`, `Claude-Session` ou autre mention de Claude.
8. Si les 200 jours sont cochés : ne rien faire.
