
#if !(defined CMPS3120_LABS_ZSS_H)
#define CMPS3120_LABS_ZSS_H


namespace cmps3120
{
  /**
   * zip stream error codes
   */
  enum zss_error_t
  {
    ZSS_OK = 0,
    
    /** end of zip stream */
    ZSS_DONE = 1,
    
    /** operation undefined */
    ZSS_UNDEFINED = 2,
    
    /** bad zip stream header */
    ZSS_BAD_HEADER = 3,
    
    /** request for dictionary */
    ZSS_DICTIONARY = 4,
    
    /** memory issues */
    ZSS_MEMORY = 5,
    
    /** Huffman codes acquired an error */
    ZSS_CODE_ERROR = 6,
    
    /** block construction error */
    ZSS_BLOCK_ERROR = 7,
    
    /** data was poorly encoded */
    ZSS_DATA_ERROR = 8,
    
    /** bad terminating checksum */
    ZSS_CHECKSUM = 9,
    
    /** overflow; pull some pending data and try again */
    ZSS_OVERFLOW = 10,
    
    /** data was poorly encoded */
    ZSS_DATA_FETCH = 11,
    /** data was poorly encoded */
    ZSS_DATA_BACK = 12,
    /** dynamic code bank read error */
    ZSS_EX_DATA_ERROR = 13,

    /** the stream was terminated too early */
    ZSS_MISSING_CHECKSUM = 14,
    
    ZSS_LAST = 15
  };
  typedef int zss_error;

  struct zss_huff_entry;

  /**
   * Huffman code bits with length
   */
  struct zss_huff_pair
  {
    unsigned long int bits;
    unsigned short len;
  };
  /**
   * Huffman code bank.
   */
  class zss_huffs
  {
  private:
    unsigned int m_count;
    struct zss_huff_entry *m_entries;
    unsigned int m_alloc;
    unsigned char m_sort;
  
  public:
    zss_huffs(void);
    ~zss_huffs(void);
    zss_huffs(const zss_huffs& );
    zss_huffs& operator=(const zss_huffs& );
    
    unsigned int get_count(void) const;
    bool resize(unsigned int nc);
    
    /**
     * @param xc the code to add to the code bank
     * @param xlen the length of the code in bits
     * @param val the value that the code represents
     * @return a success flag
     */
    bool append_code
      (unsigned int xlen, unsigned int val);
    
    /**
     * Sort codes and make trees.
     */
    bool sort_and_make(void);
    
    /**
     * @return a code bank for a direct data block
     */
    static zss_huffs for_direct(void);
    /**
     * @return a code bank for a fixed-code data bank
     */
    static zss_huffs for_fixed(void);
    
    /**
     * @return a code bank for a fixed-code distance bank
     */
    static zss_huffs for_distance(void);
    
    /**
     * @param bits the bits of the code to look up
     * @param len the length of the code to look up
     * @return the value connected to the given bit sequence,
     *   or -1 if the bit sequence could not be matched
     */
    int get_value(unsigned long int bits, unsigned int len) const;

    /**
     * @param value the value to look up
     * @return the code bits for a given value; if the value
     *   was not found, the code length will be set to zero
     */
    zss_huff_pair get_bits(unsigned int value) const;

    /**
     * Sort characters by values.
     */    
    void sort_by_value(void);
    
    void print_err(void) const;

  protected:
    /**
     * Sort characters by code length.
     */    
    void sort_by_length(void);
    
    /**
     * Sort characters by bits, shorter bit codes first.
     */    
    void sort_by_bits(void);
    
    /**
     * Make codes by code length.
     */    
    bool make_codes(void);
    
    /**
     * Comparison function
     */
    static int cmp_entry_v(const void* a, const void* b);

    /**
     * Comparison function
     */
    static int cmp_entry_w(const void* a, const void* b);
  };
  
  /**
   * Adler32 checksum
   */
  class zss_checksum
  {
  private:
    unsigned int xva, xvb;

  public:
    zss_checksum(unsigned long int l = 1UL);
    zss_checksum& add(unsigned char ch);
    operator unsigned long int(void) const;
  };
  
  /**
   * Zip-stream header
   */
  struct zss_header
  {
  public:
    /**
     * Constructor for a default zip-stream header
     */
    zss_header(void);
    /**
     * Constructor for a header from a packed integer
     */
    zss_header(unsigned int );
    /**
     * @return the zip-stream header paked into a single integer
     */
    operator unsigned int (void) const;
    /**
     * @return whether the header is valid
     */
    bool is_valid(void) const;
    /**
     * Computes and sets the check value for the ehader
     */
    void set_check(void);
    
    /**
     * Check value, such that when the header is packed into
     * an integer, the integer is a multiple of 31.
     */
    unsigned int fcheck : 5;
    /**
     * A flag, whether the described zip-stream uses a dictionary
     */
    unsigned int fdict : 1;
    /**
     * Compression level.
     */
    unsigned int flevel : 2;

    /**
     * Compression method. 8 = deflate
     */
    unsigned int xmethod : 4;
    /**
     * Compression information. The meaning depends on the
     * compression method.
     */
    unsigned int xinfo : 4;
  };

  /**
   * Data used by the zip stream decoder.
   */
  class zss_buffer
  {
  private:
    unsigned char* m_dta;
    unsigned int m_siz;
  
  public:
    /**
     * Constructor
     */
    zss_buffer(void);
    /**
     * Copy constructor
     */
    zss_buffer(const zss_buffer& );
    /**
     * Assignment
     */
    zss_buffer& operator=(const zss_buffer& );
    /**
     * Destructor
     */
    ~zss_buffer(void);
    
    /**
     * @return the data stored in the buffer
     */
    unsigned char* data(void);
    /**
     * @return the data stored in the buffer
     */
    const unsigned char* data(void) const;
    
    /**
     * @return the size of the buffer
     */
    unsigned int size(void) const;
    
    /**
     * Resize the buffer.
     * @param s the desired size of the buffer
     * @return whether the resize was successful
     */
    bool resize(unsigned int s);

    /**
     * Array indexing operator
     * @return the byte at the given index
     */
    unsigned char& operator[](unsigned int );
    /**
     * Array indexing operator
     * @return the byte at the given index
     */
    const unsigned char& operator[](unsigned int ) const;
    
    /**
     * Add a byte to the end of the buffer.
     * @param C the value of the byte to add to the buffer
     * @return whether the append operation was successful
     */
    bool append(int C);
  };
  
  /**
   * Single-use zip stream transcoder.
   * @todo zip stream encoder
   */
  class zss
  {
  private:
    int xerr;
    zss_buffer hist;
    zss_buffer pend;
    zss_huffs codes;
    zss_huffs dists;
    zss_huffs xcodes;
    unsigned int hist_pos;
    unsigned int hist_len;
    unsigned int pend_pos;
    zss_header xhdr;
    zss_checksum xadler;
    bool dict_set_tf;
    unsigned long dict_adler;
    
  public:
    /** Constructor */
    zss(void);
    /** Destructor */
    virtual ~zss(void);
    
    /**
     * Reset the zip-stream transcoder.
     */
    virtual void reset(void);
    
    /**
     * @return any error code posted on the transcoder
     */
    zss_error get_error(void) const;
    
    /**
     * Process an array of character/byte data.
     * @param dta the zip stream data to process
     * @param[out] pos the position in the data array at which the
     *   last error was obtained
     * @return an error code, or zero on success
     */
    zss_error put(const zss_buffer& dta, unsigned int *pos = 0L);

    /**
     * Process an array of character/byte data.
     * @param dta the zip stream data to process
     * @param siz the number of bytes in the data
     * @param[out] pos the position in the data array at which the
     *   last error was obtained
     * @return an error code, or zero on success
     */
    zss_error put
      (const unsigned char* dta, unsigned int siz, unsigned int *pos = 0L);
  
    /**
     * @return whether a dictionary has been set
     */
    bool is_dict_set(void) const;
    
    /**
     * @return the current dictionary's checksum, if it has been set
     */
    unsigned long get_dict_checksum(void) const;
    
    /**
     * Set dictionary.
     * @return a success flag
     */
    bool set_dictionary(const zss_buffer& dct);
    
    /**
     * @return the header
     */
    const zss_header& get_header(void) const;
    
    /**
     * Set the zip stream header.
     * @param h the new header
     */
    void set_header(const zss_header& h);
    
    /**
     * @return the active code bank
     */
    zss_huffs& get_codes(void);
  
    /**
     * @return the active code bank
     */
    const zss_huffs& get_codes(void) const;
  
    /**
     * Set the active code bank.
     * @param h the code bank to set
     */
    void set_codes(const zss_huffs& h);

    /**
     * @return the active distance code bank
     */
    zss_huffs& get_distances(void);
  
    /**
     * @return the active distance code bank
     */
    const zss_huffs& get_distances(void) const;
  
    /**
     * Set the active distance code bank.
     * @param h the distance code bank to set
     */
    void set_distances(const zss_huffs& h);
    
    /**
     * @return the active code bank
     */
    zss_huffs& get_extended_codes(void);
  
    /**
     * @return the active code bank
     */
    const zss_huffs& get_extended_codes(void) const;
  
    /**
     * Set the active code bank.
     * @param h the code bank to set
     */
    void set_extended_codes(const zss_huffs& h);

    /**
     * Append to the output buffer, without affecting the
     *   history.
     * @param ch the character to append
     * @return a success flag
     * @note does not call @link #put_previous@endlink
     */
    bool append_no_history(unsigned char ch);
    
    /**
     * Append to the output buffer.
     * @param ch the character to append
     * @return a success flag
     * @note also calls @link #put_previous@endlink
     */
    bool append(unsigned char ch);
    
    /**
     * @return the number of bytes pending
     */
    unsigned int get_pending_count(void) const;
    
    /**
     * @return the buffer of pending bytes
     */
    zss_buffer& get_pending(void);
    /**
     * @return the buffer of pending bytes
     */
    const zss_buffer& get_pending(void) const;

    /**
     * Drop all pending bytes.
     */  
    void clear_pending(void);
    
    /**
     * Get the stream checksum.
     */
    unsigned long int get_checksum(void) const;
    
    /**
     * Get the length of the history.
     * @return the length of the history
     */
    unsigned int get_history_length(void) const;
    
    /**
     * @return the number of characters processed so far.
     */
    unsigned long int get_char_count(void) const;

    /**
     * Notify of end of stream.
     * @return a \link zss_error_t\endlink value
     */
    zss_error finish(void);
    
  protected:
    /** for use by subclasses */
    int readmode;
    /** for use by subclasses */
    int submode;
    /** for use by subclasses */
    unsigned long int xpect_dict_adler, xlong;
    /** for use by subclasses: shows current character count */
    unsigned long int xcharc;

    /**
     * Reset the base transcoder values.
     */
    void reset_sub(void);
    /**
     * Get a previous character in the stream.
     * @param pos the number of steps to retreat
     * @return the character, or zero if the character could 
     *   not be obtained
     */
    unsigned char get_previous(unsigned int pos) const;
    /**
     * Write a character into the history.
     * @param ch the character to add
     * @note the history is limited by the window size
     * @see @link #setup_window @endlink
     */
    void put_previous(unsigned char ch);
    /**
     * Directly set the error code.
     * @param v the error code to post on the transcoder
     * @note Normally, this function need not be called by subclasses.
     * Return values from \link put_char \endlink and \link put_eof
     * \endlink are automatically posted on the transcoder.
     */
    void set_error(zss_error v);
    /**
     * Setup the history window based on the current header.
     * @return whether the window setup was successful
     */
    bool setup_window(void);

    /**
     * @return an error code, or zero on success
     */
    virtual zss_error put_eof(void);
    /**
     * @return an error code, or zero on success
     */
    virtual zss_error put_char(unsigned char);
  };
  
}

#endif /*CMPS3120_LABS_ZSS_H*/
