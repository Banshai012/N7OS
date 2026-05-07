.text
# cette directive sert a rendre l'etiquette publique
    .globl keyboard_IT_handler
# debut du traitant
keyboard_IT_handler:
# sauvegarde des registres importants
    pushl %eax
    pushl %edx
    pushl %ecx
# appel a la fonction C realisant le traitant
    call handler_keyboard
# restauration des registres importants
    popl %ecx
    popl %edx
    popl %eax
# fin du traitant
    iret
