#ifndef HVL_H
#define HVL_H

#include "hvl_types.h"

// There were several issues in my previous hypervisor, both in maintainability
// and ease of use when coming back to work on it after even a short absence. The 
// code layout was pretty messy and hard enough to follow so in this project, the aim
// will be to make the process of creating and launching a VM easy and understandable,
// even from the perspective of someone who doesn't exactly know what is going on or
// hasn't seen or studied the source code before.
// Another large issue was with the types that I used, I enjoyed using stronly typed version
// of certain types because I believed it improved code readability and made things easier
// to understand (which it did, to an extent) but in times where I would document certain
// hardware level bit-fields (e.g Control register structure, RFLAGS etc), I should not have
// used these types as parametes in the public interface which was used to develop, this bulked
// up code and added unnecessary lines for which I had to initialise a variable of this type, even
// when I would have the data stored in some integral of the same size, possibly taking up
// extra stack space. In this project I will be aiming to only use these types internally, which
// would make the code much more readable when I want to work with bitfields when enabling/disabling
// features, or updating system state through control registers, MSRs or anything alike
// Stronly typed aliases can be used as long as they are trivially constructable from basic literals

#endif
