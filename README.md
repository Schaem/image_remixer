# image_remixer

## What is this good for?

We all love our classical artworks, the mastery that goes into it is mesmerizing!

But I also love to see different takes and spins of certain works of art, so here is a software solution that at least satisfy my need to discover new things :)


## Requirements:
- The .exe file is intendend for Windows
- have an input image (for now only jpgs are usable)
- save an .txt file with the following tutorial:

	Get on https://coolors.co/palettes/trending and then you can choose a palette which suites your needs.
	You can left-click on the three dots on the palette and then 'export palette' should pop up.
	Then you should click the 'Code' option, after that you can see a 'copy' button on the bottom right.
	Do that and save the content in a plain text file (txt) and put the textfile in the directory where you execute your programm. 
  
 ## Usage 
With cmd/PowerShell you simply use the following code

`image_remixer.exe input_path_pic.jpg output_path_pic.jpg path_colorpalette.txt`

## Example
This is where the files can be stored:

![grafik](https://user-images.githubusercontent.com/104129468/164418601-b69f456c-2dd3-428d-8eab-2fbf2f1035cd.png)


Then you need to do the following:

- open up the cmd 
- change the directory to the desired path via cd [optional]
- use the .exe accordingly

![grafik](https://user-images.githubusercontent.com/104129468/164419288-c242a34c-7637-4468-9649-60e399e463f8.png)

## Potential content updates
- [X] improve cli, add a `--help` to inform users
- [X] improve error handling
- [ ] support for pngs
- [ ] support for Linux
- [ ] support for batch remixing
- [ ] support for more color inputs

## Hope you have fun with it! :)

![god_reaches_to_adam](https://user-images.githubusercontent.com/104129468/164423677-c98edfb4-f0b9-4733-8938-77a00ef3f962.png)

