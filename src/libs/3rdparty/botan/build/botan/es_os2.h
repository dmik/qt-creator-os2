/**
* OS/2 EntropySource
* (C) 2011 Ruediger Ihle
*
* Distributed under the terms of the Botan license
*/

#ifndef BOTAN_ENTROPY_SRC_BEOS_H__
#define BOTAN_ENTROPY_SRC_BEOS_H__

#include <botan/entropy_src.h>

namespace Botan {

/**
* OS/2 Entropy Source
*/
class BOTAN_DLL OS2_EntropySource : public EntropySource
   {
   private:
      std::string name() const { return "OS/2 Statistics"; }

      void poll(Entropy_Accumulator& accum);
   };

}

#endif
