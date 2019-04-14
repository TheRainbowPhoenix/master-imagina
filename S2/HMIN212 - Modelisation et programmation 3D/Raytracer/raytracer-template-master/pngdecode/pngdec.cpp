
#include "./pngdec.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

namespace cmps3120
{
  unsigned char pngdec_std_header[8] =
    {137, 80, 78, 71, 13, 10, 26, 10 };
  unsigned char pngdec_chunk_iend[4] =
    { 0x49, 0x45, 0x4E, 0x44 };
  unsigned char pngdec_chunk_ihdr[4] =
    { 0x49, 0x48, 0x44, 0x52 };
  unsigned char pngdec_chunk_plte[4] =
    { 0x50, 0x4c, 0x54, 0x45 };
  unsigned char pngdec_chunk_trns[4] =
    { 0x74, 0x52, 0x4e, 0x53 };
  unsigned char pngdec_chunk_idat[4] =
    { 0x49, 0x44, 0x41, 0x54 };
  
  png_decoder::png_decoder(void)
  : xdec(), m_max(60000)
  {
    reset();
  }
  png_decoder::~png_decoder(void)
  {
  }
  png_error png_decoder::read_file(const char* name)
  {
    FILE *nfile = fopen(name,"rb");
    if (nfile != NULL)
    {
      int readch; unsigned char buf[256];
      reset();
      while ((!get_error())
      &&  ((readch = fread(buf,sizeof(char),256,nfile)) > 0)
      ) {
        put(buf,readch,NULL);
      }
      fclose(nfile);
      return get_error();
    } else {
      return PNG_FILE_READ_FAILED;
    }
  }
  unsigned int png_decoder::get_max_dimension(void) const{ return m_max; }
  void png_decoder::set_max_dimension(unsigned int x){ m_max = x; }
  void png_decoder::reset_sub(void)
  {
    get_palette().resize(0);
    xdec.reset();
    xpect_length = 0;
    xpect_chunks = 0;
  }
  png_error png_decoder::put_char(unsigned char ch)
  {
    int outres = 0;
    switch (readmode)
    {
    case 0: /* header */
      {
        if (submode < 8)
        {
          xbuf[submode] = ch;
          submode++;
        }
        if (submode >= 8)
        {
          if (memcmp(xbuf,pngdec_std_header,8) )
          {
            outres = PNG_BAD_SIGNATURE;
          } else {
            readmode = 1;
            submode = 0;
            xpect_length = 0;
          }
        }
      }break;
    case 1: /* chunk length */
      {
        if (submode < 4)
        {
          xpect_length = (xpect_length<<8)+ch;
          submode++;
        }
        if (submode >= 4)
        {
          readmode = 2;
          submode = 0;
        }
      }break;
    case 2: /* chunk name */
      {   
        if (submode < 4)
        {
          xbuf[submode] = ch;
          put_previous(ch);
          submode++;
        }
        if (submode >= 4)
        {
          xbuf[4] =0;
          /*fprintf(stderr,"  (%s chunk[[:%5lx] +x%5lx %lo)\n",xbuf,
                     xcharc,xpect_length,xpect_chunks);*/
          if (memcmp(pngdec_chunk_iend,xbuf,4) == 0)
          {
            readmode = 5;
            xpect_chunks |= PNG_CC_IEND;
          } else if (memcmp(pngdec_chunk_ihdr,xbuf,4) == 0)
          {
            if (xpect_chunks & PNG_CC_IHDR)
            {
              outres = PNG_BAD_CHUNK;
            } else if (!xpect_length)
            {
              outres = PNG_MISSING_DATA;
            } else {
              readmode = 6;
              submode = 0;
              xpect_chunks |= PNG_CC_IHDR;
            }
          } else if (memcmp(pngdec_chunk_plte,xbuf,4) == 0)
          {
            if ((xpect_chunks & PNG_CC_PLTE)
            ||  (!(xpect_chunks&PNG_CC_IHDR)) )
            {
              outres = PNG_BAD_CHUNK;
            } else {
              readmode = 7;
              submode = 0;
              xpect_chunks |= PNG_CC_PLTE;
            }
          } else if (memcmp(pngdec_chunk_trns,xbuf,4) == 0)
          {
            if ((get_header().color_type & PNG_HAS_PALETTE)
            &&  (!(xpect_chunks&PNG_CC_PLTE)) )
            {
              outres = PNG_BAD_CHUNK;
            } else if ((!(get_header().color_type & PNG_HAS_PALETTE))
            &&  (!(xpect_chunks&PNG_CC_IHDR)) )
            {
              outres = PNG_BAD_CHUNK;
            } else {
              readmode = 8;
              submode = 0;
              xpect_chunks |= PNG_CC_tRNS;
            }
          } else if (memcmp(pngdec_chunk_idat,xbuf,4) == 0)
          {
            if ((get_header().color_type & PNG_HAS_PALETTE)
            &&  (!(xpect_chunks&PNG_CC_PLTE)) )
            {
              outres = PNG_BAD_CHUNK;
            } else if ((!(get_header().color_type & PNG_HAS_PALETTE))
            &&  (!(xpect_chunks&PNG_CC_IHDR)) )
            {
              outres = PNG_BAD_CHUNK;
            } else {
              submode = 0;
              if (!(xpect_chunks & PNG_CC_IDAT))
              {
                if (get_header().interlace_type == 1)
                {
                  get_interlace_data() = png_adam7_data(1);
                  xdiv_width = get_interlace_data().compute_width
                    (get_header().width);
                  xdiv_height = get_interlace_data().compute_height
                    (get_header().height);
                  xdiv_xpos = 0;
                  xdiv_ypos = 0;
                  readmode = 11;
                } else {
                  get_interlace_data() = png_adam7_data(0);
                  xdiv_width = get_header().width;
                  xdiv_height= get_header().height;
                  xdiv_xpos = 0;
                  xdiv_ypos = 0;
                  readmode = 9;
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
                  outres = PNG_UNSUPPORTED_HEADER;
                }
                xpect_chunks |= PNG_CC_IDAT;
              } else {
                if (get_header().interlace_type == 1)
                {
                  readmode = 10+get_interlace_data().get_level();
                } else {
                  readmode = 9;
                }
              }
              /*fprintf(stderr,"readmode <= %u\n",readmode);*/
            }
          } else if (!(xbuf[0]&0x20))
          {
            outres = PNG_UNSUPPORTED_CHUNK;
          } else {
            readmode = 5;xbuf[4] =0;
            /*fprintf(stderr,"%s chunk[[:%5lx] %lu %lo\n",xbuf,
                     xcharc,xpect_length,xpect_chunks);*/
          }
          
          if (xpect_length == 0)
          {
            readmode = 3;
            submode = 0;
            xpect_nothing = 0;
          }
        }
      }break;
    case 3:
      {
        if (submode < 4)
        {
          xpect_nothing = (xpect_nothing<<8)+ch;
          submode++;
        }
        if (submode >= 4)
        {
          /*fprintf(stderr,"  {%08x v %08x}\n",
                  xpect_nothing, get_checksum());*/
          if (xpect_nothing == get_checksum())
          {
            if (xpect_chunks & PNG_CC_IEND)
            {
              readmode = 4;
              outres = PNG_DONE;
            } else {
              /* read next chunk */
              readmode = 1;
              submode =0;
              xpect_length=0;
            }
            reset_checksum();
          } else
            outres = PNG_BAD_CHECKSUM;
        }
      }break;
    case 4: /* end of PNG file */
      {
        outres = PNG_DONE;
      }break;
    case 5: /* end-of-image/unknown chunk */
    case 10:
    case 18:
      {
        if (xpect_length > 0)
        {
          put_previous(ch);
          xpect_length--;
        }
        if (!xpect_length)
        {
          readmode = 3;
          submode =0;
          xpect_nothing = 0;
        }
      }break;
    case 6: /* header */
      {
        if (xpect_length > 0)
        {
          put_previous(ch);
          xpect_length--;
          if (submode < 13) xbuf[submode++] = ch;
        }
        if (!xpect_length)
        {
          if (submode < 13)
          {
            outres = PNG_MISSING_DATA;
          } else {
            int i;
            png_header xhdr;
            xhdr.width = 0;
            xhdr.height = 0;
            for (i = 0; i < 4; i++)
              xhdr.width = (xhdr.width<<8) + xbuf[i];
            for (i = 4; i < 8; i++)
              xhdr.height= (xhdr.height<<8) + xbuf[i];
            xhdr.bit_depth = xbuf[8];
            xhdr.color_type = xbuf[9];
            xhdr.compression_type = xbuf[10];
            xhdr.filter_type = xbuf[11];
            xhdr.interlace_type = xbuf[12];
            get_header() = xhdr;
            if ((xhdr.is_supported() )
            &&  (xhdr.width <= m_max)
            &&  (xhdr.height <= m_max))
            {
              if (get_receptor() != NULL)
              {
                get_receptor()->set_header(xhdr);
              }
            } else
              outres = PNG_UNSUPPORTED_HEADER;
          }
          readmode = 3;
          submode =0;
            xpect_nothing = 0;
        }
      }break;
    case 7: /* palette */
      {
        if (submode == 0)
        {
          xlong = xpect_length/3;
          if (xlong > 256) xlong = 256;
          if (get_palette().resize(xlong) )
          {
            /* do nothing */
          } else {
            outres = PNG_NO_MEMORY;
          }
        }
        if ((unsigned int)(submode/3) < get_palette().size())
        {  try{
          switch (submode%3)
          {
          case 0: get_palette()[submode/3].r = ch;break;
          case 1: get_palette()[submode/3].g = ch;break;
          case 2: get_palette()[submode/3].b = ch;break;
          } }catch(...) { };
          submode++;
        }
        if (xpect_length > 0)
        {
          xpect_length--;
          put_previous(ch);
        }
        if (!xpect_length)
        {
          readmode = 3;
          submode =0;
          xpect_nothing = 0;
        }
      }break;
    case 8: /* simple transparency */
      {
        if (get_header().color_type & PNG_HAS_PALETTE)
        {
          if ((unsigned int)submode < get_palette().size())
          {
            try{
              get_palette()[submode].a = ch;
            }catch(...) { };
            submode++;
          }
        } else {
          if (submode < 6)
          {
            png_pixel& xp = get_palette().get_transparent_pixel();
            switch (submode)
            {
            case 0: xp.r = ch;break;
            case 1: xp.r = (xp.r<<8)+ch;
              if (!(get_header().color_type&PNG_HAS_COLOR))
              {
                xp.g=xp.r;
                xp.b=xp.r;
                xp = xp.up_cast(get_header().bit_depth);
                xp.a = 65535;
              }break;
            case 2: xp.g = ch;break;
            case 3: xp.g = (xp.r<<8)+ch;break;
            case 4: xp.b = ch;break;
            case 5: xp.b = (xp.r<<8)+ch;
              xp = xp.up_cast(get_header().bit_depth);
              xp.a = 65535;break;
            }
            submode++;
          }
        }
        if (xpect_length > 0)
        {
          xpect_length--;
          put_previous(ch);
        }
        if (!xpect_length)
        {
          readmode = 3;
          submode =0;
          xpect_nothing = 0;
        }
      }break;
    case 9: /* pixel data : no interlace */
      {   
        if (xpect_length > 0)
        {
          xbuf[0] = ch;
          xdec.put(&xbuf[0],1,NULL);
          if (xdec.get_pending_count())
          {
            const zss_buffer &xlongbuf = xdec.get_pending();
            unsigned int xslong;
            for (xslong = 0; xslong < xlongbuf.size(); xslong++)
            {
              process_sample(xlongbuf[xslong]);
            }
            xdec.clear_pending();
          }
          if ((xdec.get_error() != 0)
          &&  (xdec.get_error() != 1))
          {
            outres = PNG_ZSS_ERROR;
          }
          xpect_length--;
          put_previous(ch);
        }
        if (!xpect_length)
        {
          readmode = 3;
          submode =0;
          xpect_nothing = 0;
        }
        //
      }break;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17: /* pixel data : interlace */
      {   
        if (xpect_length > 0)
        {
          xbuf[0] = ch;
          xdec.put(&xbuf[0],1,NULL);
          if (xdec.get_pending_count())
          {
            const zss_buffer &xlongbuf = xdec.get_pending();
            unsigned int xslong;
            for (xslong = 0; xslong < xlongbuf.size(); xslong++)
            {
              process_sample(xlongbuf[xslong]);
            }
            xdec.clear_pending();
          }
          if ((xdec.get_error() != 0)
          &&  (xdec.get_error() != 1))
          {
            outres = PNG_ZSS_ERROR;
          }
          xpect_length--;
          put_previous(ch);
        }
        if (!xpect_length)
        {
          readmode = 3;
          submode =0;
          xpect_nothing = 0;
        }
        //
      }break;
    default:
      outres = PNG_UNDEFINED;
      break;
    }
    return (png_error)outres;
  }
  void png_decoder::process_sample(unsigned char xc)
  {
    int i;
    png_pixel nv;
    switch (sidemode)
    {
    case 0: /* filter */
      {
        filter_typ = xc;
        sidemode++;
        xdiv_index=0;
      }break;
    case 1:
      {
        switch (filter_typ)
        {
        case 0: /* no effect */
          break;
        case 1:
          {
            if ((xdiv_index >= filter_dist)
            &&  (xdiv_index < filter_backlog.size()))
            {
              xc = (xc+filter_log[(xdiv_index-filter_dist)&7])&255;
            }
            //xc=0;
          }break;
        case 2:
          {
            if (xdiv_index < filter_backlog.size())
            {
              xc = (xc+filter_backlog[xdiv_index])&255;
            }
            //xc=0;
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
            xc = (xc+(avg>>1))&255;
            //xc=0;
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
            xc = (xc+xpt)&255;
            //xc = 0;
          }break;
        default:
          break;
        }
        
        if (get_receptor() != NULL)
        {
          png_receptor *rcpt = get_receptor();
          png_adam7_data &idta = get_interlace_data();
          switch (get_header().bit_depth)
          {
          case 1:
            if (get_header().color_type & PNG_HAS_PALETTE)
              for (i = 0; (i < 8)&&(xdiv_xpos<xdiv_width); i++, xdiv_xpos++)
              {
                nv.r = (xc&(1<<(7-i)))?1:0;
                if (nv.r < get_palette().size())
                {
                  nv = get_palette()[nv.r];
                } else {
                  nv.r = 0;nv.g = 0;nv.b = 0;nv.a=0;
                }
                rcpt->set_pixel(idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                      (readmode>=11)?readmode-10:0, nv);
              }
            else
              for (i = 0; i < 8; i++,xdiv_xpos++)
              {
                nv.r = (xc&(1<<(7-i)))?1:0;
                nv.g = nv.r*65535;
                nv.b = nv.g;
                nv.r = nv.g;
                nv.a = 65535;
                if (get_palette().get_transparent_pixel() == nv)
                {
                  nv.a = 0;
                }
                rcpt->set_pixel(idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                      (readmode>=11)?readmode-10:0, nv);
              }break;
          case 2:
            if (get_header().color_type & PNG_HAS_PALETTE)
              for (i = 0; i < 8; i+=2, xdiv_xpos++)
              {
                nv.r = (xc&(3<<(6-i)))>>(6-i);
                if (nv.r < get_palette().size())
                {
                  nv = get_palette()[nv.r];
                } else {
                  nv.r = 0;nv.g = 0;nv.b = 0;nv.a=0;
                }
                rcpt->set_pixel(idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                      (readmode>=11)?readmode-10:0, nv);
              }
            else
              for (i = 0; i < 8; i+=2,xdiv_xpos++)
              {
                nv.r = (xc&(3<<(6-i)))>>(6-i);
                nv.g = nv.r*21845;
                nv.b = nv.g;
                nv.r = nv.g;
                nv.a = 65535;
                if (get_palette().get_transparent_pixel() == nv)
                {
                  nv.a = 0;
                }
                rcpt->set_pixel(idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                      (readmode>=11)?readmode-10:0, nv);
              }break;
          case 4:
            if (get_header().color_type & PNG_HAS_PALETTE)
              for (i = 0; i < 8; i+=4, xdiv_xpos++)
              {
                nv.r = (xc&(15<<(4-i)))>>(4-i);
                if (nv.r < get_palette().size())
                {
                  nv = get_palette()[nv.r];
                } else {
                  nv.r = 0;nv.g = 0;nv.b = 0;nv.a=0;
                }
                rcpt->set_pixel(idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                      (readmode>=11)?readmode-10:0, nv);
              }
            else
              for (i = 0; i < 8; i+=4,xdiv_xpos++)
              {
                nv.r = (xc&(15<<(4-i)))>>(4-i);
                nv.g = nv.r*4369;
                nv.b = nv.g;
                nv.r = nv.g;
                nv.a = 65535;
                if (get_palette().get_transparent_pixel() == nv)
                {
                  nv.a = 0;
                }
                rcpt->set_pixel(idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                      (readmode>=11)?readmode-10:0, nv);
              }break;
          case 8:
            {
              switch (get_header().color_type)
              {
              case 0:
                {
                  nv.r = xc*257;
                  nv.g = nv.r;
                  nv.b = nv.r;
                  nv.a = 65535;
                  if (get_palette().get_transparent_pixel() == nv)
                  {
                    nv.a = 0;
                  }
                  rcpt->set_pixel(idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                    (readmode>=11)?readmode-10:0, nv);
                  xdiv_xpos++;
                }break;
              case 2:
                if (xdiv_index%3 == 2){
                  nv.r = filter_log[(xdiv_index-2)&7]*257;
                  nv.g = filter_log[(xdiv_index-1)&7]*257;
                  nv.b = xc*257;
                  nv.a = 65535;
                  if (get_palette().get_transparent_pixel() == nv)
                  {
                    nv.a = 0;
                  }
                  rcpt->set_pixel(idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                    (readmode>=11)?readmode-10:0, nv);
                  xdiv_xpos++;
                }break;
              case 3:
                {
                  if (xc < get_palette().size())
                  {
                    nv = get_palette()[xc];
                  } else {
                    nv.r = 0;nv.g = 0;nv.b = 0;nv.a=0;
                  }
                  rcpt->set_pixel(idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                      (readmode>=11)?readmode-10:0, nv);
                  xdiv_xpos++;
                }break;
              case 4:
                if (xdiv_index%2 == 1){
                  nv.r = filter_log[(xdiv_index-1)&7]*257;
                  nv.g = nv.r;
                  nv.b = nv.r;
                  nv.a = xc*257;
                  rcpt->set_pixel(idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                    (readmode>=11)?readmode-10:0, nv);
                  xdiv_xpos++;
                }break;
              case 6:
                if (xdiv_index%4 == 3){
                  nv.r = filter_log[(xdiv_index-3)&7]*257;
                  nv.g = filter_log[(xdiv_index-2)&7]*257;
                  nv.b = filter_log[(xdiv_index-1)&7]*257;
                  nv.a = xc*257;
                  rcpt->set_pixel(idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                    (readmode>=11)?readmode-10:0, nv);
                  xdiv_xpos++;
                }break;
              }
            }break;
          case 16:
            {
              switch (get_header().color_type)
              {
              case 0:
                if (xdiv_index%2 == 1){
                  nv.r = (filter_log[(xdiv_index-1)&7]<<8)+xc;
                  nv.g = nv.r;
                  nv.b = nv.r;
                  nv.a = 65535;
                  if (get_palette().get_transparent_pixel() == nv)
                  {
                    nv.a = 0;
                  }
                  rcpt->set_pixel(idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                    (readmode>=11)?readmode-10:0, nv);
                  xdiv_xpos++;
                }break;
              case 2:
                if (xdiv_index%6 == 5){
                  nv.r = (filter_log[(xdiv_index-5)&7]<<8)+
                    filter_log[(xdiv_index-4)&7];
                  nv.g = (filter_log[(xdiv_index-3)&7]<<8)+
                    filter_log[(xdiv_index-2)&7];
                  nv.b = (filter_log[(xdiv_index-1)&7]<<8)+xc;
                  nv.a = 65535;
                  if (get_palette().get_transparent_pixel() == nv)
                  {
                    nv.a = 0;
                  }
                  rcpt->set_pixel(idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                    (readmode>=11)?readmode-10:0, nv);
                  xdiv_xpos++;
                }break;
              case 3:
                if (xdiv_index&1){
                  nv.r = (filter_log[(xdiv_index-1)&7]<<8)+xc;
                  if (nv.r < get_palette().size())
                  {
                    nv = get_palette()[nv.r];
                  } else {
                    nv.r = 0;nv.g = 0;nv.b = 0;nv.a=0;
                  }
                  rcpt->set_pixel(idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                      (readmode>=11)?readmode-10:0, nv);
                  xdiv_xpos++;
                }break;
              case 4:
                if (xdiv_index%4 == 3){
                  nv.r = (filter_log[(xdiv_index-3)&7]<<8)+
                    filter_log[(xdiv_index-2)&7];
                  nv.g = nv.r;
                  nv.b = nv.r;
                  nv.a = (filter_log[(xdiv_index-1)&7]<<8)+xc;
                  rcpt->set_pixel(idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                    (readmode>=11)?readmode-10:0, nv);
                  xdiv_xpos++;
                }break;
              case 6:
                if ((xdiv_index&7) == 7){
                  nv.r = (filter_log[(xdiv_index-7)&7]<<8)+
                    filter_log[(xdiv_index-6)&7];
                  nv.g = (filter_log[(xdiv_index-5)&7]<<8)+
                    filter_log[(xdiv_index-4)&7];
                  nv.b = (filter_log[(xdiv_index-3)&7]<<8)+
                    filter_log[(xdiv_index-2)&7];
                  nv.a = (filter_log[(xdiv_index-1)&7]<<8)+xc;
                  rcpt->set_pixel(idta.real_x(xdiv_xpos),idta.real_y(xdiv_ypos),
                    (readmode>=11)?readmode-10:0, nv);
                  xdiv_xpos++;
                }break;
              }
            }break;
          }
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
          if (((readmode >= 11) && (readmode <= 17))
          ||  (readmode == 9))
          {
            //readmode++;
            /* recompute data */
            if ((readmode == 17)
            ||  (readmode == 9))
              readmode++;
            else if ((readmode >= 11) && (readmode <= 16))
            {
              do
              {
                readmode++;
                /*fprintf(stderr,"readmode++ %i\n",readmode);*/
                get_interlace_data() = png_adam7_data(readmode-10);
                xdiv_width = get_interlace_data().compute_width
                  (get_header().width);
                xdiv_height = get_interlace_data().compute_height
                  (get_header().height);
              } while ( ((readmode >= 11) && (readmode <= 17))
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
            }
          }
        }
      }break;
    }
    return /*xres */;
  }
  
};

