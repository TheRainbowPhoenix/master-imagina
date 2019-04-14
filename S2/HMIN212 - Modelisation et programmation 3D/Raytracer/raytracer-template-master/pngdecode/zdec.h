
#if !(defined CMPS3120_LABS_ZDEC_H)
#define CMPS3120_LABS_ZDEC_H

#include "./zss.h"

namespace cmps3120
{
  class zdec : public zss
  {
  public:
    zdec(void);
    ~zdec(void);
    
    void reset(void);
    
  private:
    int bit_pos, si, ti;
    bool last_block;
    unsigned long int back_long;
    int num_fetch;
    int hclen, hlit, hdist;
    
    zss_error put_char(unsigned char x);
    zss_error put_eof(void);
    zss_error process_bit(int b);
    int compute_fetch(int rval);
    int compute_back_long(int rval);
  };
};

#endif /*CMPS3120_LABS_ZDEC_H*/

