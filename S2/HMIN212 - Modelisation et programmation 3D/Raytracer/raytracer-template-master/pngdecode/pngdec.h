
#if !(defined CMPS3120_LABS_PNGDEC_H)
#define CMPS3120_LABS_PNGDEC_H

#include "./png.h"
#include "./zdec.h"
#include <stdio.h>

namespace cmps3120
{
  /**
   * Sample code:
   * \code
   // Your image/texture class:
   class image : public cmps3120::png_receptor
   {
     ...
   public:
     void set_header
        (cmps3120::png_header h);
     void set_pixel
        (unsigned int x, unsigned int y, unsigned int level, cmps3120::png_pixel v);
   }
   
   // ...
   
   //The part of the code where you load the images
   image the_sprite;
   png_decoder the_decoder;
   png_error result;
   the_decoder.set_receptor( &the_sprite );
   result = the_decoder.read_file("sprite.png");
   if (result == PNG_DONE)
     std::cout << "The image loaded successfully.";
   else
     std::cout << "There was an error loading the image.";
   
   \endcode
   */
  class png_decoder : public png_base
  {
  private:
    zdec xdec;
    unsigned char xbuf[16];
    unsigned int m_max;
    
  public:
    png_decoder(void);
    ~png_decoder(void);
    
    /**
     * @return the maximum dimension allowed
     */
    unsigned int get_max_dimension(void) const;
    /**
     * Set the maximum dimension allowed
     * @param x the new maximum
     */
    void set_max_dimension(unsigned int x);
    
    /**
     * Read a file.
     * @param name the name of the PNG file to read
     * @return the last error code. If the read was successful,
     *   PNG_DONE is returned.
     * @note don't forget to set the png_receptor first
     */
    png_error read_file(const char* name);
    
  protected:
    void reset_sub(void);
    png_error put_char(unsigned char);
    void process_sample(unsigned char);
  };
};

#endif /* CMPS3120_LABS_PNGDEC_H */
