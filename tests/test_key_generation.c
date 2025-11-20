#include "../include/ecco/ecco.h"
#include <stdio.h>

int main() {
    struct keyring keyring;
    struct curve curve;
    
    if (curve_populate(&curve, "P-256")) {
        
    } else {
        perror("bad curve");
    }
    init_keyring(&keyring);
    set_global_modulo(curve.modulus_p);

    set_keyring_private(&keyring);
    generate_keyring(&curve, &keyring);

    set_keyring_public(&keyring);
    generate_keyring(&curve, &keyring);

    compute_shsecret(&keyring);
    set_keyring_private(&keyring);
    compute_shsecret(&keyring);

    print_keyring(&keyring);
}