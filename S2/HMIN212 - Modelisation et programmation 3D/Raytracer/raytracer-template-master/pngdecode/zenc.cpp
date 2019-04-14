
#include "./zenc.h"
#include <stdlib.h>
#include <limits.h>
#include <string.h>

namespace cmps3120
{
  zenc_hash_line::zenc_hash_line(unsigned int *p, unsigned int s)
    : m_dta(p), m_length(s)
  {
  }
  zenc_hash_line::~zenc_hash_line(void)
  {
    m_dta = NULL;
    m_length = 0;
  }

  zenc_hash_line::operator unsigned int&(void)
  {
    return *m_dta;
  }
  zenc_hash_line::operator const unsigned int&(void) const
  {
    return *m_dta;
  }

  unsigned int& zenc_hash_line::operator[](unsigned int j)
  {
    return m_dta[j];
  }
  const unsigned int& zenc_hash_line::operator[](unsigned int j) const
  {
    return m_dta[j];
  }
  unsigned int zenc_hash_line::length(void) const
  {
    return m_length;
  }

  zenc_hash_const_line::zenc_hash_const_line
    (const unsigned int *p, unsigned int s)
    : m_dta(p), m_length(s)
  {
  }
  zenc_hash_const_line::~zenc_hash_const_line(void)
  {
    m_dta = NULL;
    m_length = 0;
  }

  zenc_hash_const_line::operator const unsigned int&(void) const
  {
    return *m_dta;
  }

  const unsigned int& zenc_hash_const_line::operator[](unsigned int j) const
  {
    return m_dta[j];
  }
  unsigned int zenc_hash_const_line::length(void) const
  {
    return m_length;
  }

  zenc_hash::zenc_hash(void)
    : m_size(0), m_length(0), m_table(NULL)
  {
  }
  zenc_hash::~zenc_hash(void)
  {
    if (m_table != NULL)
    {
      free(m_table);
      m_table = NULL;
    }
    m_size = 0;
    m_length = 0;
  }

  unsigned int zenc_hash::get_size(void) const
  {
    return m_size;
  }
  unsigned int zenc_hash::get_entry_length(void) const
  {
    return m_length;
  }
  bool zenc_hash::resize(unsigned int sz, unsigned int lh)
  {
    unsigned int xl;
    if (!lh)
      lh = 1;
    if (sz < ((UINT_MAX/sizeof(unsigned int))/lh))
    {
      xl = sz*lh;
      if (xl == m_size*m_length)
      {
        m_size = sz;
        m_length = lh;
      } else if (xl)
      {
        unsigned int *ptr = (unsigned int*)calloc
          (xl,sizeof(unsigned int));
        if (ptr != NULL)
        {
          if (m_table != NULL)
          {
            free(m_table);
            m_table = NULL;
          }
          m_table = ptr;
          m_size = sz;
          m_length = lh;
        } else {
          return false;
        }
      } else {
        if (m_table != NULL)
        {
          free(m_table);
          m_table = NULL;
        }
        m_table = NULL;
        m_size = sz;
        m_length = lh;
      }
    }
    return true;
  }
  void zenc_hash::put_entry(unsigned int hash_base, unsigned int value)
  {
    if ((m_table != NULL)
    &&  (m_size > 0)
    &&  (m_length > 0))
    {
      if (m_length > 1)
      {
        memmove(&at(hash_base,1),&at(hash_base,0),
            sizeof(unsigned int)*(m_length-1));
      }
      at(hash_base) = value;
    }
  }
  unsigned int& zenc_hash::at(unsigned int i, unsigned int j)
  {
    if (m_size)
      i %= m_size;
    if ((i >= m_size)
    ||  (j >= m_length))
      return *(unsigned int*)NULL;
    return m_table[i*m_length+j];
  }
  const unsigned int& zenc_hash::at(unsigned int i, unsigned int j) const
  {
    if (m_size)
      i %= m_size;
    if ((i >= m_size)
    ||  (j >= m_length))
      return *(unsigned int*)NULL;
    return m_table[i*m_length+j];
  }
  unsigned int& zenc_hash::operator()(unsigned int i, unsigned int j)
  {
    return at(i,j);
  }
  const unsigned int& zenc_hash::operator()
    (unsigned int i, unsigned int j) const
  {
    return at(i,j);
  }
  zenc_hash_line zenc_hash::operator[](unsigned int i)
  {
    return zenc_hash_line(&at(i), m_length);
  }
  zenc_hash_const_line zenc_hash::operator[]
    (unsigned int i) const
  {
    return zenc_hash_const_line(&at(i), m_length);
  }

  zenc_pair::zenc_pair(unsigned char x)
    : nvalue(x&255), dvalue(0), n_ext(0), d_ext(0)
  {}
  zenc_pair::zenc_pair(unsigned int len, unsigned int dist)
    : nvalue(257), dvalue(0), n_ext(0), d_ext(0)
  {
    if ((len < 3) || (len > 258) || (!dist) || (dist > 32768))
    {
      nvalue = 286;
      dvalue = 32;
    } else
    {
      if (len >= 3 && len <= 10)
        nvalue = 257+(len-3);
      else if (len >= 11 && len <= 18)
      {
        len -= 11;
        nvalue = 265+(len>>1);
        n_ext = len&1;
      } else if (len >= 19 && len <= 34)
      {
        len -= 19;
        nvalue = 269+(len>>2);
        n_ext = len&3;
      } else if (len >= 35 && len <= 66)
      {
        len -= 35;
        nvalue = 273+(len>>3);
        n_ext = len&7;
      } else if (len >= 67 && len <= 130)
      {
        len -= 67;
        nvalue = 277+(len>>4);
        n_ext = len&15;
      } else if (len >= 131 && len <= 257)
      {
        len -= 131;
        nvalue = 281+(len>>5);
        n_ext = len&31;
      } else if (len == 258)
      {
        nvalue = 285;
      }

      /* distances */
      d_ext = dist-1;
      for (len = 0; d_ext && len < 32; len++, d_ext>>=1) 
        ;
      if (len < 3)
      {
        dvalue = dist-1;
        d_ext = 0;
      } else {
        dvalue = (len<<1)+(((dist-1)>>(len-2))&1)-2;
        d_ext = (dist-1)&((1<<(len-2))-1);
      }
    }
  }
  zenc_pair::zenc_pair(enum zenc_pair_stop_t)
    : nvalue(256), dvalue(0), n_ext(0), d_ext(0)
  {}
  unsigned int zenc_pair::literal(void) const { return nvalue; }
  unsigned int zenc_pair::length(void) const { return nvalue; }
  unsigned int zenc_pair::length_ext(void) const { return n_ext; }
  unsigned int zenc_pair::distance(void) const { return dvalue; }
  unsigned int zenc_pair::distance_ext(void) const { return d_ext; }

  zenc::zenc(void)
    : m_d_block_size(1024)
  {
    reset();
  }
  zenc::~zenc(void)
  {
  }
  void zenc::reset(void)
  {
    reset_sub();
    m_last_block = false;
    m_has_blocks = false;
    m_shift_out = 0;
    m_bytes_pend.resize(0);
    m_hash.resize(0,0);
    m_hash.resize(256,4);
    m_hashpos = 0;
    m_bithold = 0;
    m_bitcount = 0;
  }
  zss_error zenc::put_char(unsigned char x)
  {
    return put_char_or_eof(x&255);
  }
  zss_error zenc::put_eof(void)
  {
    return put_char_or_eof(-1);
  }

  zss_error zenc::put_char_or_eof(int x)
  {
    int out;
    bool need_redo;
    do
    {
      out = ZSS_UNDEFINED;
      need_redo = false;
      switch (readmode)
      {
      case 0:
        {
          out = 0;
          if (xlong == 0)
          {
            bool ok = setup_window();
            if (!ok)
            {
              out = ZSS_MEMORY;
            } else {
              m_xhdr = get_header();
              m_xhdr.set_check();
              m_shift_out = m_xhdr;
            }
          }
          if (xlong < 2)
          {
            if (!append_no_history
                ((m_shift_out>>((1-xlong)<<3))&255) )
              out = ZSS_MEMORY;
            xlong++;
          }
          if (xlong >= 2)
          {
            if (m_xhdr.fdict)
              readmode = 1;
            else
              readmode = 2;
            xlong = 0;
          }
          need_redo = true;
        }break;
      case 1: /* dictionary */
        {
          out = 0;
          if (xlong < 4)
          {
            if (!append_no_history
                ((get_dict_checksum()>>((3-xlong)<<3))&255) )
              out = ZSS_MEMORY;
            xlong++;
          }
          if (xlong >= 4)
          {
            readmode = 2;
            xlong = 0;
          }
          need_redo = true;
        }break;
      case 3: /* checksum */
        {
          out = 0;
          if (xlong < 4)
          {
            if (!append_no_history
                ((get_checksum()>>((3-xlong)<<3))&255) )
              out = ZSS_MEMORY;
            xlong++;
          }
          if (xlong >= 4)
          {
            readmode = 4;
            out = ZSS_DONE;
          } else{
            need_redo = true;
          }
        }break;
      case 4: /* end of stream */
        {
          out = ZSS_DONE;
        }break;

      case 2:
        {
          out = gen_bits(x);
          if (out == ZSS_DONE)
          {
            readmode = 3;
            out = 0;
            push_resync();
            need_redo = true;
          }
        }break;

      default:
        /*out = ZSS_UNDEFINED;*/
        break;
      }
    } while ((!out) && (need_redo));
    return (zss_error) out;
  }
  int zenc::gen_bits(int x)
  {
    int out = 0;
    if (x >= 0)
    {
      x &= 255;
      if (m_bytes_pend.append(x) )
      {
        if (m_bytes_pend.size() > m_d_block_size)
        {
          out = output_block();
        }
      } else {
        out = ZSS_MEMORY;
      }
    } else {
      /* end of data */
      m_last_block = true;
      out = output_block();
    }
    return out;
  }
  int zenc::output_block(void)
  {
    int outmode, putmode = get_header().flevel;
    switch (putmode)
    {
    case 0: /* direct output */
      {
        unsigned int i;
        outmode = push_bit(m_last_block?1:0);
        if (!outmode) outmode = push_bit(0);
        if (!outmode) outmode = push_bit(0);
        push_resync();
        unsigned long int xlen, xnlen, xpos = 0;
        xlen = m_bytes_pend.size();
        xnlen = ~xlen;
        do
        {
          if (xlen > 65535)
            xlen = 65535;
          xnlen = ~xlen;
          append_no_history((xlen    )&255);
          append_no_history((xlen >>8)&255);
          append_no_history((xnlen   )&255);
          append_no_history((xnlen>>8)&255);
          for (i = 0; (i < m_bytes_pend.size())&&(!outmode); i++)
          {
            if (!append_no_history(m_bytes_pend[i]))
              outmode = ZSS_MEMORY;
            next_hash(m_bytes_pend[i]);
          }
          if (xpos >= (m_bytes_pend.size()-xlen))
          {
            xlen = 0;
            xpos = m_bytes_pend.size();
          } else {
            xpos += xlen;
            xlen = m_bytes_pend.size()-xpos;
          }
        } while(xlen);
        m_has_blocks = true;
        if (m_last_block) outmode = ZSS_DONE;
        m_bytes_pend.resize(0);
      }break;
    case 1: /* fixed Huffman codes */
    case 2: /* TODO dynamic Huffman codes, 7-9 */
    case 3: /* TODO dynamic Huffman codes, 4-10, with removal */
      {
        unsigned int i, l;
        outmode = push_bit(m_last_block?1:0);
        if (!outmode) outmode = push_bit(1);
        if (!outmode) outmode = push_bit(0);

        if (!outmode)
        {
          set_codes(zss_huffs::for_fixed());
          set_distances(zss_huffs::for_distance());

          get_codes().sort_by_value();
          get_distances().sort_by_value();

          for (i = 0; (!outmode) && (i < m_bytes_pend.size()); i++)
          {
            l = try_hash(i);
            if (l==1)
            {
              outmode = get_error();
            } else if (l)
            {
              i += l-1;
            } else {
              outmode = augmented_push_bit(m_bytes_pend[i]);
              if (!outmode)
                next_hash(m_bytes_pend[i]);
            }
          }

          augmented_push_bit(zenc_pair_stop);
        }
        m_has_blocks = true;
        if (m_last_block) outmode = ZSS_DONE;
        m_bytes_pend.resize(0);
      }break;
    }
    return outmode;
  }
  int zenc::push_bit(int x)
  {
    m_bithold = (m_bithold)|((x?1:0)<<m_bitcount);
    m_bitcount++;
    if (m_bitcount >= 8)
    {
      if (!append_no_history(m_bithold&255)) return ZSS_MEMORY;
      m_bithold = 0;
      m_bitcount = 0;
    }
    return 0;
  }
  int zenc::push_resync(void)
  {
    if (m_bitcount)
    {
      if (!append_no_history(m_bithold&255)) return ZSS_MEMORY;
      m_bithold = 0;
      m_bitcount = 0;
    }
    m_bithold = 0;
    return 0;
  }
  void zenc::next_hash(unsigned char x)
  {
    put_previous(x);
    if (get_history_length() >= 3)
    {
      unsigned int accum;
      accum = get_previous(1);
      accum += get_previous(2);
      accum += get_previous(3);

      m_hash.put_entry(accum,m_hashpos++);
    }
  }
  unsigned int zenc::try_hash(unsigned int curs)
  {
    if ((curs >= 3)
    &&  (m_bytes_pend.size() >= 6)
    &&  (curs < m_bytes_pend.size()-3))
    {
      unsigned int j, k, back, relback;
      unsigned int accum;
      accum = m_bytes_pend[curs]&255;
      accum += m_bytes_pend[curs+1]&255;
      accum += m_bytes_pend[curs+2]&255;
      for (j = 0; j < m_hash[accum].length(); j++)
      {
        back = m_hash[accum][j];
        /* confirm the usability of the history position */
        if (back > m_hashpos-3)
          continue;
        relback = m_hashpos-back;

        if (relback > get_history_length())
          continue;
        if (get_previous(relback) != m_bytes_pend[curs])
          continue;
        if (get_previous(relback-1) != m_bytes_pend[curs+1])
          continue;
        if (get_previous(relback-2) != m_bytes_pend[curs+2])
          continue;

        /* try to go as far as possible */
        k = 3;
        next_hash(m_bytes_pend[curs++]);
        next_hash(m_bytes_pend[curs++]);
        next_hash(m_bytes_pend[curs++]);
        for (; k < 258 && curs < m_bytes_pend.size(); curs++)
        {
          if (get_previous(relback) == m_bytes_pend[curs])
          {
            next_hash(m_bytes_pend[curs]);
            k++;
          } else
            break;
        }

        augmented_push_bit(zenc_pair(k,relback));

        return k;
      }
    }
    return 0;
  }
  unsigned char zenc::augmented_previous
    (unsigned int curs, unsigned int pos)
  {
    if (pos <= curs)
    {
      return m_bytes_pend[curs-pos];
    } else {
      return get_previous(pos-curs);
    }
  }
  int zenc::augmented_push_bit(const zenc_pair& ndv)
  {
    int out = 0;
    unsigned int i, l;
    if ((ndv.literal() >= 0) && (ndv.literal() <= 256))
    {
      zss_huff_pair hp = get_codes().get_bits(ndv.literal());
      if (hp.len)
      {
        while (hp.len--)
        {
          out = push_bit((hp.bits&(1<<hp.len))?1:0);
        }
      } else {
        out = ZSS_DATA_ERROR;
      }
    } else if ((ndv.literal() >= 257) && (ndv.literal() <= 285))
    {
      zss_huff_pair hp;
      hp = get_codes().get_bits(ndv.length());
      if (hp.len)
      {
        while (hp.len--)
        {
          out = push_bit((hp.bits&(1<<hp.len))?1:0);
        }
      } else {
        out = ZSS_DATA_ERROR;
      }

      if (!out)
      {
        if (ndv.length() < 261)
        {
          l = 0;
        } else if (ndv.length() == 285)
        {
          l = 0;
        } else {
          l = (ndv.length()-261)>>2;
        }

        for (i = 0; (!out) && i < l; i++)
        {
          out = push_bit((ndv.length_ext()&(1<<i))?1:0);
        }
      }

      if (!out)
      {
        hp = get_distances().get_bits(ndv.distance());
        if (hp.len)
        {
          while (hp.len--)
          {
            out = push_bit((hp.bits&(1<<hp.len))?1:0);
          }
        } else {
          out = ZSS_DATA_ERROR;
        }
      }

      if (!out)
      {
        if (ndv.distance() < 2)
        {
          l = 0;
        } else {
          l = (ndv.distance()-2)>>1;
        }

        for (i = 0; (!out) && i < l; i++)
        {
          out = push_bit((ndv.distance_ext()&(1<<i))?1:0);
        }
      }
    } else {
      out = ZSS_DATA_ERROR;
    }
    return out;
  }
};

