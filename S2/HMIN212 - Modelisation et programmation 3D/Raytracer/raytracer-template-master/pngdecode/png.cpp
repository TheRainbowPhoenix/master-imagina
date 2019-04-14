
#include "./png.h"
#include <stdlib.h>
#include <stdexcept>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define pass (0)

namespace cmps3120
{
  static unsigned long int png_crc_cache[256];
  static bool png_crc_cache_tf = false;
  static void png_make_crc_cache(void);

  unsigned long int png_color_recast
    (unsigned long int nw, unsigned int bd, unsigned int ct, bool no_throw_tf)
  {
    int u = 1;
    if (bd == 16)
    {
      if (ct & PNG_HAS_PALETTE)
      {
        /* pass; */
      } else {
        if (ct & PNG_HAS_COLOR)
          u+=2;
        if (ct & PNG_HAS_ALPHA)
          u++;
      }
      if (nw > (UINT_MAX/(2*u)))
      {
        errno = ERANGE;
        if (!no_throw_tf)
          throw std::range_error("cmps3120::png_color_recast");
        return UINT_MAX;
      } else {
        return nw*2*u;
      }
    } else if (bd == 8)
    {
      if (ct & PNG_HAS_PALETTE)
      {
        pass;
      } else {
        if (ct & PNG_HAS_COLOR)
          u+=2;
        if (ct & PNG_HAS_ALPHA)
          u++;
      }
      if (nw > (UINT_MAX/u))
      {
        errno = ERANGE;
        if (!no_throw_tf)
          throw std::range_error("cmps3120::png_color_recast");
        return UINT_MAX;
      } else {
        return nw*u;
      }
    } else if ((ct & PNG_HAS_PALETTE) || (!ct))
    {
      switch (bd)
      {
      case 1:
        {
          return (nw/8)+((nw&7) ? 1 : 0);
        }break;
      case 2:
        {
          return (nw/4)+((nw&3) ? 1 : 0);
        }break;
      case 4:
        {
          return (nw/2)+((nw&1) ? 1 : 0);
        }break;
      default:
        {
          errno = EDOM;
          if (!no_throw_tf)
            throw std::domain_error("cmps3120::png_color_recast");
          return 0;
        }break;
      }
    }
    if (!no_throw_tf)
      throw std::domain_error("cmps3120::png_color_recast");
    errno = EDOM;
    return 0;
  }
  unsigned char png_paeth_predict
    (unsigned char left, unsigned char up, unsigned char corner)
  {
    int point = (int)left, xleft, xup, xcorner;
    point += (int)up;
    point -= (int)corner;
    xleft = abs(point-left);
    xup   = abs(point-up);
    xcorner=abs(point-corner);
    if ((xleft<=xup) && (xleft<=xcorner)) return left;
    else if (xup<=xcorner) return up;
    else return corner;
  }
  
  png_header::png_header(void)
  : width(1), height(1), bit_depth(1), color_type(0), compression_type(0),
    filter_type(0), interlace_type(0)
  {
  }
  png_header::~png_header(void)
  {
  }
  bool png_header::is_supported(void) const
  {
    if (compression_type != 0)
      return false;
    if (filter_type != 0)
      return false;
    if (interlace_type > 1)
      return false;
    
    
    switch (color_type)
    {
    case 0:
      if ((bit_depth != 1)
      &&  (bit_depth != 2)
      &&  (bit_depth != 4)
      &&  (bit_depth != 8)
      &&  (bit_depth != 16))
        return false;
      break;
    case 2:
    case 4:
    case 6:
      if ((bit_depth != 8)
      &&  (bit_depth != 16))
        return false;
      break;
    case 3:
      if ((bit_depth != 1)
      &&  (bit_depth != 2)
      &&  (bit_depth != 4)
      &&  (bit_depth != 8))
        return false;
      break;
    default:
      return false;
    }
    return true;
  }
  
  png_pixel::png_pixel() : r(0), g(0), b(0), a(0) {}
  png_pixel::png_pixel(const png_pixel& p) : r(p.r), g(p.g), b(p.b), a(p.a) {}
  png_pixel& png_pixel::operator=(const png_pixel& p)
  { r=p.r;g=p.g;b=p.b;a=p.a;return *this; }
  png_pixel::png_pixel(const png_color& p)
  : r(p.r*257), g(p.g*257), b(p.b*257), a(p.a*257) {}
  png_pixel& png_pixel::operator=(const png_color& p)
  { r=p.r*257;g=p.g*257;b=p.b*257;a=p.a*257;return *this; }
  png_pixel::operator png_color(void) const
  {
    png_color out;
    out.r = (r+128)>>8;
    out.g = (g+128)>>8;
    out.b = (b+128)>>8;
    out.a = (a+128)>>8;
    return out;
  }
  bool png_pixel::operator==(const png_pixel& other)
  {
    return (other.r == r)&&(other.g==g)
      &&(other.b==b)&&(other.a==a);
  }
  png_pixel png_pixel::down_cast(unsigned int n) const
  {
    png_pixel out;
    unsigned int i = 1;
    if (n < 16)
    {
      i = 65535/((1<<n)-1);
    }
    if (!i) i = 1;
    out.r = r/i;
    out.g = g/i;
    out.b = b/i;
    out.a = a/i;
    return out;
  }
  png_pixel png_pixel::up_cast(unsigned int n) const
  {
    png_pixel out;
    unsigned int i = 1;
    if (n < 16)
    {
      i = 65535/((1<<n)-1);
    }
    if (!i) i = 1;
    out.r = r*i;
    out.g = g*i;
    out.b = b*i;
    out.a = a*i;
    return out;
  }
  unsigned int png_pixel::gray(void) const
  {
    unsigned long int out = 0;
    out = r*5;
    out += g*9;
    out += b*2;
    return (unsigned int)(out >> 4);
  }

  
  void png_make_crc_cache(void)
  {
    unsigned long c;
    int n, k;
    for (n = 0; n < 256; n++) {
      c = (unsigned long) n;
      for (k = 0; k < 8; k++) {
        if (c & 1)
          c = 0xedb88320L ^ (c >> 1);
        else
          c = c >> 1;
      }
      png_crc_cache[n] = c;
    }
    png_crc_cache_tf = true;
  }

  png_checksum::png_checksum(unsigned long int l)
  : m_x(l^0xffffffff)
  {
    /*fprintf(stderr,"cache %s %08lx\n",(png_crc_cache_tf?"true":"false"),
      m_x
    );*/
    if (!png_crc_cache_tf)
      png_make_crc_cache();
  }
  png_checksum& png_checksum::add(unsigned char ch)
  {
    m_x = png_crc_cache[(m_x ^ ch) & 0xff] ^ (m_x >> 8);
    return *this;
  }
  png_checksum::operator unsigned long int(void) const
  {
    return m_x^0xffffffff;
  }
  
  png_palette::png_palette(void)
  : m_colors(NULL), m_count(0)
  {
    m_alpha.r=0;
    m_alpha.g=0;
    m_alpha.b=0;
    m_alpha.a=0;
  }
  png_palette::png_palette(const png_palette& other)
  : m_colors(NULL), m_count(0)
  {
    m_alpha = other.m_alpha;
    if (resize(other.m_count))
    {
      if (m_count)
      memcpy(m_colors,other.m_colors,sizeof(png_color)*m_count);
    }
  }
  png_palette& png_palette::operator=(const png_palette& other)
  {
    m_alpha = other.m_alpha;
    if (resize(other.m_count))
    {
      if (m_count)
      memcpy(m_colors,other.m_colors,sizeof(png_color)*m_count);
    }
    return *this;
  }
  png_palette::~png_palette(void)
  {
    if (m_colors != NULL)
    {
      free(m_colors);
      m_colors = NULL;
    }
    m_count = 0;
  }
  unsigned int png_palette::size(void) const
  {
    return m_count;
  }
  bool png_palette::resize(unsigned int l)
  {
    if (l == m_count)
    {
      return true;
    } else if (!l)
    {
      if (m_colors != NULL)
      {
        free(m_colors);
        m_colors = NULL;
      }
      m_count = 0;
      return true;
    } else if (l <= ((UINT_MAX/sizeof(png_color))-16))
    {
      png_color* nc = (png_color*)realloc(m_colors,sizeof(png_color)*l);
      if (nc != NULL)
      {
        m_colors = nc;
        for (; m_count < l; m_count++)
        {
          m_colors[m_count].r = 0;
          m_colors[m_count].g = 0;
          m_colors[m_count].b = 0;
          m_colors[m_count].a = 255;
        }
        m_count = l;
        return true;
      } else return false;
    } else return false;
  }
  png_color& png_palette::operator[](unsigned int i)
  {
    if (i >= m_count)
      throw std::out_of_range("png_palette::operator[]");
    return m_colors[i];
  }
  const png_color& png_palette::operator[](unsigned int i) const
  {
    if (i >= m_count)
      throw std::out_of_range("png_palette::operator[]");
    return m_colors[i];
  }
  png_pixel& png_palette::get_transparent_pixel(void){ return m_alpha; }
  const png_pixel& png_palette::get_transparent_pixel(void) const
    { return m_alpha; }
  int png_palette::closest_match(png_pixel v, int siz)
  {
    unsigned int i, l = ((siz >= 0) ? siz : m_count);
    int j = -1;
    double lsq = 0;
    if (l > m_count) l = m_count;
    if (m_colors == NULL) return -1;
    for (i = 0; i < l; i++)
    {
      double nu, nv;
      png_pixel p = m_colors[i];
      nu = v.r-p.r;
      nv = nu*nu;
      nu = v.g-p.g;
      nv+= nu*nu;
      nu = v.b-p.b;
      nv+= nu*nu;
      nu = v.a-p.a;
      nv+= nu*nu;
      if (nv == 0)
        return i;
      else if (nv < lsq || (!i))
      {
        lsq = nv;
        j = i;
      }
    }
    return j;
  }
  
  png_buffer::png_buffer(void)
  : m_dta(NULL), m_siz(0)
  {
  }
  png_buffer::png_buffer(const png_buffer& b)
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
  png_buffer& png_buffer::operator=(const png_buffer& b)
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
  png_buffer::~png_buffer(void)
  {
    if (m_dta != NULL)
      free(m_dta);
    m_dta = NULL;
    m_siz = 0;
  }
  
  unsigned char* png_buffer::data(void){ return m_dta; }
  const unsigned char* png_buffer::data(void) const{ return m_dta; }
  
  unsigned int png_buffer::size(void) const{ return m_siz; }
  
  bool png_buffer::resize(unsigned int s)
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

  unsigned char& png_buffer::operator[](unsigned int i)
  {
    return *(m_dta+i);
  }
  const unsigned char& png_buffer::operator[](unsigned int i) const
  {
    return *(m_dta+i);
  }
  bool png_buffer::append(int C)
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

  png_adam7_data::png_adam7_data(int level)
  {
    if ((level >= 1) && (level <= 7))
    {
      w = (1<<((7-level)>>1));
      h = (1<<((8-level)>>1));
      xoff = ((level&1) ? 0 : (1<<((6-level)>>1)));
      yoff = (((level&1)&&(level^1)) ? (1<<((7-level)>>1)) : 0);
    } else {
      w=1; h=1; xoff=0; yoff=0;
    }
    /*fprintf(stderr,"level %i [w %u h %u x %u y %u]\n",level,
      w,h,xoff,yoff);*/
    l=level;
  }
  int png_adam7_data::get_level(void) const{ return l;}
  unsigned int png_adam7_data::compute_width(unsigned int inw)
  {
    unsigned int outw = (inw+(w-1))/(xoff+w);
    /* */
    //fprintf(stderr,"level %i cw < %u > %u\n",l,inw,outw);
    return outw;
  }
  unsigned int png_adam7_data::compute_height(unsigned int inh)
  {
    unsigned int outh = (inh+(h-1))/(yoff+h);
    /* */
    //fprintf(stderr,"level %i ch < %u > %u\n",l,inh,outh);
    return outh;
  }
  unsigned int png_adam7_data::real_x(unsigned int ix)
  {
    unsigned int ox = ix*(xoff+w) + xoff;
    //fprintf(stderr,"level %i rx < %u > %u\n",l,ix,ox);
    return ox;
  }
  unsigned int png_adam7_data::real_y(unsigned int iy)
  {
    unsigned int oy = iy*(yoff+h) + yoff;    
    //fprintf(stderr,"level %i ry < %u > %u\n",l,iy,oy);
    return oy;
  }

  
  /* png_receptor */
  png_receptor::~png_receptor(void)
  {
  }
  png_header png_receptor::get_header(void){ return png_header(); }
  void png_receptor::set_header(png_header h){ /*pass*/return; }
  png_pixel png_receptor::get_pixel(unsigned int x, unsigned int y, unsigned int level)
      { return png_pixel(); }
  void png_receptor::set_pixel
      (unsigned int x, unsigned int y, unsigned int level, png_pixel v)
      { /*pass*/return; }
  
  png_base::png_base(void)
  : m_xerr(0), m_receptor(NULL)
  {
    reset();
  }
  png_base::~png_base(void)
  {
  }
  void png_base::reset(void)
  {
    m_xerr = 0;
    m_crc = png_checksum();
    readmode = 0;
    submode = 0;sidemode = 0;
    xpect_nothing = 0;
    xlong = 0;
    xcharc=0;
    xpect_chunks = 0;
    xdiv_width = 0; xdiv_height = 0;
    xdiv_xpos = 0;  xdiv_ypos = 0; xdiv_index=0;
    filter_typ = 0;
    filter_dist =0;
    filter_backlog.resize(0);
    memset(&filter_log[0],0,8*sizeof(unsigned char));
    reset_sub();
  }
  png_error png_base::get_error(void) const{ return (png_error)m_xerr; }
  png_error png_base::put(const png_buffer& dta, unsigned int *pos)
  {
    return put(dta.data(), dta.size(), pos);
  }
  png_error png_base::put
    (const unsigned char* dta, unsigned int siz, unsigned int *pos)
  {
    int xout = m_xerr;
    unsigned int i;
    for (i = 0; (!xout) && (i < siz); i++, (xout?0:xcharc++))
    {
      xout = put_char(dta[i]);
    }
    if (pos)
    {
      *pos = i;
    }
    m_xerr = xout;
    return (png_error)xout;
  }

  png_header& png_base::get_header(void)
  {
    return m_header;
  }
  const png_header& png_base::get_header(void) const
  {
    return m_header;
  }
  png_palette& png_base::get_palette(void){ return m_palette; }
  const png_palette& png_base::get_palette(void) const{ return m_palette; }
  png_adam7_data& png_base::get_interlace_data(void){ return m_idata; }
  const png_adam7_data& png_base::get_interlace_data(void) const{ return m_idata; }
  png_receptor* png_base::get_receptor(void) const
  {
    return m_receptor;
  }
  void png_base::set_receptor(png_receptor* r)
  {
    m_receptor = r;
  }

  unsigned long int png_base::get_checksum(void) const
  {
    return m_crc;
  }
  void png_base::reset_checksum(void)
  {
    m_crc = png_checksum();
  }
  unsigned long int png_base::get_char_count(void) const
  {
    return xcharc;
  }
  void png_base::reset_sub(void)
  {
    pass;
  }
  void png_base::put_previous(unsigned char ch)
  {
    m_crc.add(ch);
  }
  void png_base::set_error(png_error v)
  {
    m_xerr = v;
  }
  png_error png_base::put_char(unsigned char)
  {
    return PNG_UNDEFINED;
  }
  png_error png_base::get(png_buffer& dta, unsigned int *pos)
  {
    return get(dta.data(), dta.size(), pos);
  }
  png_error png_base::get
    (unsigned char* dta, unsigned int siz, unsigned int *pos)
  {
    int xout = m_xerr;
    unsigned int i;
    for (i = 0; (!xout) && (i < siz); (xout?0:i++), (xout?0:xcharc++))
    {
      xout = get_byte(dta[i]);
    }
    if (pos)
    {
      *pos = i;
    }
    m_xerr = xout;
    return (png_error)xout;
  }
    
  png_error png_base::get_byte(unsigned char& y)
  {
    return PNG_UNDEFINED;
  }

};

