Simple PNG decoder for Computer Graphics class.

Language:
 - C++

Installation:
 - Just copy the 8 files into your project.

Preparation:
 - Subclass the `cmps3120::png_receptor`.
 - Override the `set_header` method. That's how you will
   get notified of the width and height of the image.
 - Override the `set_pixel` method. That's how you will
   get the pixel values for the image.
 
Usage:
 - Create an instance of `cmps3120::png_decoder`.
 - Set the receptor for the decoder by calling the `set_receptor` method.
 - Call `read_file` for easy PNG file reading, or use one of the
   `put` methods to send chunks of data directly.

Sample code:
```cpp
   using namespace cmps3120;

   // Your image/texture class header:
   class image : public png_receptor
   {
     ...
   public:
     void set_header
        (png_header h);
     void set_pixel
        (unsigned int x, unsigned int y, unsigned int level, png_pixel v);
   }

   // Your image/texture class implementation
   void image::set_header(png_header h)
   {
     //set your width and height fields
     m_width = h.width;
     m_height = h.height;

     //Setup the space for the image.
     //You would replace this line with code that
     //  creates an array or initializes a std::vector,
     //  for example.
     allocate_space(m_width,m_height);
   }
   void image::set_pixel
     (unsigned int x, unsigned int y, unsigned int level, png_pixel v)
   {
     //Replace these lines with code that takes the pixel values
     //  and stores them in the array created in set_header.
     //
     //Remember that the color values from v are 16-bit, i.e. range
     //  0 - 65535.
     unsigned int position = coordinates_to_index(x,y);
     m_array[position] = v.r;
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
```

   