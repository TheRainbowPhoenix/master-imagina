# Description

Notre projet ce compose de deux programmes permettant de créer et d'utiliser un
chat de communication distante dans via un terminal.
Le premier programme "server" permet de mettre en place les structures qui 
géreront les communications avec les clients distants.
Le deuxieme programme "client" permet de se connecter à un serveur distant et
de communiquer avec les autres clients egalement connectés. 

# Compilation

Deplacez vous dans le repertoire du projet puis utilisez une des commandes
suivantes.

```bash
make
#   ou
g++
```

# Execution

Deplacez vous dans le repertoire du projet, compilez puis mettez en place le
serveur (Usage : server <PORT> <MAX_CLIENT>).

```bash
./bin/server 25 30
```
Ensuite connectez vous à ce serveur grâce à sont adresse et son numéro de port
(Usage : client <SERV_ADDR> <SERV_PORT>).

```bash
./bin/client localhost 25
```
Vous pouvez maintenant communiquer avec les autres clients connectés.