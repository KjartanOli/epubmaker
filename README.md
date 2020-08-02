# Epubmaker

Epubmaker is a command line tool to generate a fully functional epub document from a list of xhtml files

## Requirements
* C++ complier with support for C++17 (I use gcc-10 and -std=c++20 but C++17 should have everything required)
* [libzippp](https://github.com/ctabin/libzippp)

## Getting started

```
git clone https://github.com/KjartanOli/epubmaker.git
cd epubmaker
make
make install
```

## Usage
Epubmaker supports a variety of options, most of which relate to setting the metadata of the produced epub file. The full list of options can be seen with:
	epubmaker --help

The most basic use of Epubmaker is:
	epubmaker DIRNAME

Assuming that the directory DIRNAME contains a file named order.txt this will produce a file named book.epub in DIRNAME. This file will not be compliant with the epub standard, because Author, Title, and Language were not set.

## The order.txt file
The order.txt file is Epubmaker's way of knowing how to order the chapters of your epub. It should contain a list of relative paths to the xhtml files which serve as chapters, one on each line. All of the files present in the order.txt file must exist and have either a .html or .xhtml extension, if either of these conditions is not met, Epubmaker will terminate immediately with no output.

## Things to note
* By default Epubmaker will search for stylesheets in DIRNAME/Styles and images
  in DIRNAME/Images unless --style-dir and/or --img-dir are used to change those
  search paths. If either search directory does not exist Epubmaker will exit
  with an error unless --no-style and/or --no-images is passed.
* Epubmaker will not search subdirectories for stylesheets or images
* Epubmaker does not detect whether an image/stylesheet is used in a chapter or
  not. It will place all files in the image directory and all files with the
  .css extension in the style directory into the epub.
* Epubmaker can detect the title of chapters based on the contents of the
  \<title> tag of the file as long as it is in the format <title>TITLE</title>
  on a single line. If it does not detect this pattern in a file Epubmaker will
  default to using the filename as a title
* All chapters are placed in OEBPS/Text and other resources are placed in other subdirectories of OEBPS as appropriate for the type of the resource. Meaning img tags or link tags must start their href attribute with ../ to exit Text
* Even if an alternate directory is specified for either stylesheets or images,
  Epubmaker will still place the contents of those directories into OEBPS/Images
  and OEBPS/Styles meaning that if img tags or link tags reference other
  directory names those links will lead to nothing.
* Due to an issue I don't understand with libzippp when adding data from memory
  into an archive version 1.1.2 writes the generated cover and table of
  contents to disk. If you wish to avoid this, use version 1.1.1, but do note
  that version 1.1.1 will require manual edditing of the generated files.

