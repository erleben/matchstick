#ifndef PROCEDURAL_NOISE_H
#define PROCEDURAL_NOISE_H

namespace procedural
{

  class Noise
  {
  public:

    static bool & on()
    {
      static bool value = false;

      return value;
    }

    static double & scale()
    {
      static double value = 0.001;

      return value;
    }

  };
			
} // end of namespace procedural

// PROCEDURAL_NOISE_H
#endif
