#ifndef ARM11_28_DECODE_H
#define ARM11_28_DECODE_H

//interface for all instructions

#include "branch.h"
#include "singleDT.h"
#include "multiply.h"
#include "DataProcDec.h"
#include "halt.h"

//This part receives the instructions fetched and has to
// distinguish between the different instructions save the
// value of the different parts they have (Rn, Rs, <offset etc) for
// each of the instructions, and save it as the decoded intruction


void decode(MACHINE *machine);

#endif //ARM11_28_DECODE_H