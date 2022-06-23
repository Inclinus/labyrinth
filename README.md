# Labyrinth

Labyrinth est un projet de Langage C fait en 1ère année à l'ESGI.

Intervenant ayant proposé le projet : M. Kevin TRANCHO

## Prérequis

- Librairie SDL2

## Installation

Téléchargez le code source (ou clonez le repository) et compilez-le à l'aide de la commande suivante (Linux) :

```bash
gcc main.c level.c -o prog $(sdl2-config --cflags --libs) -lSDL2_image
```


## Principe du jeu

Ce jeu de labyrinthe est relativement complet, vous devez choisir une taille (entre 10 et 40).

Et c'est parti, vous êtes lancés, trouvez le chemin jusqu'à l'étoile pour remporter la partie !

Les labyrinthes sont tous générés aléatoirement, ne vous inquiétez pas !

## Améliorations

### Bombes
Vous l'auriez remarqué, vous avez la possibilité de vous amusez un peu plus dans votre recherche de l'étoile :
```bash
Souhaitez-vous des bombes sur le labyrinthe ? (o/n) :
```

En effet, vous avez la possibilité de faire apparaitre des bombes aléatoirement sur le labyrinthe, vous permettant d'exploser tous les murs autour de vous ! (N'ayez crainte, vous avez une armure suffisamment résistante pour ne pas prendre de dégâts)

###Temps de jeu
Vous remarquez également qu'à la fin de chaque partie, vous obtenez votre temps de jeu, intéressant pour essayer de battre son meilleur score ! (Enfin bon, l'aléatoire étant ce qu'il est, bon courage)

## Contribuer
Le projet est terminé, les contributions ne sont pas admises sur ce repository.

## License
[MIT](https://choosealicense.com/licenses/mit/)
