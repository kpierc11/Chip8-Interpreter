#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <fstream>
#include <bitset>
import std;

using namespace std;

// The chip 8 has 16 8 bit registers Vx where x is a hexadecmimal digit through F. There is also a 16-bit register called I. This register is generally used to store memory addresses, so only the lowest (rightmost) 12 bits are usually used.
struct Chip8Registers
{
    // General purpose 8 bit registers
    uint8_t V[16];

    // Used to store memory addresses only the lowest rightmost 12 bits are used.
    uint16_t I;

    // When these registers are non-zero they are automatically decremented at a rate of 60Hz
    uint8_t delayTimer;
    uint8_t soundTimer;

    // Stores the current executing address
    uint16_t programCounter;

    // Points to top most level of stack
    uint8_t stackPointer;

    // Used to store addresses for subroutines
    uint16_t stack[16];
};

void readROM()
{
}

int main(int argc, char *argv[])
{
    // The chip 8 is capable fo up  to 4KB (4096 bytes) of Ram from location 0x0000
    unsigned char memoryBuffer[4096] =
        {
            // Font
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F

        };

    Chip8Registers cpuRegisters;

    SDL_Window *window;
    SDL_Renderer *renderer;
    bool done = false;

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(
        "An SDL3 window",
        640,
        320,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window)
    {
        // In the case that the window could not be made...
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, nullptr);

    if (!renderer)
    {
        // In the case that the window could not be made...
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    // Read Rom file
    ifstream romFile("roms/danm8ku.ch8", ios::binary);

    char ch;

    if (!romFile)
    {
        println("Couldn't read file");
        return 1;
    }

    romFile.seekg(0, romFile.end);
    int length = romFile.tellg();
    romFile.seekg(0, romFile.beg);

    println("Length of file: {}", length);

    int programEntryPoint = 0x200;

    while (romFile.get(ch))
    {
        memoryBuffer[programEntryPoint++] = static_cast<int>(ch);
    }

    romFile.close();

    // Set the program counter initial address
    cpuRegisters.programCounter = 0x200;

    struct Pixel
    {
        SDL_FRect position;
        SDL_Color color;
    };

    vector<Pixel> pixels;

    const float pixelSize = 10.0f;

    uint8_t currentKeyValue = 0;

    while (!done)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                done = true;
            }

            if (event.type == SDL_EVENT_KEY_UP)
            {
                currentKeyValue = 0x0;
            }

            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.key == SDLK_0)
                {
                    currentKeyValue = 0x0;
                }
                if (event.key.key == SDLK_1)
                {
                    currentKeyValue = 0x1;
                }
                if (event.key.key == SDLK_2)
                {
                    currentKeyValue = 0x2;
                }
                if (event.key.key == SDLK_3)
                {
                    currentKeyValue = 0x3;
                }
                if (event.key.key == SDLK_4)
                {
                    currentKeyValue = 0x4;
                }
                if (event.key.key == SDLK_5)
                {
                    currentKeyValue = 0x5;
                }
                if (event.key.key == SDLK_6)
                {
                    currentKeyValue = 0x6;
                }
                if (event.key.key == SDLK_7)
                {
                    currentKeyValue = 0x7;
                }
                if (event.key.key == SDLK_8)
                {
                    currentKeyValue = 0x8;
                }
                if (event.key.key == SDLK_9)
                {
                    currentKeyValue = 0x9;
                }
                if (event.key.key == SDLK_A)
                {
                    currentKeyValue = 0xA;
                }
                if (event.key.key == SDLK_B)
                {
                    currentKeyValue = 0xB;
                }
                if (event.key.key == SDLK_C)
                {
                    currentKeyValue = 0xC;
                }
                if (event.key.key == SDLK_D)
                {
                    currentKeyValue = 0xD;
                }
                if (event.key.key == SDLK_E)
                {
                    currentKeyValue = 0xE;
                }
                if (event.key.key == SDLK_F)
                {
                    currentKeyValue = 0xF;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // // Fetch instructions
        uint16_t opcode = static_cast<uint16_t>((memoryBuffer[cpuRegisters.programCounter]) << 8) | memoryBuffer[cpuRegisters.programCounter + 1];

        uint8_t firstOpcodeNibble = (opcode >> 12) & 0xF;
        uint16_t NNN = opcode & 0xFFF;
        uint8_t vxRegister = (opcode >> 8) & 0x0F;
        uint8_t vyRegister = (opcode >> 4) & 0x0F;
        uint8_t NN = opcode & 0xFF;
        uint8_t N = opcode & 0x0F;

        // if (cpuRegisters.programCounter < length + 0x200)
        // {
        cpuRegisters.programCounter += 2;
        // println("Full Opcode: {:04X}", opcode);
        // println("First opcode nibble: {:X}", firstOpcodeNibble);
        // println("NN: {:02X}", NN);
        // println("Vx Register: {:X}", vxRegister);
        // println("Vy Register: {:X}", vyRegister);
        // println("NNN: {:03X}", NNN);
        // println("N: {:X}", N);
        //}

        switch (firstOpcodeNibble)
        {

        case 0x0:

            if (opcode == 0x00E0)
            {
                for (auto &pixel : pixels)
                {
                    pixel.color = {0, 0, 0, 255};
                }
            }

            if (opcode == 0x00EE)
            {
                cpuRegisters.programCounter = cpuRegisters.stack[cpuRegisters.stackPointer];
                cpuRegisters.stackPointer--;
            }
            break;

        case 0x1:
            cpuRegisters.programCounter = NNN;
            break;

        case 0x2:
            cpuRegisters.stackPointer++;
            cpuRegisters.stack[cpuRegisters.stackPointer] = cpuRegisters.programCounter;
            cpuRegisters.programCounter = NNN;
            break;

        case 0x3:

            if (cpuRegisters.V[vxRegister] == NN)
            {
                cpuRegisters.programCounter += 2;
            }
            break;

        case 0x4:
            if (cpuRegisters.V[vxRegister] != NN)
            {
                cpuRegisters.programCounter += 2;
            }
            break;

        case 0x5:
            if (cpuRegisters.V[vxRegister] == cpuRegisters.V[vyRegister])
            {
                cpuRegisters.programCounter += 2;
            }
            break;

        case 0x6:
            cpuRegisters.V[vxRegister] = NN;
            break;

        case 0x7:
            cpuRegisters.V[vxRegister] += NN;
            break;

        case 0x8:
            if (N == 0x0)
            {
                cpuRegisters.V[vxRegister] = cpuRegisters.V[vyRegister];
            }
            if (N == 0x1)
            {
                cpuRegisters.V[vxRegister] |= cpuRegisters.V[vyRegister];
            }
            if (N == 0x2)
            {
                cpuRegisters.V[vxRegister] &= cpuRegisters.V[vyRegister];
            }
            if (N == 0x3)
            {
                cpuRegisters.V[vxRegister] ^= cpuRegisters.V[vyRegister];
            }
            if (N == 0x4)
            {
                uint16_t result = cpuRegisters.V[vxRegister] + cpuRegisters.V[vyRegister];
                cpuRegisters.V[vxRegister] = static_cast<uint8_t>(result);

                if (result > 0xFF)
                {
                    cpuRegisters.V[0xF] = 1;
                }
                else
                {
                    cpuRegisters.V[0xF] = 0;
                }
            }
            if (N == 0x5)
            {
                int16_t result = cpuRegisters.V[vxRegister] - cpuRegisters.V[vyRegister];
                cpuRegisters.V[vxRegister] = static_cast<uint8_t>(result);
                if (result < 0)
                {
                    cpuRegisters.V[0xF] = 0;
                }
                else
                {
                    cpuRegisters.V[0xF] = 1;
                }
            }
            if (N == 0x6)
            {
                bitset<8> byte = cpuRegisters.V[vyRegister];
                cpuRegisters.V[vxRegister] = cpuRegisters.V[vyRegister] >> 1;
                cpuRegisters.V[0xf] = byte[0];
            }
            if (N == 0x7)
            {
                int16_t result = cpuRegisters.V[vyRegister] - cpuRegisters.V[vxRegister];
                cpuRegisters.V[vxRegister] = static_cast<uint8_t>(result);

                if (result < 0)
                {
                    cpuRegisters.V[0xF] = 0;
                }
                else
                {
                    cpuRegisters.V[0xF] = 1;
                }
            }
            if (N == 0xE)
            {
                bitset<8> byte = cpuRegisters.V[vyRegister];
                cpuRegisters.V[vxRegister] = cpuRegisters.V[vyRegister] << 1;
                cpuRegisters.V[0xF] = byte[7];
            }
            break;

        case 0x9:
            if (cpuRegisters.V[vxRegister] != cpuRegisters.V[vyRegister])
            {
                cpuRegisters.programCounter += 2;
            }
            break;

        case 0xA:
            cpuRegisters.I = NNN;
            break;

        case 0xB:
            cpuRegisters.programCounter = NNN + cpuRegisters.V[0];
            break;

        case 0xD:
        {
            // println("Full Draw Opcode {:X}", opcode);

            // cout << std::hex << cpuRegisters.I << "\n";
            // cout << std::hex << cpuRegisters.I + N << "\n";
            uint16_t xCor = cpuRegisters.V[vxRegister];
            uint16_t yCor = cpuRegisters.V[vyRegister];

            for (uint16_t i = cpuRegisters.I; i < cpuRegisters.I + N; i++)
            {
                uint8_t byte = memoryBuffer[i];

                bitset<8> bits(byte);

                for (short j = 7; j >= 0; --j)
                {
                    Pixel pixel;
                    // cout << bits[j];

                    pixel.position.x = static_cast<float>(xCor) * 10.0f;
                    pixel.position.y = static_cast<float>(yCor) * 10.0f;
                    if (bits[j])
                    {
                        pixel.position.h = pixelSize;
                        pixel.position.w = pixelSize;
                        pixel.color = {255, 255, 255, 255};
                    }
                    else
                    {
                        pixel.position.h = pixelSize;
                        pixel.position.w = pixelSize;
                        pixel.color = {0, 0, 0, 255};
                    }

                    xCor += 1.0f;
                    pixels.push_back(pixel);
                }
                xCor = cpuRegisters.V[vxRegister];
                yCor += 1.0f;
                // cout << " " << "\n";
            }

            // println("X cord {}", xCor);
            // println("Y Cord {}", yCor);
            // println("cmd: display Draw ");
        }
        break;

        case 0xE:
            if (NN == 0x9E)
            {

                if (currentKeyValue == cpuRegisters.V[vxRegister])
                {
                    cpuRegisters.programCounter += 2;
                }
            }

            if (NN == 0xA1)
            {
                if (currentKeyValue != cpuRegisters.V[vxRegister])
                {
                    cpuRegisters.programCounter += 2;
                }
            }
            break;

        case 0xF:
            if (NN == 0x07)
            {
                cpuRegisters.V[vxRegister] = cpuRegisters.delayTimer;
            }
            if (NN == 0x0A)
            {
               cpuRegisters.V[vxRegister] = currentKeyValue;
            }
            if (NN == 0x15)
            {
                cpuRegisters.delayTimer = cpuRegisters.V[vxRegister];
            }
            if (NN == 0x18)
            {
                cpuRegisters.soundTimer = cpuRegisters.V[vxRegister];
            }
            if (NN == 0x1E)
            {
                cpuRegisters.I += cpuRegisters.V[vxRegister];
            }
            if (NN == 0x29)
            {
                cpuRegisters.I = cpuRegisters.V[vxRegister];
            }
            if (NN == 0x33)
            {
                uint16_t value = cpuRegisters.V[vxRegister];
                uint8_t hundreds = value / 100;
                uint8_t tens = (value / 10) % 10;
                uint8_t ones = value % 10;

                memoryBuffer[cpuRegisters.I] = hundreds;
                memoryBuffer[cpuRegisters.I + 1] = tens;
                memoryBuffer[cpuRegisters.I + 2] = ones;
            }
            if (NN == 0x55)
            {
                uint16_t start = cpuRegisters.I;
                for (auto &reg : cpuRegisters.V)
                {
                    memoryBuffer[start] = reg;

                    if (reg == cpuRegisters.V[vxRegister])
                    {
                        break;
                    }
                    start++;
                }
            }
            if (NN == 0x65)
            {
                uint16_t start = cpuRegisters.I;
                for (auto &reg : cpuRegisters.V)
                {
                    reg = memoryBuffer[start];

                    if (reg == cpuRegisters.V[vxRegister])
                    {
                        break;
                    }
                    start++;
                }
            }
            break;
        }
        // Decode

        for (Pixel &pixel : pixels)
        {
            SDL_SetRenderDrawColor(renderer, pixel.color.r, pixel.color.g, pixel.color.b, pixel.color.a);
            SDL_RenderFillRect(renderer, &pixel.position);
        }

        //   Execute
        SDL_RenderPresent(renderer);
    }

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;
}