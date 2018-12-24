#ifndef TINY_ALIGNED_16_H
#define TINY_ALIGNED_16_H

#ifndef ALIGNED_16
#ifdef __GNUC__
#define ALIGNED_16 __attribute__((aligned (16)))
#endif
#ifdef _MSC_VER
#define ALIGNED_16
//__declspec(align(16))    // 2010-08-06 Kenny: Problem: MSVC STL implements some interfaces using arguments by copy rather than arguments by reference. The VC compiler does  not know how to but a 16 byte aligned data type on the call stack. A true fix for this would be for Microsoft to fix their STL distribution. 
#endif
#endif
    
// TINY_ALIGNED_16_H
#endif 
