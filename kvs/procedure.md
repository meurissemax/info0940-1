# Procédure pour ajouter un nouveau system call dans le linux kernel

Exemple : ajouter le system call "hello"

1. Créer un dossier `hello/` (dans le dossier linux).
2. Dans le dossier `hello`, créer un fichier `hello.c` dont la structure est la suivante :

```c
#include <linux/kernel.h>

asmlinkage long hello(void) {
	printk("Hello world\n");

	return 0;
}
```

3. Créer un `Makefile` dans le dossier `hello/` et y ajouter la ligne suivante :

```bash
obj-y := hello.o
```

Cela permet d'être sûr que le fichier `hello.c` sera ajouté et compilé dans les sources de linux.

4. Retourner dans le dossier parent (dossier de linux) et éditer le `Makefile`. Chercher la ligne suivante :

```bash
core-y += kernel/ mm/ fs/ ipc/ security/ crypto/ block/
```

et y ajouter le dossier `hello/`

```bash
core-y += kernel/ mm/ fs/ ipc/ security/ crypto/ block/ hello/
```

5. Éditer la table des system calls. Pour cela, faire :

```bash
cd arch/x86/entry/syscalls/
```

et éditer le fichier `syscall_32.tbl` (pour un système 32 bits). Ajouter dans ce fichier le system call (au même format que les autres).

6. Retourner dans le dossier parent (dossier de linux) et faire :

```bash
cd include/linux/
```

et éditer le fichier `syscalls.h` (y ajouter le prototype de la fonction `hello()`).

7. Compiler le kernel et re démarrer la machine.
