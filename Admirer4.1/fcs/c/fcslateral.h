/***************************************/
/* Created by FFA Flight Systems Dept. */
/* Bromma, Sweden                      */
/* 29-Nov-2000 19:42:32                */
/***************************************/

double fg[3][11] = {
{ -0.2988, -0.1482, -0.1127, -0.0929, -0.0786, -0.0808, -0.1271, -0.1207, -0.1437, -0.1913, -0.2500 },
{ -0.3930, -0.1837, -0.1218, -0.0969, -0.0758, -0.0742, -0.0740, -0.1033, -0.1249, -0.1572, -0.1879 },
{ -0.1134, -0.1134, -0.1134, -0.1134, -0.0761, -0.0711, -0.0711, -0.0958, -0.1069, -0.1366, -0.1512 } };

double gari[3][11] = {
{ -0.4888, 0.1812, 0.3863, 0.6153, 0.5331, 0.5797, 0.3758, 0.7605, 0.7017, 0.4860, -0.0711 },
{ -0.8864, 0.0663, 0.3354, 0.5493, 0.5463, 0.6073, 0.5874, 0.7290, 0.8397, 0.6899, 0.2014 },
{ 0.4023, 0.4023, 0.4023, 0.4023, 0.5389, 0.6068, 0.5857, 0.7672, 0.7247, 0.8314, 0.4463 } };

double gbeta[3][11] = {
{ 7.9713, 4.6485, 3.6331, 3.0139, 1.2534, 0.7872, 0.8657, 0.3034, -0.8020, -0.7749, -1.7562 },
{ 11.3443, 6.0008, 4.9229, 4.1498, 2.1949, 1.4637, 1.4697, 1.0492, 0.2989, 0.4559, -0.0319 },
{ 6.5288, 6.5288, 6.5288, 6.5288, 3.6469, 2.4639, 2.3471, 2.1049, 1.7867, 2.1131, 1.8223 } };

double groll[3][11] = {
{ 0.5658, 0.1739, 0.0706, 0.0601, 0.0389, 0.0599, 0.0623, 0.0657, 0.0595, 0.0563, 0.1529 },
{ 0.9497, 0.1236, 0.1457, 0.1218, 0.0835, 0.1075, 0.1095, 0.1329, 0.1588, 0.1945, 0.2294 },
{ 0.0485, 0.0485, 0.0485, 0.0485, 0.1495, 0.1752, 0.1644, 0.2115, 0.2510, 0.2797, 0.2902 } };

double alt_lat[3] = { 20.0000, 3000.0000, 6000.0000 };

double dgbeta[3][11] = {
{ 5.3589, 2.8177, 1.9977, 1.5167, 0.8883, 0.7492, 0.8232, 1.3147, 2.0038, 2.4470, 2.5507 },
{ 6.4372, 3.4630, 2.6464, 1.9909, 1.1410, 0.9081, 0.9374, 1.3687, 1.9664, 2.3818, 2.6020 },
{ 2.8443, 2.8443, 2.8443, 2.8443, 1.5635, 1.1767, 1.1462, 1.4884, 2.0241, 2.4413, 2.7999 } };

int m_lat = 11;

double mach_lat[11] = { 0.2200, 0.3500, 0.4500, 0.5500, 0.8000, 0.9000, 0.9500, 1.0000, 1.0500, 1.1000, 1.2000 };

int n_lat = 3;
