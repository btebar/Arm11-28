cmake_minimum_required(VERSION 3.10)
project(arm11_28 C)

set(CMAKE_C_STANDARD 11)

include_directories(extension)
include_directories(src)
include_directories(src/assemble_utils)
include_directories(src/emulate_utils)

add_executable(arm11_28
        extension/tetris/main.c
        extension/tetris/tetris.c
        extension/tetris/tetris.h
        extension/cell.c
        extension/cell.h
        extension/game.c
        extension/game.h
        extension/grid.c
        extension/grid.h
        extension/initial_configs.c
        extension/initial_configs.h
        src/assemble_utils/assemblerImplementation.c
        src/assemble_utils/assemblerImplementation.h
        src/assemble_utils/branch.c
        src/assemble_utils/branch.h
        src/assemble_utils/DataProc.c
        src/assemble_utils/DataProc.h
        src/assemble_utils/defs.c
        src/assemble_utils/defs.h
        src/assemble_utils/instruction_defs.h
        src/assemble_utils/multiply.c
        src/assemble_utils/multiply.h
        src/assemble_utils/registers.h
        src/assemble_utils/sdt.c
        src/assemble_utils/sdt.h
        src/emulate_utils/binaryloader.c
        src/emulate_utils/binaryloader.h
        src/emulate_utils/branch.c
        src/emulate_utils/branch.h
        src/emulate_utils/DataProcDec.c
        src/emulate_utils/DataProcDec.h
        src/emulate_utils/decode.c
        src/emulate_utils/decode.h
        src/emulate_utils/execute.c
        src/emulate_utils/execute.h
        src/emulate_utils/halt.c
        src/emulate_utils/halt.h
        src/emulate_utils/instruction_basic.h
        src/emulate_utils/multiply.c
        src/emulate_utils/multiply.h
        src/emulate_utils/singleDT.c
        src/emulate_utils/singleDT.h
        src/emulate_utils/usefulFuncs.c
        src/emulate_utils/usefulFuncs.h
        src/assemble.c
        src/emulate.c
        src/memoryImplementation.h
        src/usefulTools.h extension/tetris/block.c extension/tetris/block.h extension/tetris/util.c extension/tetris/util.h
        extension/sudoku.c
        extension/sudoku.h)
