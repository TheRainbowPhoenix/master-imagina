
#include <stdlib.h>
#include "./zss.h"
#include <string.h>
#include <limits.h>
#include <stdio.h>

namespace cmps3120
{
    struct zss_huff_entry
    {
      unsigned long int bits;
#if (USHORT_MAX >= 289)
      unsigned short len;
      short val;
#else
      unsigned int len;
      int val;
#endif /*USHORT_MAX*/
    };

    zss_huffs::zss_huffs(void)
    : m_count(0), m_entries(NULL), m_alloc(0), m_sort(0)
    {
    }
    zss_huffs::~zss_huffs(void)
    {
      if (m_entries != NULL)
      {
        free(m_entries);
        m_entries = NULL;
      }
      m_count = 0;
    }
    zss_huffs::zss_huffs(const zss_huffs& h)
    : m_count(0), m_entries(NULL), m_alloc(0), m_sort(0)
    {
      if (h.m_count && resize(h.m_count))
      {
        memcpy(m_entries,h.m_entries,sizeof(struct zss_huff_entry)*m_count);
        m_sort = h.m_sort;
      }
    }
    zss_huffs& zss_huffs::operator=(const zss_huffs& h)
    {
      if (h.m_count && resize(h.m_count))
      {
        memcpy(m_entries,h.m_entries,sizeof(struct zss_huff_entry)*m_count);
        m_sort = h.m_sort;
      } else if ((!h.m_count) || (h.m_entries == NULL))
      {
        if (m_entries != NULL)
        {
          free(m_entries);
          m_entries = NULL;
        }
        m_count = 0;
        m_sort = 0;
      }
      return *this;
    }
    unsigned int zss_huffs::get_count(void) const
    {
      return m_count;
    }
    bool zss_huffs::resize(unsigned int nc)
    {
      if (nc >= ((UINT_MAX/sizeof(struct zss_huff_entry))-16))
      {
        return false;
      } else if (nc)
      {
        if (nc <= m_alloc)
        {
          m_count = nc;
          if (m_count < (m_alloc>>2))
          {
            struct zss_huff_entry* ne = (struct zss_huff_entry*)realloc
              (m_entries,(m_alloc>>1)*sizeof(struct zss_huff_entry));
            if (ne != NULL)
            {
              m_entries = ne;
              m_alloc = (m_alloc>>1);
            }
          }
        } else {
          struct zss_huff_entry* ne = (struct zss_huff_entry*)realloc
            (m_entries,nc*sizeof(struct zss_huff_entry));
          if (ne != NULL)
          {
            m_entries = ne;
            if (nc > m_count)
            {
              memset(m_entries+m_count,0,
                sizeof(struct zss_huff_entry)*(nc-m_count));
            }
            m_count = nc;
            m_alloc = nc;
          } else return false;
        }
        m_sort = 0;
      } else {
        if (m_entries != NULL)
        {
          free(m_entries);
          m_entries = NULL;
        }
        m_count = 0;
        m_sort = 0;
      }
      return true;
    }
    bool zss_huffs::append_code
      (unsigned int xlen, unsigned int val)
    {
      unsigned int nsiz = m_count;
      if (!resize(nsiz+1))
        return false;
      m_entries[nsiz].bits = 0;
      m_entries[nsiz].len  = xlen;
      m_entries[nsiz].val  = val;
      m_sort =0;
      return true;
    }
    int zss_huffs::cmp_entry_v(const void* aa, const void* bb)
    {
      const struct zss_huff_entry *a = (const struct zss_huff_entry*)aa;
      const struct zss_huff_entry *b = (const struct zss_huff_entry*)bb;
      
      if (a->len < b->len)
        return -1;
      if (a->len > b->len)
        return +1;
      if (a->bits < b->bits)
        return -1;
      if (a->bits > b->bits)
        return +1;
      return 0;
    }
    int zss_huffs::cmp_entry_w(const void* aa, const void* bb)
    {
      const struct zss_huff_entry *a = (const struct zss_huff_entry*)aa;
      const struct zss_huff_entry *b = (const struct zss_huff_entry*)bb;
      
      if (a->val < b->val)
        return -1;
      if (a->val > b->val)
        return +1;
      return 0;
    }
    void zss_huffs::sort_by_length(void)
    {
      unsigned int i;
      struct zss_huff_entry *xe;
      if (m_entries == NULL) return;
      for (i = 0, xe = m_entries; i < m_count; i++, xe++)
      {
        xe->bits = i;
      }
      qsort(m_entries, m_count, sizeof(struct zss_huff_entry),
        &zss_huffs::cmp_entry_v);
      /*for (i = 0, xe = m_entries; i < m_count; i++, xe++)
      {
        fprintf(stderr,"[%lo %u %i]\n",xe->bits,xe->len,xe->val);
      }*/
      m_sort = 1;
    }
    void zss_huffs::sort_by_bits(void)
    {
      /*unsigned int i;
      struct zss_huff_entry *xe;
      fprintf(stderr,"!!\n");*/
      if (m_entries == NULL) return;
      qsort(m_entries, m_count, sizeof(struct zss_huff_entry),
        &zss_huffs::cmp_entry_v);
      /*for (i = 0, xe = m_entries; i < m_count; i++, xe++)
      {
        fprintf(stderr,"[%lo %u %i]\n",xe->bits,xe->len,xe->val);
      }*/
      m_sort = 2;
    }
    void zss_huffs::print_err(void) const
    {
      unsigned int i;
      struct zss_huff_entry *xe;
      fprintf(stderr,"!!\n");
      if (m_entries == NULL) return;
      for (i = 0, xe = m_entries; i < m_count; i++, xe++)
      {
        fprintf(stderr,"[%lo %u %i]\n",xe->bits,xe->len,xe->val);
      }
    }
    void zss_huffs::sort_by_value(void)
    {
      /*unsigned int i;
      struct zss_huff_entry *xe;
      fprintf(stderr,"~~\n");*/
      if (m_entries == NULL) return;
      qsort(m_entries, m_count, sizeof(struct zss_huff_entry),
        &zss_huffs::cmp_entry_w);
      /*for (i = 0, xe = m_entries; i < m_count; i++, xe++)
      {
        fprintf(stderr,"[%lo %u %i]\n",xe->bits,xe->len,xe->val);
      }*/
      m_sort = 3;
    }
    bool zss_huffs::make_codes(void)
    {
      unsigned int i, j = 0;
      unsigned long int l = 0;
      if (m_entries == NULL) return true;
      for (i = 0; i < m_count; i++)
      {
        if (j < m_entries[i].len)
        {
          l<<=(m_entries[i].len-j);
          j = m_entries[i].len;
        } else if (!j)
        {
          m_entries[i].bits = 0;
          continue;
        }
        if ((1UL<<j)&l) return false;
        m_entries[i].bits = l;
        l++;
      }
      return true;
    }
    bool zss_huffs::sort_and_make(void)
    {
      bool res;
      sort_by_length();
      res = make_codes();
      if (res)
        sort_by_bits();
      return res;
    }
    zss_huffs zss_huffs::for_direct(void)
    {
      unsigned int i;
      zss_huffs out;
      for (i = 0; i < 256; i++)
      {
        out.append_code(8,i);
      }
      out.sort_and_make();
      return out;
    }
    zss_huffs zss_huffs::for_distance(void)
    {
      unsigned int i;
      zss_huffs out;
      for (i = 0; i < 31; i++)
      {
        out.append_code(5,i);
      }
      out.sort_and_make();
      return out;
    }
    zss_huffs zss_huffs::for_fixed(void)
    {
      unsigned int i;
      zss_huffs out;
      for (i = 0; i < 144; i++)
      {
        out.append_code(8,i);
      }
      for (; i < 256; i++)
      {
        out.append_code(9,i);
      }
      for (; i < 280; i++)
      {
        out.append_code(7,i);
      }
      for (; i < 288; i++)
      {
        out.append_code(8,i);
      }
      out.sort_and_make();
      return out;
    }
    int zss_huffs::get_value(unsigned long int bits, unsigned int len) const
    {
      struct zss_huff_entry xe, *xv;
      if ((m_entries == NULL)
      ||  (!m_count))
        return -1;
      xe.bits = bits;
      xe.len = len;
      xe.val = -1;
      if (m_sort == 2)
      {
        xv = (struct zss_huff_entry*)bsearch
          (&xe,m_entries,m_count,sizeof(struct zss_huff_entry),
          &cmp_entry_v);
      } else {
        unsigned int l;
        xv = NULL;
        for (l = 0; l < m_count; l++)
        {
          if (cmp_entry_v(&xe,m_entries+l) == 0)
          {
            xv = m_entries+l;
            break;
          }
        }
      }
      if (xv == NULL)
        return -1;
      return xv->val;
    }
    zss_huff_pair zss_huffs::get_bits(unsigned int value) const
    {
      struct zss_huff_entry xe, *xv;
      zss_huff_pair out = {0,0};
      if ((m_entries == NULL)
      ||  (!m_count))
        return out;
      xe.bits = -1;
      xe.len = -1;
      xe.val = value;
      if (m_sort == 3)
      {
        xv = (struct zss_huff_entry*)bsearch
          (&xe,m_entries,m_count,sizeof(struct zss_huff_entry),
          &cmp_entry_w);
      } else {
        unsigned int l;
        xv = NULL;
        for (l = 0; l < m_count; l++)
        {
          if (cmp_entry_w(&xe,m_entries+l) == 0)
          {
            xv = m_entries+l;
            break;
          }
        }
      }
      if (xv == NULL)
        return out;
      out.bits = xv->bits;
      out.len = xv->len;
      return out;
    }


    zss_header::zss_header(void)
    : fcheck(0), fdict(0), flevel(1),
      xmethod(8), xinfo(7)
    { 
    }
    zss_header::zss_header(unsigned int v)
    {
      fcheck = v&31; v>>=5;
      fdict = v&1; v>>=1;
      flevel = v&3; v>>=2;
      xmethod = v&15; v>>=4;
      xinfo = v&15;
    }
    zss_header::operator unsigned int (void) const
    {
      unsigned int v;
      v = xinfo&15; v<<=4;
      v |= xmethod&15; v<<=2;
      v |= flevel&3; v<<=1;
      v |= fdict&1; v<<=5;
      v |= fcheck&31;
      return v;
    }
    void zss_header::set_check(void)
    {
      unsigned int v = ((unsigned int)*this)&(~31U);
      fcheck = 31-(v % 31);
    }
    bool zss_header::is_valid(void) const
    {
      unsigned int v = (unsigned int)*this;
      return !(v % 31);
    }


    zss_checksum::zss_checksum(unsigned long int l)
    : xva(l&65535), xvb((l>>16)&65535)
    {
    }
    zss_checksum& zss_checksum::add(unsigned char ch)
    {
      xva = ((unsigned long int)xva+ch)%65521UL;
      xvb = ((unsigned long int)xvb+xva)%65521UL;
      return *this;
    }
    zss_checksum::operator unsigned long int(void) const
    {
      return ((unsigned long int)xva)|(((unsigned long int)xvb)<<16);
    }



    zss_buffer::zss_buffer(void)
    : m_dta(NULL), m_siz(0)
    {
    }
    zss_buffer::zss_buffer(const zss_buffer& b)
    : m_dta(NULL), m_siz(0)
    {
      if (b.m_siz && (b.m_dta != NULL))
      {
        unsigned char* nc = (unsigned char*)calloc
          (b.m_siz,sizeof(unsigned char));
        if (nc != NULL)
        {
          memcpy(nc,b.m_dta,b.m_siz*sizeof(unsigned char));
          m_dta = nc;
          m_siz = b.m_siz;
        }
      }
    }
    zss_buffer& zss_buffer::operator=(const zss_buffer& b)
    {
      if (m_dta != NULL)
      {
        free(m_dta);
        m_dta = NULL;
      }
      m_siz = 0;
      if (b.m_siz && (b.m_dta != NULL))
      {
        unsigned char* nc = (unsigned char*)calloc
          (b.m_siz,sizeof(unsigned char));
        if (nc != NULL)
        {
          memcpy(nc,b.m_dta,b.m_siz*sizeof(unsigned char));
          m_dta = nc;
          m_siz = b.m_siz;
        }
      }
      return *this;
    }
    zss_buffer::~zss_buffer(void)
    {
      if (m_dta != NULL)
        free(m_dta);
      m_dta = NULL;
      m_siz = 0;
    }
    
    unsigned char* zss_buffer::data(void){ return m_dta; }
    const unsigned char* zss_buffer::data(void) const{ return m_dta; }
    
    unsigned int zss_buffer::size(void) const{ return m_siz; }
    
    bool zss_buffer::resize(unsigned int s)
    {
      unsigned char *nc;
      if (s)
      {
        if (m_dta != NULL)
        {
          nc = (unsigned char*)realloc(m_dta,s*sizeof(unsigned char));
          if (nc != NULL)
          {
            m_dta = nc;
            if (s > m_siz)
            {
              memset(m_dta+m_siz,0,sizeof(unsigned char)*(s-m_siz));
            }
            m_siz = s;
          } else {
            return false;
          }
        } else {
          nc = (unsigned char*)calloc(s,sizeof(unsigned char));
          if (nc != NULL)
          {
            m_dta = nc;
            m_siz = s;
          } else {
            return false;
          }
        }
      } else {
        if (m_dta != NULL)
          free(m_dta);
        m_dta = NULL;
        m_siz = 0;
      }
      return true;
    }

    unsigned char& zss_buffer::operator[](unsigned int i)
    {
      return *(m_dta+i);
    }
    const unsigned char& zss_buffer::operator[](unsigned int i) const
    {
      return *(m_dta+i);
    }
    bool zss_buffer::append(int C)
    {
      unsigned int xs = m_siz;
      if (xs >= ((UINT_MAX-1)/sizeof(unsigned char)))
      {
        return false;
      }
      if (!resize(xs+1))
        return false;
      *(m_dta+xs) = (unsigned char)(C&255);
      return true;
    }
    
    zss::zss(void)
    : xerr(0), hist(), pend(),
      hist_pos(0), hist_len(0), pend_pos(0),
      xhdr(0), 
      dict_set_tf(0), dict_adler(0),
      
      readmode(0), submode(0),
      xpect_dict_adler(0), xlong(0),
      xcharc(0)
    {
    }
    zss::~zss(void)
    {
      hist.resize(0);
      pend.resize(0);
    }
    zss_error zss::get_error(void) const
    {
      return xerr;
    }
    void zss::set_error(zss_error v)
    {
      xerr = v;
    }
    void zss::reset(void)
    {
      reset_sub();
    }
    void zss::reset_sub(void)
    {
      readmode = 0;
      hist.resize(0);
      pend.resize(0);
      hist_len = 0;
      hist_pos = 0;
      pend_pos = 0;
      xerr = 0;
      submode = 0;
      xhdr = 0;
      xlong = 0;
      xadler = 1;
      dict_set_tf = false;
      dict_adler = 1;
      xpect_dict_adler = 1;
      xcharc = 0;
    }
    unsigned char zss::get_previous(unsigned int pos) const
    {
      if (pos > hist_len)
        return 0;
      else if (pos > hist.size())
        return 0;
      else if (pos > hist_pos)
        return hist[hist.size()-(pos-hist_pos)];
      else
        return hist[hist_pos-pos];
    }
    void zss::put_previous(unsigned char ch)
    {
      if (hist_len > hist.size())
        hist_len = hist.size();
      xadler.add(ch);
      if (!hist.size())
        return;
      if (hist_pos < hist_len)
      {
        hist[hist_pos++] = ch;
      } else if (hist_pos < hist.size())
      {
        hist_len++;
        hist[hist_pos++] = ch;
      } else {
        hist[(hist_pos=0)++] = ch;
      }
    }
    zss_error zss::put(const zss_buffer& dta, unsigned int *pos)
    {
      return put(dta.data(), dta.size(), pos);
    }
    zss_error zss::put
      (const unsigned char* dta, unsigned int siz, unsigned int *pos)
    {
      int xout = xerr;
      unsigned int i;
      for (i = 0; (!xout) && (i < siz); i++, (xout?0:xcharc++))
      {
        xout = put_char(dta[i]);
      }
      if (pos)
      {
        *pos = i;
      }
      xerr = xout;
      return (zss_error)xout;
    }
    zss_error zss::finish(void)
    {
      int xout = xerr;
      if (!xout)
      {
        xout = put_eof();
      }
      xerr = xout;
      return (zss_error)xout;
    }
    zss_error zss::put_eof(void)
    {
      return ZSS_UNDEFINED;
    }
    zss_error zss::put_char(unsigned char x)
    {
      return ZSS_UNDEFINED;
    }
    bool zss::is_dict_set(void) const
    {
      return dict_set_tf;
    }
    unsigned long zss::get_dict_checksum(void) const
    {
      return dict_adler;
    }
    bool zss::set_dictionary(const zss_buffer& dct)
    {
      if (readmode < 2)
      {
        unsigned int i;
        for (i = 0; i < dct.size(); i++)
        {
          put_previous(dct[i]);
        }
        dict_adler = xadler;
        xadler = 1;
        return true;
      } else return false;
    }
    const zss_header& zss::get_header(void) const
    {
      return xhdr;
    }
    void zss::set_header(const zss_header& h)
    {
      xhdr = h;
    }
    bool zss::setup_window(void)
    {
      unsigned int siz;
      if (xhdr.xinfo >= 8)
        return false;
      siz = 1<<(xhdr.xinfo+8);
      return hist.resize(siz);
    }
    zss_huffs& zss::get_codes(void)
    {
      return codes;
    }
    const zss_huffs& zss::get_codes(void) const
    {
      return codes;
    }
    void zss::set_codes(const zss_huffs& h)
    {
      codes = h;
    }
    zss_huffs& zss::get_extended_codes(void)
    {
      return xcodes;
    }
    const zss_huffs& zss::get_extended_codes(void) const
    {
      return xcodes;
    }
    void zss::set_extended_codes(const zss_huffs& h)
    {
      xcodes = h;
    }
    zss_huffs& zss::get_distances(void)
    {
      return dists;
    }
    const zss_huffs& zss::get_distances(void) const
    {
      return dists;
    }
    void zss::set_distances(const zss_huffs& h)
    {
      dists = h;
    }
    bool zss::append(unsigned char ch)
    {
      if (!pend.append(ch&255) )
        return false;
      put_previous(ch&255);
      return true;
    }
    bool zss::append_no_history(unsigned char ch)
    {
      return pend.append(ch&255);
    }
    unsigned int zss::get_pending_count(void) const
    {
      return pend.size();
    }
    zss_buffer& zss::get_pending(void){ return pend; }
    const zss_buffer& zss::get_pending(void) const{ return pend; }
  
    void zss::clear_pending(void){ pend.resize(0); }
    unsigned long int zss::get_checksum(void) const
      { return xadler; }
    unsigned int zss::get_history_length(void) const
      { return hist_len; }
    unsigned long int zss::get_char_count(void) const
      { return xcharc; }

};

