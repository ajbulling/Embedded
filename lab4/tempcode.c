#include <stdio.h>
#include <stdint.h>

int main() {
    uint8_t input1 = 4;
    uint8_t input2 = 3;
    uint8_t pmode = 1;
    if (input1 == 1) pmode = 0; // One-shot mode, don't ask for input 2
    else {
        /* Ignore one for one-shot, start at zero so subtract 2
         * Then add 4 to move over to the upper 4 bits
         * -2 + 4 = 2, so add 2 to input 1
        */
        input1 += 2;
        pmode = pmode << input1; // bit-shift over to select processing mode
        pmode |= input2; // bit-wise OR to select number of samples
    }
    printf("%x\n", pmode);
}
