
#if !(defined CMPS3120_LABS_ZENC_H)
#define CMPS3120_LABS_ZENC_H

#include "./zss.h"

namespace cmps3120
{
  class zenc_hash_line
  {
  private:
    unsigned int *m_dta;
    unsigned int m_length;

    zenc_hash_line(unsigned int *p, unsigned int s);
    friend class zenc_hash;
  public:
    /**
     * Destructor
     */
    ~zenc_hash_line(void);

    /**
     * @return the first value on the line
     */
    operator unsigned int&(void);
    /**
     * @return the first value on the line
     */
    operator const unsigned int&(void) const;

    /**
     * @param j the position in the hash
     * @return the value at the given position on the line
     */
    unsigned int& operator[](unsigned int j);
    /**
     * @param j the position in the hash
     * @return the value at the given position on the line
     */
    const unsigned int& operator[](unsigned int j) const;

    /**
     * @return the length of the line
     */
    unsigned int length(void) const;
  };
  class zenc_hash_const_line
  {
  private:
    const unsigned int *m_dta;
    unsigned int m_length;

    zenc_hash_const_line(const unsigned int *p, unsigned int s);
    friend class zenc_hash;
  public:
    /**
     * Destructor
     */
    ~zenc_hash_const_line(void);

    /**
     * @return the first value on the line
     */
    operator const unsigned int&(void) const;

    /**
     * @param j the position in the hash
     * @return the value at the given position on the line
     */
    const unsigned int& operator[](unsigned int j) const;

    /**
     * @return the length of the line
     */
    unsigned int length(void) const;
  };

  class zenc_hash
  {
  private:
    unsigned int m_size;
    unsigned int m_length;
    unsigned int *m_table;

  public:
    /**
     * Constructor
     */
    zenc_hash(void);
    /**
     * Desctructor
     */
    ~zenc_hash(void);

    /**
     * @return the size of the hash
     */
    unsigned int get_size(void) const;
    /**
     * @return the number of disambiguation entries
     *   at each hash value
     */
    unsigned int get_entry_length(void) const;
    /**
     * Resize the hash table.
     * @param sz the desired size of the hash
     * @param lh the number of disambiguation entries
     *   in the hash
     * @return whether the resize was successful
     */
    bool resize(unsigned int sz, unsigned int lh = 4);

    /**
     * Add an entry to the hash table.
     * @param hash_base the value to use as the basis
     *   for computing the hash
     * @param value the value to add to the hash
     */
    void put_entry(unsigned int hash_base, unsigned int value);

    /**
     * Get a hash table entry.
     * @param i the hash table value to use for indexing
     * @param j the position in the hash
     * @return the value at the given coordinate
     */
    unsigned int& at(unsigned int i, unsigned int j = 0);
    /**
     * Get a hash table entry.
     * @param i the hash table value to use for indexing
     * @param j the position in the hash
     * @return the value at the given coordinate
     */
    const unsigned int& at(unsigned int i, unsigned int j = 0) const;

    /**
     * Get a hash table entry.
     * @param i the hash table value to use for indexing
     * @param j the position in the hash
     * @return the value at the given coordinate
     */
    unsigned int& operator()(unsigned int i, unsigned int j);
    /**
     * Get a hash table entry.
     * @param i the hash table value to use for indexing
     * @param j the position in the hash
     * @return the value at the given coordinate
     */
    const unsigned int& operator()(unsigned int i, unsigned int j) const;

    /**
     * Get a hash table entry.
     * @param i the hash table value to use for indexing
     * @return a reference to the most recent value at the given index
     */
    zenc_hash_line operator[](unsigned int i);
    /**
     * Get a hash table entry.
     * @param i the hash table value to use for indexing
     * @return a reference to the most recent value at the given index
     */
    zenc_hash_const_line operator[](unsigned int i) const;
  };

  enum zenc_pair_stop_t
  {
    zenc_pair_stop
  };

  class zenc_pair
  {
  private:
    unsigned int nvalue;
    unsigned int dvalue;
    unsigned short n_ext;
    unsigned int d_ext;

  public:
    /**
     * Constructor
     * @param x a single literal character
     */
    zenc_pair(unsigned char x);
    /**
     * Constructor
     * @param l a byte length to be repeated
     * @param d the distance backward to look
     */
    zenc_pair(unsigned int len, unsigned int dist);

    /**
     * Constructor for a stop code
     */
    zenc_pair(enum zenc_pair_stop_t);

    /** @return the literal or length code */
    unsigned int literal(void) const;
    /** @return the literal or length code */
    unsigned int length(void) const;
    
    /** @return the length extension value */
    unsigned int length_ext(void) const;

    /** @return the distance or length code */
    unsigned int distance(void) const;
    
    /** @return the distance extension value */
    unsigned int distance_ext(void) const;
  };

  class zenc : public zss
  {
  public:
    zenc(void);
    ~zenc(void);

    void reset(void);

  private:
    unsigned int m_d_block_size;
    zss_buffer m_bytes_pend;
    bool m_last_block;
    bool m_has_blocks;
    unsigned long int m_shift_out;
    zss_header m_xhdr;
    int m_bithold, m_bitcount;

    unsigned int m_hashpos;
    zenc_hash m_hash;

    zss_error put_char(unsigned char x);
    zss_error put_eof(void);
    zss_error put_char_or_eof(int );
    int gen_bits(int x);
    int output_block(void);
    int push_bit(int B);
    int push_resync(void);
    void next_hash(unsigned char x);

    /**
     * @param curs the position in the bytes-pending buffer
     * @return the number of bytes skipped, or zero if the 
     *   a skip code is not vailable
     */
    unsigned int try_hash(unsigned int curs);
    
    /**
     * @param curs the current position in the input bytes pending buffer
     * @param pos the distance to go backward; 1 is the most recent
     * @return the byte at that position in hypothetical history,
     *  or zero if not available
     */
    unsigned char augmented_previous(unsigned int curs, unsigned int pos);

    /**
     * Push a code to the output stream.
     * @param ndv the length/distance pair to push to the output stream
     * @return a \link zss_error\endlink value
     */
    int augmented_push_bit(const zenc_pair& ndv);
  };
};

#endif /*CMPS3120_LABS_ZENC_H*/
