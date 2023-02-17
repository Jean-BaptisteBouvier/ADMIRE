#include "getThrust.c"
#include "getFuelBurn.c"
/* #include "admire_thrust.h" */

int main()
{

  double tss=1.0;
  double h=3000.0;
  double mach=0.8;
  int look=1;

  double T, m_dot_fuel;

  T=getThrust(tss,h,mach,look);
  m_dot_fuel=getFuelBurn(tss,h,mach,look);

  printf("%f\n",T);
  printf("%f\n",m_dot_fuel);
/*   std::cout << T << "\n"; */

  return 1;

}
