#include "utils.h"
regex atomReg("[A-Z][0-9]*");
regex ctdReg("/\\\\");
regex cnctReg("!|\\^|v|(->)|(<->)");
regex paraReg("\\(|\\)");
regex allReg("([A-Z][0-9]*)|(/\\\\)|!|\\^|v|(->)|(<->)|\\(|\\)");