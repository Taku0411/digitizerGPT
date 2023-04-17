# digitizerTools 
- This repository provides some kinds of tools for book digitizing.
- The quality of scanned document leads to the good digitizing.
- This tools assume that
  - Book is scaned with 2-page spread and gray color.
  - The doeument area is all the same on all pages.
  - Scaned data is skew, shadow coming from binding margin.

# dependencies
- cmake(version 3.20.0 or higher)
- OpenCV

# How to digitize
1. After scan, create file set of "%03d.png" file. If you have PDF file format, extract png file by Acrobat or other tools.
2. Use *split* tool to separate 2-page spread into 1 page png file. 
  ~~~
  ./execute <path of png files>
  ~~~
  For file specification, wildcard * is available. Note that split file(s) are exported in current directory, please take care overwriting.

3. To obtaine good skew modification, remove shadow around the edges.
  ~~~ 
  ./execute <path of png files>
  ~~~
  However, this tool does not provies auto shadow detection, therefore you have to change parameters of white fill region in main.cpp or by hand using like photoshop or gimp.

4. Rotate picture by line detection.
  ~~~
  ./execute <path of png files>
  ~~~
  This tool also requires parameters adjustment for good line detection. 

5. Use affin transform to locate the pages at the same position.
  
6. Bind separated 2 pages, compress by jpeg format.

7. OCR, compress, index (please use acrobat)

8. finish


