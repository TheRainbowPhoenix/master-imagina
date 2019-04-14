
#if !(defined CMPS3120_LABS_PNG_H)
#define CMPS3120_LABS_PNG_H

namespace cmps3120
{
  /**
   * Error codes from a \link png_base \endlink
   */
  typedef enum png_error_t
  {
    /** Undefined operation or state */
    PNG_UNDEFINED = -1,
    /** No error (yet) */
    PNG_OK = 0,
    
    /** End of image */
    PNG_DONE = 1,
    
    /** Bad file signature */
    PNG_BAD_SIGNATURE = 2,
    /** Bad checksum */
    PNG_BAD_CHECKSUM = 3,
    /** Duplicate or misplaced chunk. */
    PNG_BAD_CHUNK = 4,
    /** A chunk is missing necessary data */
    PNG_MISSING_DATA = 5,
    /** Header reports unsupported features */
    PNG_UNSUPPORTED_HEADER = 6,
    /** Primary chunk unsupported */
    PNG_UNSUPPORTED_CHUNK = 7,
    /** memory issues */
    PNG_NO_MEMORY = 8,
    /** zip stream issues */
    PNG_ZSS_ERROR = 9,
    
    /** the file could not be opened for reading */
    PNG_FILE_READ_FAILED = 10,
    /** the file could not be opened for reading */
    PNG_FILE_WRITE_FAILED = 11,
    /** no receptor has been set */
    PNG_MISSING_RECEPTOR = 12,
    /** 
     * no palette has been set although the receptor's
     * header requires a palete
     */
    PNG_MISSING_PALETTE = 13,
    
    PNG_LAST_ERROR = 14
  } png_error;

  /**
   * Common chunk flags
   */
  typedef enum png_chunk_flag_t
  {
    PNG_CC_IEND = 16,
    PNG_CC_IHDR = 1,
    PNG_CC_PLTE = 2,
    PNG_CC_IDAT = 8,
    PNG_CC_tRNS = 4
  } png_chunk_flag;
  
  /**
   * Portable Network Graphics color type
   */
  typedef enum png_color_type_t
  {
    /** @note \verbatim restrict bit-depth: 1,2,4,8\endverbatim */
    PNG_HAS_PALETTE = 1,
    /** @note \verbatim restrict bit-depth: 8,16\endverbatim */
    PNG_HAS_COLOR = 2,
    /** @note \verbatim restrict bit-depth: 8,16\endverbatim */
    PNG_HAS_ALPHA = 4,
    
    PNG_GRAYSCALE = 0,
    PNG_RGB = 2,
    PNG_RGB_PALETTE = 3,
    PNG_GRAYSCALE_ALPHA = 4,
    PNG_RGBA = 6
  } png_color_type;
  
  /**
   * Convert a pixel width into a scanline length.
   * @param nw the pixel width
   * @param bd sample bit depth
   * @param ct a \link png_color_type \endlink value
   * @param no_throw_tf if true, no exceptions will be thrown
   * @return the length of a scanline given the arguments
   * @throw std::range_error if the scanline is too large
   */
  unsigned long int png_color_recast
    (unsigned long int nw, unsigned int bd, unsigned int ct,
     bool no_throw_tf = false);
    
  /**
   * Run the Paeth predictor
   * @param left the left value
   * @param up the up value
   * @param corner the up-left value
   * @return the chosen value
   */
  unsigned char png_paeth_predict
    (unsigned char left, unsigned char up, unsigned char corner);
  
  /**
   * Portable Network Graphics compression method
   */
  typedef enum png_compression_type_t
  {
    /** zlib Deflate algorithm */
    PNG_DEFLATE = 0
  } png_compression_type;

  /**
   * Portable Network Graphics filter method
   */
  typedef enum png_filter_type_t
  {
    /** default 5-filter adaptive */
    PNG_ADAPTIVE = 0
  } png_filter_type;
  
  /**
   * Portable Network Graphics interlace method
   */
  typedef enum png_interlace_type_t
  {
    /** no interlace */
    PNG_NO_INTERLACE = 0,
    /** Adam7 interlace */
    PNG_ADAM7_INTERLACE = 1
  } png_interlace_type;
  
  
  class png_checksum
  {
  private:
    unsigned long int m_x;

  public:
    png_checksum(unsigned long int l = 0UL);
    png_checksum& add(unsigned char ch);
    operator unsigned long int(void) const;
  };
  
  /**
   * Portable Network Graphics image header
   */
  struct png_header
  {
  public:
    /**
     * Default constructor
     */
    png_header(void);
    /**
     * Destructor
     * @note does nothing
     */
    ~png_header(void);
    /**
     * Pixel width
     */
    unsigned long int width;
    /**
     * Pixel height
     */
    unsigned long int height;
    /**
     * Bit depth per sample
     */
    unsigned char bit_depth;
    /**
     * a \link png_color_type \endlink value
     */
    unsigned char color_type;
    /**
     * a \link png_compression_type \endlink value
     */
    unsigned char compression_type;
    /**
     * a \link png_filter_type \endlink value
     */
    unsigned char filter_type;
    /**
     * a \link png_interlace_type \endlink value
     */
    unsigned char interlace_type;
    
    /**
     * @return whether this implementation of PNG supports
     *   the given header
     */
    bool is_supported(void) const;
  };
  
  /**
   * Palette color entry
   */
  struct png_color
  {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
  };

  /**
   * Pixel in PNG image data
   */
  struct png_pixel
  {
    /** Default constructor */
    png_pixel();
    /** Copy constructor */
    png_pixel(const png_pixel& );
    /** Assignment operator */
    png_pixel& operator=(const png_pixel& );
    /** Copy constructor */
    png_pixel(const png_color& );
    /** Assignment operator */
    png_pixel& operator=(const png_color& );
    
    /** Conversion operator */
    operator png_color(void) const;
    /** Comparison operator */
    bool operator==(const png_pixel& other);
    /**
     * Convert from 16-bit to lower bit depth
     * @param n the lower bit depth
     * @return a copy of this pixel in the lower bit depth
     */
    png_pixel down_cast(unsigned int n) const;
    /**
     * Convert from lower bit depth to 16-bit
     * @param n the lower bit depth
     * @return a copy of this pixel at depth of 16 bits per sample
     */
    png_pixel up_cast(unsigned int n) const;
    /**
     * Converts the pixel RGB values to a single grayscale value.
     * @return a value between [0, 65535], inclusive.
     */
    unsigned int gray(void) const;
    
    /** Red value, range 0 - 65535 */
    unsigned int r;
    /** Green value, range 0 - 65535 */
    unsigned int g;
    /** Blue value, range 0 - 65535 */
    unsigned int b;
    /** Alpha value, range 0 - 65535 */
    unsigned int a;
  };

  /**
   * Portable Network Graphics palette
   */
  class png_palette
  {
  private:
    png_color* m_colors;
    unsigned int m_count;
    png_pixel m_alpha;
  
  public:
    /**
     * Constructor
     */
    png_palette(void);
    /**
     * Copy constructor
     */
    png_palette(const png_palette& );
    /**
     * Assignment
     */
    png_palette& operator=(const png_palette& );
    /**
     * Destructor
     */
    ~png_palette(void);
    
    /**
     * @return the size of the palette
     */
    unsigned int size(void) const;
    
    /**
     * Resize the palette.
     * @param l the desired size
     * @return zero on success
     */
    bool resize(unsigned int l);
    
    /**
     * @param i an index into the palette
     * @return the entry at the given index
     * @throw std::out_of_range if i is out of range
     */
    png_color& operator[](unsigned int i);

    /**
     * @param i an index into the palette
     * @return the entry at the given index
     * @throw std::out_of_range if i is out of range
     */
    const png_color& operator[](unsigned int i) const;
    
    /**
     * @return the pixel value to be marked as transparent
     */
    png_pixel& get_transparent_pixel(void);

    /**
     * @return the pixel value to be marked as transparent
     */
    const png_pixel& get_transparent_pixel(void) const;

    /**
     * Try to match a pixel to a palette color.
     * @param v the pixel color to try to match
     * @param siz one plus the last index to check; if negative,
     *   the entire palette is checked
     * @return the index of the best match, or -1 if none of the 
     *   palette could be checked
     */
    int closest_match(png_pixel v, int siz = -1);
  };
  
  /**
   * Data used by the PNG processor.
   */
  class png_buffer
  {
  private:
    unsigned char* m_dta;
    unsigned int m_siz;
  
  public:
    png_buffer(void);
    png_buffer(const png_buffer& );
    png_buffer& operator=(const png_buffer& );
    ~png_buffer(void);
    
    unsigned char* data(void);
    const unsigned char* data(void) const;
    
    unsigned int size(void) const;
    
    bool resize(unsigned int s);

    unsigned char& operator[](unsigned int );
    const unsigned char& operator[](unsigned int ) const;
    
    bool append(int C);
  };
  
  class png_adam7_data
  {
  private:
    unsigned char w, h, xoff, yoff;
    int l;
    
  public:
    png_adam7_data(int level = 0);
    
    /**
     * @return the interlace level described by this data set
     */
    int get_level(void) const;
    /**
     * Compute the width of a pass.
     * @param inw the input width
     * @return the height of interlace pass described by
     *   this data set
     */
    unsigned int compute_width(unsigned int inw);
    /**
     * Compute the height of a pass.
     * @param inh the input height
     * @return the height of interlace pass described by
     *   this data set
     */
    unsigned int compute_height(unsigned int inh);
    
    /**
     * Convert interlace coordinates to real coordinates.
     * @param inx x-axis interlace coordinate
     * @return the x-coordinate of the corresponding real coordinate
     */
    unsigned int real_x(unsigned int ix);

    /**
     * Convert interlace coordinates to real coordinates.
     * @param iny y-axis interlace coordinate
     * @return the y-coordinate of the corresponding real coordinate
     */
    unsigned int real_y(unsigned int iy);
  };
  
  /**
   * \link png_base \endlink input/output receptor
   * \note \b interface 
   * 
   * To use with the png_decoder class, override the
   * \link #set_header \endlink and \link #set \endlink
   * methods. The width, height and other information 
   * will be sent through the set_header method, while
   * the pixel values will be sent through the set
   * method.
   * 
   */
  class png_receptor
  {
  public:
    virtual ~png_receptor(void);
    
    virtual png_header get_header
      (void);
    virtual void set_header
      (png_header h);
    virtual png_pixel get_pixel
      (unsigned int x, unsigned int y, unsigned int level);
    virtual void set_pixel
      (unsigned int x, unsigned int y, unsigned int level, png_pixel v);
  };

  /**
   * Portable Network Graphics stream processor
   * base class
   * 
   */
  class png_base
  {
  private:
    int m_xerr;
    png_header m_header;
    png_palette m_palette;
    png_checksum m_crc;
    png_adam7_data m_idata;
    png_receptor *m_receptor;
    
  public: 
    /**
     * Constructor
     */
    png_base(void);
    
    /**
     * Destructor
     */
    virtual ~png_base(void);
    
    /**
     * Reset the stream.
     * @note Calls \link #reset_sub \endlink
     */
    void reset(void);
    
    /**
     * Get the last error.
     * @return the last error encountered
     */
    png_error get_error(void) const;
    
    /**
     * Process an array of character/byte data.
     * @param dta the PNG image data to process
     * @param[out] pos the position in the data array at which the
     *   last error was obtained
     * @return an error code, or zero on success
     */
    png_error put(const png_buffer& dta, unsigned int *pos = 0L);

    /**
     * Process an array of character/byte data.
     * @param dta the PNG image data to process
     * @param siz the number of bytes in the data
     * @param[out] pos the position in the data array at which the
     *   last error was obtained
     * @return an error code, or zero on success
     */
    png_error put
      (const unsigned char* dta, unsigned int siz, unsigned int *pos = 0L);
  

    /**
     * @return the header
     */
    png_header& get_header(void);
    
    /**
     * @return the header
     */
    const png_header& get_header(void) const;
    
    /**
     * @return the palette
     */
    png_palette& get_palette(void);

    /**
     * @return the palette
     */
    const png_palette& get_palette(void) const;
    
    /**
     * Get the chunk checksum.
     */
    unsigned long int get_checksum(void) const;
    
    /**
     * Reset the chunk checksum.
     */
    void reset_checksum(void);
    
    /**
     * @return the interlace data set
     */
    png_adam7_data& get_interlace_data(void);
    
    /**
     * @return the interlace data set
     */
    const png_adam7_data& get_interlace_data(void) const;
    
    /**
     * @return the number of characters processed so far.
     */
    unsigned long int get_char_count(void) const;
    
    /**
     * @return the current receptor
     */
    png_receptor* get_receptor(void) const;
    
    /**
     * Set the receptor accessed by this processor
     * @param r the receptor to use
     */
    void set_receptor(png_receptor* r);
    
    /**
     * Generate an array of character/byte data.
     * @param[out] dta the generated PNG image data is placed here
     * @param[out] pos the position in the data array at which the
     *   last error was obtained
     * @return an error code, or zero on success
     */
    png_error get(png_buffer& dta, unsigned int *pos = 0L);

    /**
     * Generate an array of character/byte data.
     * @param[out] dta the generated PNG image data is placed here
     * @param maxsiz the maximum number of bytes to output to
     *   the given array
     * @param[out] pos the position in the data array at which the
     *   last error was obtained
     * @return an error code, or zero on success
     */
    png_error get
      (unsigned char* dta, unsigned int maxsiz, unsigned int *pos = 0L);
  
    
  protected:
    /** for use by subclasses */
    int readmode;
    /** for use by subclasses */
    int submode;
    /** for use by subclasses */
    int sidemode;
    /** for use by subclasses */
    unsigned long int xpect_nothing, xlong, xpect_length, xpect_chunks;
    /** for use by subclasses: shows current character count */
    unsigned long int xcharc;
    
    unsigned long int xdiv_width, xdiv_height,
      xdiv_xpos, xdiv_ypos, xdiv_index;
      
    unsigned short int filter_typ;
    unsigned short int filter_dist;
    unsigned char filter_log[8];
    png_buffer filter_backlog;

    virtual void reset_sub(void);
    void put_previous(unsigned char ch);
    void set_error(png_error v);

    /**
     * @return an error code, or zero on success
     */
    virtual png_error put_char(unsigned char);
    /**
     * @param[out] y the output byte
     * @return an error code, or zero on success
     */
    virtual png_error get_byte(unsigned char& y);
  };
};

#endif /*CMPS3120_LABS_PNG_H*/
