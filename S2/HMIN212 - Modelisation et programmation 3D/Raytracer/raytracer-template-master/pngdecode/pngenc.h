
#if !(defined CMPS3120_LABS_PNGENC)
#define CMPS3120_LABS_PNGENC

#include "./png.h"
#include "./zenc.h"

namespace cmps3120
{
  /**
   * \brief pseudo-random number generator
   */
  class png_enc_random
  {
  private:
    mutable unsigned int m_v;

  public:
    /**
     * Constructor
     * @param v a seed value
     */
    png_enc_random(unsigned int v = 1);
    /**
     * Copy constructor
     */
    png_enc_random(const png_enc_random& );
    /**
     * Destructor
     */
    ~png_enc_random(void);

    /**
     * @return the next random number value
     */
    operator unsigned int(void) const;
    /**
     * Set the seed.
     * @param v the new seed value
     */
    png_enc_random& operator=(unsigned int v);
    /**
     * Assignment
     */
    png_enc_random& operator=(const png_enc_random& );
  };

  /**
   * Portable Network Graphics image encoder
   */
  class png_encoder : public png_base
  {
  private:
    zenc xenc;
    unsigned char xbuf[16];
    unsigned int m_max;
    png_enc_random m_rand;
    png_buffer m_pend;
    unsigned int m_pendpos;
    png_pixel m_tmptrns;
    unsigned int m_d_chunk_size;

  public:
    /**
     * Constructor
     */
    png_encoder(void);
    /**
     * Destructor
     */
    ~png_encoder(void);

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
     * Write a file.
     * @param name the name of the PNG file to write
     * @return the last error code. If the write was successful,
     *   PNG_DONE is returned.
     * @note don't forget to set the png_receptor first
     */
    png_error write_file(const char* name);
    
  protected:
    void reset_sub(void);
    png_error get_byte(unsigned char&);
    int generate_sample(void);

  };
};

#endif /*CMPS3120_PNGENC*/
