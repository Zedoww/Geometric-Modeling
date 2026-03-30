Définir start_edge comme la demi-arête référencée par le sommet.
Définir current_edge égal à start_edge.
Ajouter current_edge à la liste des éléments visités.
Déplacer le pointeur vers la face adjacente via curr_edge = curr_edge.twin.next.
Vérifier si current_edge est égal à start_edge.
Répéter les étapes 3 à 5 tant que la condition de comparaison est fausse.
Arrêter l'algorithme une fois que le cycle est complet (retour au point de départ).