//
//  main.cpp
//  Emu
//
//  Created by Sergio Mancera on 30/06/15.
//  Copyright (c) 2015 Sergio Mancera. All rights reserved.
//

#include "emu.hpp"



int main(int argc, const char * argv[]) {
    
    std::string rom = argv[2];

    emu c_emu(rom);
    c_emu.init(true);
    for (;;) {
        if(!c_emu.emulateCycle())
            break;
    }
    return 0;
}
