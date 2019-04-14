
#include "./zdec.h"
#include <stdlib.h>
#include <stdio.h>

namespace cmps3120
{
  static const char zdec_dyn_codes[] = {
    16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
    };
  zdec::zdec(void)
  : zss()
  {
  }
  zdec::~zdec(void)
  {
  }
  void zdec::reset(void)
  {
    reset_sub();
    bit_pos = 0;
    last_block = false;
  }
  zss_error zdec::put_eof(void)
  {
    if (readmode == 9)
    {
      return ZSS_DONE;
    } else {
      return ZSS_MISSING_CHECKSUM;
    }
  }
  int zdec::compute_fetch(int rval)
  {
    if ((rval < 257)
    ||  (rval > 285))
      return ZSS_DATA_FETCH;
    if (rval < 265)
    {
      num_fetch = rval-254;
      ti = 0;
    } else if (rval < 285)
    {
      rval -= 261;
      ti = (rval>>2);
      num_fetch = 3+((4+(rval&3))<<ti);
    } else /*if (rval == 285)*/
    {
      num_fetch = 258;
      ti = 0;
    }
    return 0;
  }
  int zdec::compute_back_long(int rval)
  {
    if ((rval < 0)
    ||  (rval > 29))
      return ZSS_DATA_BACK;
    if (rval < 4)
    {
      back_long = rval+1;
      ti = 0;
    } else if (rval < 30)
    {
      rval -= 2;
      ti = (rval>>1);
      back_long = 1+((2+(rval&1))<<ti);
    }
    return 0;
  }
  
  zss_error zdec::put_char(unsigned char x)
  {
    unsigned int out = 0;
    /*
     * states:
     * 0  - header
     * 1  - dict
     * 2  - other decompression methods
     * 3  - checksum
     * 4  - DEFLATE : block id
     * 5  - direct - block length
     * 6  - direct - one's complement block length
     * 7  - direct - data
     * 8  - fixed - data
     * 9  - done
     */
    switch (readmode)
    {
    case 0:
      {
        if (submode < 2)
        {
          xlong <<= 8;
          xlong |= x&255;
          submode++;
        }
        if (submode >= 2)
        {
          zss_header nhdr(xlong);
          if (!nhdr.is_valid())
          {
            return ZSS_BAD_HEADER;
          }
          set_header(nhdr);
          if (nhdr.fdict)
            readmode = 1;
          else if (nhdr.xmethod == 8)
          {
            readmode = 4;
            if (!setup_window() )
            {
              out = ZSS_MEMORY;
            }
          } else
            readmode = 2;
          submode = 0;
        }
      }break;
    case 1:
      {
        if (submode < 4)
        {
          xpect_dict_adler <<= 8;
          xpect_dict_adler |= x&255;
          submode++;
        }
        if (submode >= 4)
        {
          if ((!is_dict_set())
          ||  (get_dict_checksum() != xpect_dict_adler))
          {
            return ZSS_DICTIONARY;
          } else if (get_header().xmethod == 8)
          {
            readmode = 4;
            if (!setup_window() )
            {
              out = ZSS_MEMORY;
            }
          } else
            readmode = 2;
          submode = 0;
        }
      }break;
    case 3:
      {
        /*fprintf(stderr,"*%08lxv%08lx*\n",
          xlong,(unsigned long)get_checksum());*/
        if (si < 4)
        {
          xlong = (xlong<<8)|(x&255);
          si++;
        }
        
        if (si >= 4)
        {
          if (xlong ^ (unsigned long)get_checksum())
          {
            out = ZSS_CHECKSUM;
          } else {
            readmode = 9;
            out = ZSS_DONE;
          }
        }
      }break;
    case 4:
    case 8:
    case 10:
      {
        int i;
        for (i = 0; (i < 8)&&(!out); i++)
        {
          bit_pos = i;
          out = process_bit((x&(1<<i))?1:0);
        }
      }break;
    case 5:
      {
        /* byte direct, length */
        if (si < 2)
        {
          xlong |= ((x&255)<<(si*8));
          si++;
        }
        if (si >= 2)
        {
          back_long = xlong;
          si = 0;
          readmode = 6;
          xlong = 0;
        }
      }break;
    case 6:
      {
        /* byte direct, negative length */
        if (si < 2)
        {
          xlong |= ((x&255)<<(si*8));
          si++;
        }
        if (si >= 2)
        {
          if ((back_long&=65535) ^ ((~xlong)&65535))
            out = ZSS_BLOCK_ERROR;
          else {
            si = 0;
            readmode = 7;
            xlong = 0;
            if (!back_long)
            {
              readmode = 4;
              submode = 0;
            }
          }
        }
      }break;
    case 7:
      {
        /* byte direct, data */
        if (back_long)
          back_long--;
        append(x&255);
        if (!back_long)
        {
          if (last_block)
          {
            readmode = 3;
            submode = 4;
            si = 0;
            xlong = 0;
          } else {
            readmode = 4;
            submode = 0;
          }
        }
      }break;
    case 9:
      {
        /* do nothing */
      }break;
    default:
      return 2;
    }
    return out;
  }
  zss_error zdec::process_bit(int b)
  {
    int out = 0;
    switch (submode)
    {
    case 0:
      {
        last_block = b?true:false;
        xlong = 0; si = 0;
        submode = 1;
      }break;
    case 1:
      {
        if (si < 2)
        {
          xlong |= (b?1:0)<<(si++);
        }
        if (si >= 2)
        {
          switch (xlong)
          {
          case 0:
            set_codes(zss_huffs::for_direct());
            set_distances(zss_huffs());
            submode = 2;
            readmode = 5;
            si = 0;
            break;
          case 1:
            set_codes(zss_huffs::for_fixed());
            set_distances(zss_huffs::for_distance());
            readmode = 8;
            submode = 3;
            si = 0;
            xlong = 0;
            break;
          case 2:
            set_codes(zss_huffs());
            set_distances(zss_huffs());
            readmode = 10;
            submode = 10;
            si = 0;
            xlong = 0;
            ti = 2;
            break;
          case 3:
            out = ZSS_BLOCK_ERROR;
            break;
          }
        }
      }break;
    case 2:
      {
        /* vsync to direct */
        if (bit_pos)
          break;
        readmode = 5;
        si = 0;
      }
    case 4:
      {
        /* vsync to checksum */
        if (bit_pos)
          break;
        readmode = 3;
        si = 0;
      }
    case 3:
    case 13:
      {
        /* fixed - data */
        /* dynamic - data */
        int rval;
        if (si <= 32)
        {
          xlong = (xlong<<1)|(b?1:0);
          si++;
          /*fprintf(stderr,"(%lo %i)",xlong,si);*/
          rval = get_codes().get_value(xlong,si);
          if (rval >= 0)
          {
            /*fprintf(stderr,"<%i %lx:%i>",rval,get_char_count(), bit_pos);*/
            if (rval == 256)
            {
              /*fprintf(stderr,"*%lo-(%08lx:%i)>%08lx*\n",
                xlong,get_char_count(),bit_pos,
                (unsigned long)get_checksum());*/
              if (last_block)
              {
                readmode = 3;
                submode = 4;
                si = 0;
                xlong = 0;
              } else {
                readmode = 4;
                submode = 0;
              }
            } else if (rval > 256){
              /* do fancy backtrack stuff */
              out = compute_fetch(rval);
              si = 0;
              xlong = 0;
              
              if (ti > 0)
                submode = 5;
              else
                submode = 6;
            } else {
              append(rval&255);
            }
            xlong = 0;
            si = 0;
          }
        }
        
        if (si > 32)
        {
          out = ZSS_DATA_ERROR;
        }
      }break;
    case 5:
    case 15:
      {
        /* fixed - data size (extra) */
        if (si < ti)
        {
          xlong |= (b?1:0)<<(si++);
        }
        if (si >= ti)
        {
          num_fetch += xlong;
          submode++;
          si = 0;
          xlong = 0;
        }
      }break;
    case 6:
    case 16:
      {
        /* fixed - distance */
        int rval;
        if (si <= 32)
        {
          xlong = (xlong<<1)|(b?1:0);
          si++;
          /*fprintf(stderr,"(~%lo %i)",xlong,si);*/
          rval = get_distances().get_value(xlong,si);
          if (rval >= 0)
          {
            /*fprintf(stderr,"<~%i>",rval);*/
            if ((rval >= 0) && (rval <= 29))
            {
              out = compute_back_long(rval);

              if (ti > 0)
                submode ++;
              else
                submode += 2;
            } else
              out = ZSS_DATA_ERROR;
            xlong = 0;
            si = 0;
          }
        }
        
        if (si > 32)
        {
          out = ZSS_DATA_ERROR;
        }
      }if ((submode != 8) && (submode != 18)) break;
    case 7:
    case 17:
      if ((submode == 7)
      ||  (submode == 17))
      {
        /* fixed - distance (extra) */
        if (si < ti)
        {
          xlong |= (b?1:0)<<(si++);
        }
        if (si >= ti)
        {
          back_long += xlong;
          submode++;
          si = 0;
          xlong = 0;
        } else break;
      }
    case 8:
    case 18:
      {
        do
        {
          unsigned char cc = get_previous(back_long);
          if (!append(cc&255))
          {
            out = ZSS_OVERFLOW;
            break;
          }
        } while (--num_fetch);
        if (!num_fetch)
        {
          submode -= 5;
        }
      }break;
    case 10:
      {
        /* dynamic - header */
        if (si < (ti?5:4))
        {
          xlong |= (b?1:0)<<(si++);
        }
        if (si >= (ti?5:4))
        {
          /*fprintf(stderr,"*%lo %lu*",xlong,xlong);*/
          switch (ti)
          {
          case 2:
            hlit = xlong+257;
            break;
          case 1:
            hdist = xlong+1;
            break;
          case 0:
            hclen = xlong+4;
            break;
          }
          ti--;
          xlong = 0;
          si = 0;
        }
        if (ti<0)
        {
          ti=0;si=0;xlong=0;submode=11;
          set_extended_codes(zss_huffs());
          /*fprintf(stderr,"@(%i,%i,%i)",hclen,hlit,hdist);*/
        }
      }break;
    case 11:
      {
        /* dynamic - extended codes */
        if (si < 3)
        {
          xlong |= (b?1:0)<<(si++);
        }
        if (si >= 3)
        {
          get_extended_codes().append_code(xlong,zdec_dyn_codes[ti]);
          ti++;
          si = 0;
          xlong = 0;
        }
        if (ti >= hclen)
        {
          get_extended_codes().sort_by_value();
          bool is_made = get_extended_codes().sort_and_make();
          /*fprintf(stderr,"%%%s%%",
            (is_made?"true":"false"));*/
          if (!is_made) out = ZSS_EX_DATA_ERROR;
          submode = 12;
          ti = 0;
          num_fetch = 0;
          set_codes(zss_huffs());
        }
      }break;
    case 12:
      {
        /* dynamic - length/distance extension */
        int rval;
        if (si <= 32)
        {
          xlong = (xlong<<1)|(b?1:0);
          si++;
          /*fprintf(stderr,"(`%lo %i)",xlong,si);*/
          rval = get_extended_codes().get_value(xlong,si);
          if (rval >= 0)
          {
            /*fprintf(stderr,"<`%i>",rval);*/
            if (rval == 16)
            {
              /* do fancy backtrack stuff */
              submode = 14;
              si = 0;
              xlong = 0;
              back_long = 2;
              /* num_fetch = num_fetch; */
            } else if (rval == 17)
            {
              /* do fancy backtrack stuff */
              submode = 14;
              si = 0;
              xlong = 0;
              num_fetch = 0;
              back_long = 3;
            } else if (rval == 18)
            {
              /* do fancy backtrack stuff */
              submode = 14;
              si = 0;
              xlong = 0;
              num_fetch = 0;
              back_long = 7;
            } else {
              if (ti < hlit)
                get_codes().append_code(rval,ti);
              else if ((ti-hlit) < hdist)
                get_distances().append_code(rval,ti-hlit);
              num_fetch = rval;
              ti++;
            }
            xlong = 0;
            si = 0;
            if (ti >= (hlit+hdist))
            {
              /*fprintf(stderr,"--");*/
              get_codes().sort_and_make();
              /*fprintf(stderr,"--");*/
              get_distances().sort_and_make();
              /*get_distances().print_err();*/
              /*fprintf(stderr,"--");*/
              ti = 0;
              submode = 13;
              xlong = 0;
              si = 0;
              num_fetch = 0;
            }
          }
        }
        
        if (si > 32)
        {
          out = ZSS_EX_DATA_ERROR;
        }
      }break;
    case 14:
      {
        if (si < (int)back_long)
        {
          xlong |= (b?1:0)<<(si++);
        }
        if (si >= (int)back_long)
        {
          switch (back_long)
          {
          case 2:
          case 3:
            xlong += 3;
            break;
          case 7:
            xlong += 11;
            break;
          }
          
          for (si = 0; (si < (int)xlong) && (ti < (hlit+hdist));
            si++, ti++)
          {
            if (ti < hlit)
              get_codes().append_code(num_fetch,ti);
            else if ((ti-hlit) < hdist)
              get_distances().append_code(num_fetch,ti-hlit);
          }

          xlong = 0;
          si = 0;
          if (ti >= (hlit+hdist))
          {
            /*fprintf(stderr,"--");*/
            get_codes().sort_and_make();
            /*fprintf(stderr,"--");*/
            /*get_distances().print_err();*/
            get_distances().sort_and_make();
            /*fprintf(stderr,"--\n");
            get_distances().print_err();*/
            ti = 0;
            submode = 13;
            xlong = 0;
            si = 0;
            num_fetch = 0;
          } else {
            submode = 12;
            xlong = 0;
            si = 0;
            /* num_fetch = num_fetch */
          }
        }
      }break;
    default:
      {
        out = 2;
      }break;
    }
    return out;
  }
  
};



