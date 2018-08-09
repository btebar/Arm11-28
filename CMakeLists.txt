cmake_minimum_required(VERSION 3.10)
project(arm11_28 C)

set(CMAKE_C_STANDARD 11)

add_executable(arm11_28
        src/emulate_utils/branch.c
        src/emulate_utils/DataProcDec.c
        src/emulate_utils/multiply.c
        src/emulate_utils/singleDT.c
        src/assemble.c
        src/emulate_utils/binaryloader.c
        src/emulate_utils/decode.c
        src/emulate.c
        src/emulate_utils/execute.c
        src/emulate_utils/execute.h
        src/emulate_utils/instruction_basic.h
        src/memoryImplementation.h
        src/usefulTools.h
        src/emulate_utils/usefulFuncs.c
        src/assemble_utils/assemblerImplementation.h
        src/assemble_utils/assemblerImplementation.c
        src/emulate_utils/usefulFuncs.h
        src/emulate_utils/branch.h
        src/emulate_utils/DataProcDec.h
        src/emulate_utils/halt.h
        src/emulate_utils/multiply.h
        src/emulate_utils/singleDT.h
        src/emulate_utils/binaryloader.h
        src/emulate_utils/decode.h
        src/assemble_utils/defs.h
        src/assemble_utils/defs.c
        src/assemble_utils/multiply.c
        src/assemble_utils/instruction_defs.h
        src/assemble_utils/branch.c
        src/assemble_utils/DataProc.c
        src/assemble_utils/multiply.h
        src/assemble_utils/branch.h
        src/assemble_utils/DataProc.h
        src/emulate_utils/instruction_basic.h
        src/usefulTools.h
        extension/cell.c
        extension/cell.h
        extension/game.c
        extension/game.h
        extension/grid.c
        extension/grid.h)
