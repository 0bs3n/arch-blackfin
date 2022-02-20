#include "Decoder.h"
#include <stdio.h>

int
decodeImmValue(const_forms_t cf, int x, unsigned int pc, bool abs_addr)
{
    // Legacy cruft, not sure if I can safely remove yet
    (void)pc;
    (void)abs_addr;

    /*
    // why is this disabled? -- ethan
    if (0 && constant_formats[cf].reloc)
    {
        unsigned int ea = (((constant_formats[cf].pcrel ? SIGNEXTEND (x, constant_formats[cf].nbits) : x) 
            + constant_formats[cf].offset) << constant_formats[cf].scale);

        // I don't want this to happen automatically, commenting out for now.
        // The information has been moved to a new member, outf->pc_at.
        // should something like an outf->pcrel be added? -- ethan
        if (constant_formats[cf].pcrel && abs_addr) ea += pc;

        return ea;
    }
    */
    /* Negative constants have an implied sign bit.  */
    if (constant_formats[cf].negative)
    {
        int nb = constant_formats[cf].nbits + 1;
        x = x | (1 << constant_formats[cf].nbits);
        x = SIGNEXTEND (x, nb);
    } else if (constant_formats[cf].issigned) {
        x = SIGNEXTEND (x, constant_formats[cf].nbits);
    }

    x += constant_formats[cf].offset;
    x <<= constant_formats[cf].scale;

    return x;
}
