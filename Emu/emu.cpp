

#include "emu.hpp"

emu::emu(){
    
}

emu::emu(std::string file){

    cart.open(file,std::ios::in|std::ios::binary);
    if(cart.is_open()){
        
        // get length of file:
        cart.seekg (0, cart.end);
        std::streampos length = cart.tellg();
        cart.seekg (0, cart.beg);
        
        rom = new unsigned char [length];
        
        std::cout << "Reading " << length << " characters... ";
        // read data as a block:
        cart.read (reinterpret_cast<char*>(rom),length);
        
        if (cart)
            std::cout << "all characters read successfully."<<std::endl;
        else
            std::cout << "error: only " << cart.gcount() << " could be read";
        cart.close();
        

        int title_lenght = 0x142 - 0x134;
        char subbuff[title_lenght];
        memcpy( subbuff, &rom[0x134], title_lenght );
        subbuff[title_lenght] = '\0';
        
        title = subbuff;
        unsigned int gbc = rom[0x143];

        if(gbc==0x80)
            isgbcolor = true;
        else
            isgbcolor = false;
        
        cart_type = rom[0x147];

        
    }
    else{

        std::cout <<"nonas"<<std::endl;
    }
    
    std::cout<<title<<std::endl;
    
    if (isgbcolor)
        std::cout<<"Game Boy Color title"<<std::endl;
    else
        std::cout<<"Not Game Boy Color title"<<std::endl;
    
    switch (cart_type) {
        case 0x00:
            std::cout<<"ROM ONLY"<<std::endl;
            break;
        case 0x1B:
            std::cout<<"MBC5+RAM+BATTERY"<<std::endl;
            break;
        default:
            break;
    }

}


void emu::init(){
    PC = 0x100;
    SP = 0xFFFE;
    opcode = 0x0;
    
    for (int i = 0 ; i < 8192; i ++) {
        ram[i] = 0x0;
        vram[i] = 0x0;
    }
    
    R[A] = 0x01;
    R[F] = 0xb0;
    
    R[B] = 0x00;
    R[C] = 0x13;
    
    R[D] = 0x00;
    R[E] = 0xd8;

    R[H] = 0x01;
    R[L] = 0x4d;
    
}

void emu::WriteMemory(unsigned short address, unsigned char value){
    int a = address;
    
    if (a >= 0xe000 && a <= 0xfdff)
        a-= 0xe000;
    else
        a-= 0xc000;
    
    ram[a] = value;
    std::cout<<"ADDRESS:"<<std::hex<<int(a)<<std::endl;
}

unsigned char emu::xor8bit(unsigned char a,unsigned char b,unsigned char* f){
    unsigned char result = a ^ b;
    if (result == 0x0)
        *f &= 0x8F;
    else
        *f &= 0x0F;
    return result;
}


unsigned char emu::substract8bit(unsigned char a,unsigned char b,unsigned char* f){
    
    unsigned int Z = 0;
    unsigned char result = 0x0;
    unsigned char flag = *f;
    
    a = 0xaa;
    b = 0xaa;
    
    //std::cout<<"FLAG:"<<std::hex<<int(flag)<<std::endl;
    
    for (int i=0; i<8; i++) {
        int X = (a & (1<<i)) != 0;
        int Y = (b & (1<<i)) != 0;
    
        unsigned int d = X ^ Y ^ Z;
        Z = (~X & (Y | Z)) | (Y & Z);
        
        if (i == 3){
            if (Z == 1)
                flag |= 0x20; //set H flag to 1
            else
                flag &= 0xDF; //set H flag to 0
        }
        if (i == 7){
            if (Z == 1)
                flag |= 0x10; //set C flag to 1
            else
                flag &= 0xEF; //set C flag to 0
        }
    
        result = (result) | (d << i);
    }
    
    flag |= 0x40; //set N flag to 1
    if (int(result)==0)
        flag |= 0x80; //set Z flag to 1
    else
        flag &= 0x7F; //set Z flag to 0
    *f = flag;
    
    return result;
}

bool emu::emulateCycle(bool debug){
    
    std::cout<<"PC:"<<int(PC)<<std::endl;
    opcode = rom[PC];
    

    switch (opcode) {

        case 0x00: //NOP
            if (debug)
                std::cout<<"NOP"<<std::endl;
            PC++;
            break;

        case 0x05: //DEC B
        {
            if (debug)
                std::cout<<"DEC B"<<std::endl;
            R[B] = substract8bit(R[B], 0x1, &R[F]);
            PC++;
            break;
        }
        case 0x06: //LD B, d8
            if (debug)
                std::cout<<"LD B, "<<std::hex<<int(rom[PC + 1])<<std::endl;
            R[B] = rom[PC + 1];
            PC += 2;
            break;

        case 0x0D: //DEC C
        {
            if (debug)
                std::cout<<"DEC C"<<std::endl;
            R[C] = substract8bit(R[C], 0x1, &R[F]);
            PC++;
            break;
        }
            
        case 0x0E: //LD C, d8
            if (debug)
                std::cout<<"LD C, "<<std::hex<<int(rom[PC + 1])<<std::endl;
            R[C] = rom[PC + 1];
            PC += 2;
            break;

        case 0x15: //DEC D
        {
            if (debug)
                std::cout<<"DEC D"<<std::endl;
            R[D] = substract8bit(R[D], 0x1, &R[F]);
            PC++;
            break;
        }
            
        case 0x1D: //DEC E
        {
            if (debug)
                std::cout<<"DEC E"<<std::endl;
            R[E] = substract8bit(R[E], 0x1, &R[F]);
            PC++;
            break;
        }
            
        case 0x1E: //LD E, d8
            if (debug)
                std::cout<<"LD E, "<<std::hex<<int(rom[PC + 1])<<std::endl;
            R[E] = rom[PC + 1];
            PC += 2;
            break;

        //case 0x1F: //RRA
        case 0xCB: //RRA
            if (debug)
                std::cout<<"RRA"<<std::endl;
            std::cout<<"Rot "<<std::hex<<int(R[A])<<std::endl;
            R[A] = R[A]>>1;
            std::cout<<"Rot "<<std::hex<<int(R[A])<<std::endl;
            PC ++;
            break;
            
        case 0x20: //JR cc,e
            if (debug)
                std::cout<<"JR NZ"<<std::hex<<int(rom[PC + 1])<<std::endl;
            if ((R[F] & 0x80) == 0x80)
                PC += rom[PC + 1];
            else
                PC += 2;
            break;
            
        case 0x21: //LD HL, d16
            if (debug)
                std::cout<<"LD HL, "<<std::hex<<int(rom[PC + 2] << 8 |  rom[PC + 1])<<std::endl;
            R[H] = rom[PC + 2];
            R[L] = rom[PC + 1];
            PC += 3;
            break;

        case 0x25: //DEC H
        {
            if (debug)
                std::cout<<"DEC H"<<std::endl;
            R[H] = substract8bit(R[H], 0x1, &R[F]);
            PC++;
            break;
        }
            
        case 0x2D: //DEC L
        {
            if (debug)
                std::cout<<"DEC L"<<std::endl;
            R[L] = substract8bit(R[L], 0x1, &R[F]);
            PC++;
            break;
        }
            
        case 0x32: //LD (HL-),A
        {
            if (debug)
                std::cout<<"LD (HL-),A"<<std::endl;
            unsigned short HL = R[H] << 8 | R[L];
            std::cout<<"HL"<<std::hex<<int(HL)<<std::endl;
            WriteMemory(R[H] << 8 | R[L], R[A]);

            HL -= 0x1;
            R[H] = (HL & 0xFF00)>>8;
            R[L] = HL & 0x00FF;
            PC++;
            break;
        }
            
        case 0x3D: //DEC A
        {
            if (debug)
                std::cout<<"DEC A"<<std::endl;
            R[A] = substract8bit(R[A], 0x1, &R[F]);
            PC++;
            break;
        }
            
        case 0xAF: //XOR A
            if (debug)
                std::cout<<"XOR A"<<std::endl;

            R[A] ^= xor8bit(R[A], R[A], &R[F]) ;
            
            PC++;
            break;
            
        case 0xC3: //JP a16
            if (debug)
                std::cout<<"JP "<<std::hex<<int(rom[PC + 2] << 8 |  rom[PC + 1])<<std::endl;
            PC = rom[PC + 2] << 8 |  rom[PC + 1];
            break;
            
        case 0xEE: //XOR d8
            if (debug)
                std::cout<<"XOR "<<std::hex<<int(rom[PC + 1])<<std::endl;
            R[A] ^= rom[PC+1];
            if (R[A] == 0x0)
                R[F] &= 0x8F;
            else
                R[F] &= 0x0F;
            PC += 2;
            break;
            
        case 0xFF: // RST 38h
        {
            std::cout<<"OPCODE:"<<std::hex<<int(opcode)<<std::endl;
            PC = 0xABCD;
            //std::bitset<8> h((PC & 0xFF00)>>8);
            //std::bitset<8> l((PC & 0x00FF));
            unsigned char h = (PC & 0xFF00)>>8;
            unsigned char l = PC & 0x00FF;

            ram[SP-1] = h;
            ram[SP-2] = l;
            
            break;
        
        }
        default:
            std::cout<<"Unknown OPCODE:"<<std::hex<<int(opcode)<<std::endl;
            return false;
            break;
    }
    //std::cout<<"OPCODE:"<<std::hex<<int(opcode)<<std::endl;
    //std::cout<<"PC:"<<std::hex<<int(PC)<<std::endl;
    return true;
    
}