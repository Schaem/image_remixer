# image_remixer

## What is this good for?

We all love our classical artworks, the mastery that goes into it is mesmerizing!

But I also love to see different takes and spins of certain works of art, so here is a software solution that at least satisfy my need to discover new things now written with python :)


## Requirements:
- have an input image (for now only jpgs are usable)
- retrieve .txt files with the following tutorial:

	Get on https://coolors.co/palettes/trending and then you can choose a palette which suites your needs.
	You can left-click on the three dots on the palette and then 'export palette' should pop up.
	Then you should click the 'CSS' option, after that you can see a 'copy' button on the bottom right.
	Do that and save the content in a plain text file (txt) and put the textfile in the directory where you want execute your programm. 
  
 ## Usage 
With the terminal you simply use like the following code

`python main.py input_path_pic.jpg output_path_pic.jpg path_colorpalette.txt` but you can use `--help` for an extensive overview of what is possible

## Example
This is where the files can be stored:

<img width="854" height="108" alt="grafik" src="https://github.com/user-attachments/assets/fcb5e610-4dfa-41a1-89f8-2855c2de76e7" />

Then you need to do the following:

- open up the cmd 
- change the directory to the desired path via cd [optional]
- and use the commands

<img width="1290" height="170" alt="grafik" src="https://github.com/user-attachments/assets/9da053a5-1081-4f6c-8fd9-587054d4f51d" />


## Potential content updates
- [X] improve cli, add a `--help` to inform users
- [X] improve error handling
- [X] support for batch remixing
- [X] support for more color inputs
- [ ] support for pngs

## Hope you have fun with it! :)

![god_reaches_to_adam](https://user-images.githubusercontent.com/104129468/164423677-c98edfb4-f0b9-4733-8938-77a00ef3f962.png)

