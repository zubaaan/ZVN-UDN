#ifndef COMMON_H
#define COMMON_H

#define NOT_VALID_VALUE -1000.0

struct initialData
{
    double
    y_T     = NOT_VALID_VALUE,
    y_M     = NOT_VALID_VALUE,
    z_M     = NOT_VALID_VALUE,
    x_C1    = NOT_VALID_VALUE,
    x_C2    = NOT_VALID_VALUE,
    dt      = NOT_VALID_VALUE,
    T_ob    = NOT_VALID_VALUE,
    D_OBN   = NOT_VALID_VALUE,
    T_D     = NOT_VALID_VALUE,
    V_G     = NOT_VALID_VALUE,
    V_C     = NOT_VALID_VALUE,
    sigma_coord = NOT_VALID_VALUE,
    sigma_speed = NOT_VALID_VALUE,
    sigma_pilot = NOT_VALID_VALUE,
    phi_M   = NOT_VALID_VALUE,
    N_k     = NOT_VALID_VALUE;
};

#endif // COMMON_H
