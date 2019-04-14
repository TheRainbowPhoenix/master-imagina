
#include "pngenc.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

namespace cmps3120
{
  unsigned char pngenc_std_header[8] =
    {137, 80, 78, 71, 13, 10, 26, 10 };
  unsigned char pngenc_chunk_iend[4] =
    { 0x49, 0x45, 0x4E, 0x44 };
  unsigned char pngenc_chunk_ihdr[4] =
    { 0x49, 0x48, 0x44, 0x52 };
  unsigned char pngenc_chunk_plte[4] =
    { 0x50, 0x4c, 0x54, 0x45 };
  unsigned char pngenc_chunk_trns[4] =
    { 0x74, 0x52, 0x4e, 0x53 };
  unsigned char pngenc_chunk_idat[4] =
    { 0x49, 0x44, 0x41, 0x54 };

  png_enc_random::png_enc_random(unsigned int v)
    : m_v(v)
  {
  }
  png_enc_random::png_enc_random(const png_enc_random& v)
    : m_v(v.m_v)
  {
  }
  png_enc_random::~png_enc_random(void)
  {
  }

  png_enc_random::operator unsigned int(void) const
  {
    unsigned int out = m_v;
    m_v >>= 1;
    m_v *= 0xce3f5a29;
    m_v += 0x39402958;
    //m_v ^= 0xa92536ab;
    //m_v ^= out;
    out = m_v%RAND_MAX;
    return out;
  }
  png_enc_random& png_enc_random::operator=(unsigned int v)
  {
    m_v = v;
    return *this;
  }
  png_enc_random& png_enc_random::operator=(const png_enc_random& v)
  {
    m_v = v.m_v;
    return *this;
  }
  
  
  png_encoder::png_encoder(void)
    : xenc(), m_max(100000), m_d_chunk_size(1024)
  {
    reset();
  }
  png_encoder::~png_encoder(void)
  {
  }

  unsigned int png_encoder::get_max_dimension(void) const
  {
    return m_max;
  }
  void png_encoder::set_max_dimension(unsigned int x)
  {
    m_max = x;
  }
  png_error png_encoder::write_file(const char* name)
  {
    FILE *nfile = fopen(name,"wb");
    if (nfile != NULL)
    {
      unsigned int readch; unsigned char buf[256];
      reset();
      while (!get_error())
      {
        get(buf,256,&readch);
        if (readch > 0)
          fwrite(buf,sizeof(unsigned char),readch,nfile);
      }
      fclose(nfile);
      return get_error();
    } else {
      return PNG_FILE_WRITE_FAILED;
    }
  }
  void png_encoder::reset_sub(void)
  {
    m_rand = time(NULL);
    memset(xbuf,0,sizeof(xbuf));
  }
  png_error png_encoder::get_byte(unsigned char& y)
  {
    int out = 0, i;
    switch (readmode)
    {
    case 0: /* header */
      {
        if (submode < 8)
        {
          y = pngenc_std_header[submode];
          submode++;
        }
        if (submode >= 8)
        {
          readmode = 1;
          submode = 0;
        }
      }break;
    case 1: /* IHDR length */
      {
        if (submode < 4)
        {
          y = (submode!=3 ? 0 : 13);
          submode++;
        }
        if (submode >= 4)
        {
          readmode = 2;
          submode = 0;
          reset_checksum();
        }
      }break;
    case 2: /* IHDR tag */
      {
        if (submode < 4)
        {
          y = pngenc_chunk_ihdr[submode];
          put_previous(pngenc_chunk_ihdr[submode]);
          submode++;
        }
        if (submode >= 4)
        {
          readmode = 3;
          submode = 0;
        }
      }break;
    case 3: /* IHDR data */
      {
        if (submode == 0)
        {
          png_header xh;
          if (get_receptor() != NULL)
          {
            xh = get_receptor()->get_header();
            if (!xh.is_supported())
              out = PNG_UNSUPPORTED_HEADER;
          } else {
            xh.width = 1;
            xh.height = 1;
          }

          memset(xbuf,0,sizeof(xbuf));
          for (i = 0; i < 4; i++)
          {
            xbuf[i] = (xh.width>>((3-i)<<3))&255;
          }
          for (i = 0; i < 4; i++)
          {
            xbuf[4+i] = (xh.height>>((3-i)<<3))&255;
          }
          xbuf[8] = xh.bit_depth;
          xbuf[9] = xh.color_type;
          xbuf[10]= xh.compression_type;
          xbuf[11]= xh.filter_type;
          xbuf[12]= xh.interlace_type;
          get_header() = (xh);
        }
        if (submode < 13)
        {
          y = xbuf[submode];
          put_previous(xbuf[submode]);
          submode++;
        }
        if (submode >= 13)
        {
          readmode = 4;
          submode = 0;
        }
      }break;
    case 4:
    case 8:
    case 12:
    case 16:
    case 24:
      {
        if (submode == 0)
        {
          xlong = get_checksum();
          reset_checksum();
        }
        if (submode < 4)
        {
          y = (xlong>>((3-submode)<<3))&255;
          submode++;
        }
        if (submode >= 4)
        {
          if (readmode == 4)
          {
            if (get_palette().size() > 0)
              readmode = 5;
            else if (get_palette().get_transparent_pixel().a
                && (!(get_header().color_type & PNG_HAS_PALETTE)))
              readmode = 13;
            else if (get_header().color_type & PNG_HAS_PALETTE)
              { readmode = 5; out = PNG_MISSING_PALETTE; }
            else
              readmode = 17;
            submode = 0;
          } else if (readmode == 8)
          {
            if (get_header().color_type & PNG_HAS_PALETTE)
              readmode = 9;
            else if (get_palette().get_transparent_pixel().a)
              readmode = 13;
            else
              readmode = 17;
            submode = 0;
          } else if ((readmode == 12)
          ||  (readmode == 16))
          {
            readmode = 17;
            submode = 0;
          } else if (readmode == 24)
          {
            readmode = 25;
            submode = 0;
          }
        }
      }break;
    case 5: /* block length */
    case 9:
    case 13:
    case 21:
      {
        if (submode == 0)
        {
          if (readmode == 5)
          {
            xlong = get_palette().size()*3;
            if (xlong > 768)
              xlong = 768;
          } else if (readmode == 9)
          {
            xlong = get_palette().size();
            if (xlong > 256)
              xlong = 256;
          } else if (readmode == 13)
          {
            xlong = ((get_header().color_type & 2)
                ? 6 : 2);
          } else xlong = 0;
        }
        if (submode < 4)
        {
          y = (xlong>>((3-submode)<<3))&255;
          submode++;
        }
        if (submode >= 4)
        {
          if (readmode == 5)
            readmode = 6;
          else if (readmode == 9)
            readmode = 10;
          else if (readmode == 13)
            readmode = 14;
          else if (readmode == 21)
            readmode = 22;
          submode = 0;
        }
      }break;
    case 6: /* PLTE tag */
      {
        if (submode < 4)
        {
          y = pngenc_chunk_plte[submode];
          put_previous(pngenc_chunk_plte[submode]);
          submode++;
        }
        if (submode >= 4)
        {
          readmode = 7;
          submode = 0;
        }
      }break;
    case 7: /* palette data */
      {
        if ((unsigned int)submode < xlong)
        {
          unsigned int sm = submode/3;
          if (sm < get_palette().size())
          {
            switch (submode%3)
            {
            case 0:
              y = get_palette()[sm].r;
              put_previous(get_palette()[sm].r);
              break;
            case 1:
              y = get_palette()[sm].g;
              put_previous(get_palette()[sm].g);
              break;
            case 2:
              y = get_palette()[sm].b;
              put_previous(get_palette()[sm].b);
              break;
            }
            submode++;
          }
        }
        if ((unsigned int)submode >= xlong)
        {
          readmode = 8;
          submode = 0;
        }
      }break;
    case 10: /* tRNS tag */
    case 14:
      {
        if (submode < 4)
        {
          y = pngenc_chunk_trns[submode];
          put_previous(pngenc_chunk_trns[submode]);
          submode++;
        }
        if (submode >= 4)
        {
          if (readmode == 10)
            readmode = 11;
          else if (readmode == 14)
            readmode = 15;
          submode = 0;
        }
      }break;
    case 11:
      {
        if ((unsigned int)submode < xlong)
        {
          if ((unsigned int)submode < get_palette().size())
          {
            y = get_palette()[submode].a;
            put_previous(get_palette()[submode].a);
            submode++;
          }
        }
        if ((unsigned int)submode >= xlong)
        {
          readmode = 12;
          submode = 0;
        }
      }break;
    case 15:
      {
        if (submode == 0)
        {
          m_tmptrns = get_palette().get_transparent_pixel().down_cast
            (get_header().bit_depth);
        }
        if ((unsigned int)submode < xlong)
        {
          switch (submode)
          {
          case 0:
            y = (m_tmptrns.r>>8)&255;break;
          case 1:
            y = (m_tmptrns.r)&255;break;
          case 2:
            y = (m_tmptrns.g>>8)&255;break;
          case 3:
            y = (m_tmptrns.g)&255;break;
          case 4:
            y = (m_tmptrns.b>>8)&255;break;
          case 5:
            y = (m_tmptrns.b)&255;break;
          }
        }
        if ((unsigned int)submode >= xlong)
        {
          readmode = 16;
          submode = 0;
        }
      }break;
    case 17:
    case 18:
    case 19:
      {
        int err = 0;
        if (submode == 0)
        {
          if (readmode == 17)
          {
            xenc.reset();
            xenc.set_header(zss_header());
            readmode++;
            sidemode = 0;

            if (get_header().interlace_type == 1)
            {
              get_interlace_data() = png_adam7_data(1);
              xdiv_width = get_interlace_data().compute_width
                (get_header().width);
              xdiv_height = get_interlace_data().compute_height
                (get_header().height);
              xdiv_xpos = 0;
              xdiv_ypos = 0;
            } else {
              get_interlace_data() = png_adam7_data(0);
              xdiv_width = get_header().width;
              xdiv_height= get_header().height;
              xdiv_xpos = 0;
              xdiv_ypos = 0;
            }
            filter_typ =0;
            filter_backlog.resize(0);
            try
            {
              filter_backlog.resize(png_color_recast(xdiv_width,
                get_header().bit_depth, get_header().color_type)
                );
              filter_dist = png_color_recast(1,
                get_header().bit_depth, get_header().color_type);
              /*fprintf(stderr,"filtr %i <- %i,%i\n",filter_dist,
                get_header().color_type, get_header().bit_depth
              );*/
            } catch (...)
            {
              out = PNG_UNSUPPORTED_HEADER;
            }
          }
          xenc.clear_pending();
          submode++;
        }
        if (submode == 1)
        {
          while ((xenc.get_pending_count() < m_d_chunk_size)
              && (readmode < 19)
              && (!err))
          {
            xbuf[0] = generate_sample();
            err = xenc.put(&xbuf[0],1,NULL);
          }
          m_pendpos = 0;
          if ((readmode >= 19)
          &&  (!err))
            err = xenc.finish();
          if (err && (err != 1)) out = PNG_ZSS_ERROR;
          submode++;
          xlong = xenc.get_pending_count();
        }
        if (submode == 2)
        {
          /* length */
          if (m_pendpos < 4)
          {
            y = (xlong>>((3-m_pendpos)<<3))&255;
            m_pendpos++;
          } else {
            submode++;
            m_pendpos = 0;
          }
        }
        if (submode == 3)
        {
          /* tag */
          if (m_pendpos < 4)
          {
            y = pngenc_chunk_idat[m_pendpos];
            put_previous(pngenc_chunk_idat[m_pendpos]);
            m_pendpos++;
          } else {
            submode++;
            m_pendpos = 0;
          }
        }
        if (submode == 4)
        {
          if (m_pendpos < xenc.get_pending_count())
          {
            y = xenc.get_pending()[m_pendpos];
            put_previous(xenc.get_pending()[m_pendpos]);
            m_pendpos++;
          } else {
            submode = 5;
            m_pendpos = 0;
          }
        }
        if (submode == 5)
        {
          if (m_pendpos == 0)
          {
            xlong = get_checksum();
            reset_checksum();
          }
          if (m_pendpos < 4)
          {
            y = (xlong>>((3-m_pendpos)<<3))&255;
            m_pendpos++;
          }
          if (m_pendpos >= 4)
          {
            submode = 0;
            if (readmode == 19)
            {
              readmode =21;
            }
          }
        }
      }break;
    case 22: /* IEND tag */
      {
        if (submode < 4)
        {
          y = pngenc_chunk_iend[submode];
          put_previous(pngenc_chunk_iend[submode]);
          submode++;
        }
        if (submode >= 4)
        {
          readmode = (xlong ? 23 : 24);
          submode = 0;
        }
      }break;
    case 23:
      {
        if (xlong)
        {
          put_previous(y);
          xlong--;
        }
        if (!xlong)
          readmode = 24;
      }break;
    case 25: /* end of stream */
      {
        out = PNG_DONE;
      }break;
    default:
      {
        out = PNG_UNDEFINED;
      }break;
    }
    return (png_error)out;
  }

  int png_encoder::generate_sample(void)
  {
    int ch = 0;
    unsigned char xc;
    int xreadmode = get_interlace_data().get_level()+10;
    unsigned int i, lv;
    if (xreadmode == 10) xreadmode = 9;
    switch (sidemode)
    {
    case 0: /* filter */
      {
        filter_typ = m_rand%5u;
        sidemode++;
        xdiv_index=0;
        ch = filter_typ;
        /*fprintf(stderr,"<%u %lu>\n",filter_typ,
            xenc.get_char_count() );*/
      }break;
    case 1: /* data */
      {
        if (get_receptor() != NULL)
        {
          png_receptor *rcpt = get_receptor();
          png_adam7_data &idta = get_interlace_data();
          switch (get_header().bit_depth)
          {
          case 1:
            {
              xc = 0;
              if (get_header().color_type & PNG_HAS_PALETTE)
              {
                for (i = 0; (i < 8)&&(xdiv_xpos<xdiv_width);
                    i++, xdiv_xpos++)
                {
                 
                  lv = get_palette().closest_match
                    (rcpt->get_pixel
                      (idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                       idta.get_level()), 2);
                  xc |= ((lv&1)<<(7-i));
                }
              } else {
                for (i = 0; (i < 8)&&(xdiv_xpos<xdiv_width);
                    i++, xdiv_xpos++)
                {
                 
                  lv = (rcpt->get_pixel
                      (idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                       idta.get_level()).gray()>>15);
                  xc |= ((lv&1)<<(7-i));
                }
              }
            }break;
          case 2:
            {
              xc = 0;
              if (get_header().color_type & PNG_HAS_PALETTE)
              {
                for (i = 0; (i < 4)&&(xdiv_xpos<xdiv_width);
                    i++, xdiv_xpos++)
                {
                 
                  lv = get_palette().closest_match
                    (rcpt->get_pixel
                      (idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                       idta.get_level()), 4);
                  xc |= ((lv&3)<<((3-i)<<1));
                }
              } else {
                for (i = 0; (i < 4)&&(xdiv_xpos<xdiv_width);
                    i++, xdiv_xpos++)
                {
                 
                  lv = (rcpt->get_pixel
                      (idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                       idta.get_level()).gray()>>14);
                  xc |= ((lv&3)<<((3-i)<<1));
                }
              }
            }break;
          case 4:
            {
              xc = 0;
              if (get_header().color_type & PNG_HAS_PALETTE)
              {
                for (i = 0; (i < 2)&&(xdiv_xpos<xdiv_width);
                    i++, xdiv_xpos++)
                {
                 
                  lv = get_palette().closest_match
                    (rcpt->get_pixel
                      (idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                       idta.get_level()), 16);
                  xc |= ((lv&15)<<((1-i)<<2));
                }
              } else {
                for (i = 0; (i < 2)&&(xdiv_xpos<xdiv_width);
                    i++, xdiv_xpos++)
                {
                 
                  lv = (rcpt->get_pixel
                      (idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                       idta.get_level()).gray()>>12);
                  xc |= ((lv&15)<<((1-i)<<2));
                }
              }
            }break;
          case 8:
            {
              if (get_header().color_type == 0)
              {
                xc = (rcpt->get_pixel
                    (idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                     idta.get_level()).gray()>>8);
                xdiv_xpos++;
              } else if (get_header().color_type == 2)
              {
                switch (xdiv_index%3)
                {
                  case 0:
                    m_tmptrns = rcpt->get_pixel
                      (idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                       idta.get_level());
                    xc = m_tmptrns.r>>8;
                    break;
                  case 1:
                    xc = m_tmptrns.g>>8;
                    break;
                  case 2:
                    xc = m_tmptrns.b>>8;
                    xdiv_xpos++;
                    break;
                }
              } else if (get_header().color_type == 3)
              {
                lv = get_palette().closest_match
                  (rcpt->get_pixel
                    (idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                     idta.get_level()), 256);
                xc = lv;
                xdiv_xpos++;
              } else if (get_header().color_type == 4)
              {
                switch (xdiv_index%2)
                {
                  case 0:
                    m_tmptrns = rcpt->get_pixel
                      (idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                       idta.get_level());
                    xc = m_tmptrns.gray()>>8;
                    break;
                  case 1:
                    xc = m_tmptrns.a>>8;
                    xdiv_xpos++;
                    break;
                }
              } else if (get_header().color_type == 6)
              {
                switch (xdiv_index%4)
                {
                  case 0:
                    m_tmptrns = rcpt->get_pixel
                      (idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                       idta.get_level());
                    xc = m_tmptrns.r>>8;
                    break;
                  case 1:
                    xc = m_tmptrns.g>>8;
                    break;
                  case 2:
                    xc = m_tmptrns.b>>8;
                    break;
                  case 3:
                    xc = m_tmptrns.a>>8;
                    xdiv_xpos++;
                    break;
                }
              }
            }break;
          case 16:
            {
              if (get_header().color_type == 0)
              {
                switch (xdiv_index%2)
                {
                case 0:
                  m_tmptrns.r = (rcpt->get_pixel
                    (idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                     idta.get_level()).gray());
                  xc = (m_tmptrns.r>>8)&255;
                  break;
                case 1:
                  xc = (m_tmptrns.r)&255;
                  xdiv_xpos++;
                  break;
                }
              } else if (get_header().color_type == 2)
              {
                switch (xdiv_index%6)
                {
                  case 0:
                    m_tmptrns = rcpt->get_pixel
                      (idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                       idta.get_level());
                    xc = (m_tmptrns.r>>8)&255;
                    break;
                  case 1:
                    xc = (m_tmptrns.r&255);
                    break;
                  case 2:
                    xc = (m_tmptrns.g>>8)&255;
                    break;
                  case 3:
                    xc = (m_tmptrns.g&255);
                    break;
                  case 4:
                    xc = (m_tmptrns.b>>8)&255;
                    break;
                  case 5:
                    xc = (m_tmptrns.b&255);
                    xdiv_xpos++;
                    break;
                }
              } else if (get_header().color_type == 3)
              {
                switch (xdiv_index%2)
                {
                  case 0:
                    m_tmptrns.r = get_palette().closest_match
                      (rcpt->get_pixel
                        (idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                        idta.get_level()), 65535);
                    xc = (m_tmptrns.r>>8)&255;
                    break;
                  case 1:
                    xc = (m_tmptrns.r)&255;
                    xdiv_xpos++;
                }
              } else if (get_header().color_type == 4)
              {
                switch (xdiv_index%4)
                {
                  case 0:
                    m_tmptrns = rcpt->get_pixel
                      (idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                       idta.get_level());
                    m_tmptrns.r = m_tmptrns.gray();
                    xc = (m_tmptrns.r>>8)&255;
                    break;
                  case 1:
                    xc = (m_tmptrns.r)&255;
                    break;
                  case 2:
                    xc = (m_tmptrns.a>>8)&255;
                    break;
                  case 3:
                    xc = (m_tmptrns.a)&255;
                    xdiv_xpos++;
                    break;
                }
              } else if (get_header().color_type == 6)
              {
                switch (xdiv_index%8)
                {
                  case 0:
                    m_tmptrns = rcpt->get_pixel
                      (idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                       idta.get_level());
                    xc = (m_tmptrns.r>>8)&255;
                    break;
                  case 1:
                    xc = m_tmptrns.r&255;
                    break;
                  case 2:
                    xc = (m_tmptrns.g>>8)&255;
                    break;
                  case 3:
                    xc = (m_tmptrns.g&255);
                    break;
                  case 4:
                    xc = (m_tmptrns.b>>8)&255;
                    break;
                  case 5:
                    xc = (m_tmptrns.b&255);
                    break;
                  case 6:
                    xc = (m_tmptrns.a>>8)&255;
                    break;
                  case 7:
                    xc = (m_tmptrns.a&255);
                    xdiv_xpos++;
                    break;
                }
              }
            }break;
          default:
            xc = m_rand;
            break;
          }
        } else
          xc = m_rand; 
        xc&=255;
        // 
        ch = xc;
        switch (filter_typ)
        {
        case 0: /* no effect */
          break;
        case 1:
          {
            if ((xdiv_index >= filter_dist)
            &&  (xdiv_index < filter_backlog.size()))
            {
              ch = (xc-filter_log[(xdiv_index-filter_dist)&7])&255;
            }
            //ch=0;
          }break;
        case 2:
          {
            if (xdiv_index < filter_backlog.size())
            {
              ch = (xc-filter_backlog[xdiv_index])&255;
            }
            //ch=0;
          }break;
        case 3:
          {
            unsigned int avg = 0;
            if ((xdiv_index >= filter_dist)
            &&  (xdiv_index < filter_backlog.size()))
            {
              avg += filter_log[(xdiv_index-filter_dist)&7];
            }
            if (xdiv_index < filter_backlog.size())
            {
              avg += filter_backlog[xdiv_index];
            }
            ch = (xc-(avg>>1))&255;
            //ch=0;
          }break;
        case 4:
          {
            unsigned int a = 0, b = 0, c = 0, xpt;
            if ((xdiv_index >= filter_dist)
            &&  (xdiv_index < filter_backlog.size()))
            {
              a = filter_log[(xdiv_index-filter_dist)&7];
              c = filter_backlog[xdiv_index-filter_dist];
            }
            if (xdiv_index < filter_backlog.size())
            {
              b = filter_backlog[xdiv_index];
            }
            xpt = png_paeth_predict(a,b,c);
            ch = (xc-xpt)&255;
            //ch = 0;
          }break;
        default:
          break;
        }

        if (xdiv_index < filter_backlog.size())
        {
          if (xdiv_index >= 8)
            filter_backlog[xdiv_index-8] = filter_log[xdiv_index&7];
          filter_log[xdiv_index&7] = xc;
          xdiv_index++;
        }
        if (xdiv_index >= filter_backlog.size())
        {
          sidemode = 0;
          xdiv_ypos++;
          /*fprintf(stderr,"++ %lu\n",xdiv_ypos);*/
          for (xdiv_xpos = ((xdiv_index >= 8) ? (xdiv_index-8) : 0);
             xdiv_xpos<xdiv_index; xdiv_xpos++)
          {
            filter_backlog[xdiv_xpos] = filter_log[xdiv_xpos&7];
          }
          xdiv_index = 0;
          xdiv_xpos=0;
          memset(&filter_log[0],0,8*sizeof(unsigned char));
        }
        if (xdiv_ypos >= xdiv_height)
        {
          /* on to next interlace level */
          if (((xreadmode >= 11) && (xreadmode <= 17))
          ||  (xreadmode == 9))
          {
            /* recompute data */
            if ((xreadmode == 17)
            ||  (xreadmode == 9))
            {
              xreadmode++;
              /*fprintf(stderr,"x19: y < h: %lu < %lu\n",
                  xdiv_ypos,xdiv_height);*/
              readmode = 19;
            } else if ((xreadmode >= 11) && (xreadmode <= 16))
            {
              do
              {
                xreadmode++;
                /*fprintf(stderr,"xreadmode++ %i\n",xreadmode);*/
                get_interlace_data() = png_adam7_data(xreadmode-10);
                xdiv_width = get_interlace_data().compute_width
                  (get_header().width);
                xdiv_height = get_interlace_data().compute_height
                  (get_header().height);
              } while ( ((xreadmode >= 11) && (xreadmode <= 17))
                &&      ((xdiv_width == 0)|| (xdiv_height==0)));
              xdiv_xpos = 0;
              xdiv_ypos = 0;
              xdiv_index= 0;
              filter_typ =0;
              filter_backlog.resize(0);
              filter_backlog.resize(png_color_recast(xdiv_width,
                get_header().bit_depth, get_header().color_type,true)
                );
              filter_dist = png_color_recast(1,
                get_header().bit_depth, get_header().color_type,true);
              if (xreadmode > 17)
                readmode = 19;
            }
          }
        }
      }break;
    default:
      readmode = 19;
      break;
    }
    /* TODO fix */
    return ch;
  }
};

