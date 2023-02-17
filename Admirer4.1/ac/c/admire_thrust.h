#include <math.h>

/* #include "admire_data.h" */
#include "aerodata_fuel01.h"
#include "setup_aerodata_fuel01.h"
#include "aerodata_fuel02.h"
#include "setup_aerodata_fuel02.h"
#include "aerodata_thrust01.h"
#include "setup_aerodata_thrust01.h"
#include "aerodata_thrust02.h"
#include "setup_aerodata_thrust02.h"

/* #include "getThrust.c" */
/* #include "getFuelBurn.c" */

double getThrust(double tss_in, double h_geo, double mach_xb, int look);
double getFuelBurn(double tss_in, double h_geo, double mach_xb, int look);
