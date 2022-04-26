//https://solarianprogrammer.com/2019/06/10/c-programming-reading-writing-images-stb_image-libraries/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

/*
	<READ ME>
	The main idea behind this programm is to use the brightness of the pixels to sort them and create a sort of histogramm, where
	every "brightness-class" gets a new color and thus a new image is made, with another color palette!
	
	<HOW TO USE>
	<PALETTE>
	Get on https://coolors.co/palettes/trending and then you can choose a palette which suites your needs.
	You can left-click on the three dots on the palette and then 'export palette' should pop up.
	Then you should click the 'Code' option, after that you can see a 'copy' button on the bottom right.
	Do that and save the content in a plain text file (txt) and put the textfile in the directory where you execute your programm.
	
	<IMAGE>
	currently it is only viable for 3-channel images like jpeg.
*/	
	


//This struct will be used to hold an array of 3-channel colors like in jpgs with an (R,G,B) structure
struct own_color{
	int color_channel[3];
};

/*
	This is how every pixel with a three color channel will get evaluated, feel free to use another formula,
	here is a stackoverflow post which can serve as an inspiration 
	
	https://stackoverflow.com/questions/596216/formula-to-determine-perceived-brightness-of-rgb-color
*/
int evaluate_brightness(unsigned char red_value, unsigned char green_value, unsigned char blue_value)
{
	//https://stackoverflow.com/questions/596216/formula-to-determine-brightness-of-rgb-color
	int brightness = (int) ((red_value+green_value+blue_value) / 3);
	return brightness;
}


/*
	We will use this function solely for qsort
*/
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

/*
	If memory serves me well this is just a more complicated calloc, should change it in the near future
*/
void create_pointer_to_memory(int **memory_ptr, int size){
	*memory_ptr = NULL;
	
	*memory_ptr = (int *)calloc(size,sizeof(int));
	if(*memory_ptr==NULL){
		printf("Unable to allocate memory\n");
        exit(1);
	}
}

/*
	This method goes over the image and saves the brightness-value from every pixel, creating an list/array to then sort it.
*/
void make_brightness_array(int *brightness_adress, unsigned char *img_source, int img_width, int img_height, int img_channels)
{
	size_t img_size = img_width * img_height * img_channels;
    
	int s=0;
    
	//Here we loop through the picture, until we have visited all the unique pixels in the image!
	for(unsigned char *p_source = img_source; p_source != img_source + img_size; p_source += img_channels) {
        brightness_adress[s]= evaluate_brightness(*p_source, *(p_source+1),*(p_source+2));
        s++;
    }
}



/*
	This method makes an array where you can read the brightness borders of a brightness class, will be used in the p_quartil_filter
*/
int make_class_border_array(int *sorted_input_array, int length_sorted_array, int *class_border_array, int amount_colors){
	class_border_array[0]=0;
	for(int i=0; i < amount_colors; i++){
		class_border_array[i+1] = sorted_input_array[(int)((i+1)*(length_sorted_array)/amount_colors)];
	}
}


/*
	Now here we will use all the methods, to go over the picture the amount of times of how many colors we have (which is inefficent, I must admit)
	And assign each brightness-class (indicatedt with start and end) a certain color
*/
void p_quartil_filter(unsigned char *img_source, int width, int height, int channels, unsigned char *img_destination, int *class_border_list, struct own_color *palette, int amount_colors)
{
	size_t img_size = width * height * channels;
		
	//Here we go over the array and write the data into the new image
	for(int i=0; i < amount_colors; i++)
	{
		for(unsigned char *p_source = img_source, *p_destination = img_destination; p_source != img_source + img_size; p_source += channels,
		p_destination += channels) {
			int brightness_pixel = evaluate_brightness(*p_source, *(p_source+1),*(p_source+2));
			if(class_border_list[i] <= brightness_pixel && brightness_pixel <= class_border_list[i+1]){
				*(p_destination)   = (unsigned char) palette[i].color_channel[0];
				*(p_destination+1) = (unsigned char) palette[i].color_channel[1];
				*(p_destination+2) = (unsigned char) palette[i].color_channel[2];
			} 
		}
	}
}


/*
	currently the main method has essentially two parts, first we want to read in the colors from the text
	and then the image processing starts with the methods
*/
int main(int argc, char **argv) {
	//This should help users that run into problems at the cli
	if(!(argc != 2 || argc != 4)){
		printf("the amount of argc %d", argc);
		printf("this is not the needed amount of parameters, try 'image_remixer.exe --help' or '-h'");
		exit(1);
	} else if(argc == 2 && ((strcmp(argv[1], "--help")==0)||(strcmp(argv[1], "-h")==0))){//
		printf("\nUsage:\n");
		printf("imager_remixer.exe input_image.jpg output_image.jpg color.txt\n\n");
		
		printf("General Options:\n");
		printf("  --help,      -h           list all avaiable parameters and usage of exe file   \n");
		printf("  --attention, -a           list the limitations and bypasses of the exe file    \n\n");
		exit(1);
	} else if(argc == 2 && ((strcmp(argv[1], "--attention")==0)||(strcmp(argv[1], "-a")==0))){
		printf("\nATTENTIION:\n\n");
		printf("Using paths you need to make sure everything exists regarding directories,\n");
		printf("because the program will not create directories for you                   \n\n");
		
		printf("Use the color-format I described in the read-me\n");
		printf("If you really want, you can structure it in the following way:\n\n");
		
		printf("/* CSV */\n");
		printf("hexcolor_1,hexcolor_2,hexcolor_3\n\n");
		
		printf("But each hexcolor should be presented without a hashtag\n");
		printf("Also don't put more than 20 colors, the code will not handle more\n\n");
		exit(1);
	} 
	
	if (strcmp(strrchr(argv[1], '\0') - 4, ".jpg")){
		printf("\nThe first parameter doesn't seem to be a .jpg file, try using the exe with the '--help' parameter only\n\n");
		exit(1);
	}
	
	if (strcmp(strrchr(argv[2], '\0') - 4, ".jpg")){
		printf("\nThe second parameter doesn't seem to be a .jpg file, try using the exe with the '--help' parameter only\n\n");
		exit(1);
	}
	
	if (strcmp(strrchr(argv[3], '\0') - 4, ".txt")){
		printf("\nThe last parameter doesn't seem to be a .txt file, try using the exe with the '--help' parameter only\n\n");
		exit(1);
	}
	
	//First the picture is loaded
	char * source = argv[1]; 
	char * destiny = argv[2];

	struct own_color palette[20];
	
	//this variable will be used to count the amount of colors
	int color_counter=0;
	
	//read_line will be used as a boolean that determines when it is time to extract the values of a text
	int read_line = 1;
	
	//This is used to open the txt file
	char *filename = argv[3];
	
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Error: could not open file %s", filename);
        //return 1;
		exit(1);
    }

    // reading line by line, max 256 bytes
    const unsigned MAX_LENGTH = 256;
    char buffer[MAX_LENGTH];
	//color_values is the string that will contain all the hex-values, seperated with commas.
	//e.g. 'ff0a11,bb4010,abcdef'
	char color_values[MAX_LENGTH];
	
    while (fgets(buffer, MAX_LENGTH, fp)){
        //printf("%s", buffer);
		if(read_line==0){
			strcpy(color_values, buffer);
			read_line = 1;
		}
		if(strcmp(buffer,"/* CSV */\n") == 0){
		read_line = 0;
		}
	}

    // close the file
    fclose(fp);

	char * token = strtok(color_values, ",");
	int num;
    // loop through the string to extract all other tokens
    while( token != NULL && color_counter < 20) {
      //printf( " %s\n", token ); //printing each token
	  num = (int)strtol(token, NULL, 16);
	  
	  palette[color_counter].color_channel[0]=((num >> 16) & 0xff);
	  palette[color_counter].color_channel[1]=((num >> 8) & 0xff);
	  palette[color_counter].color_channel[2]=((num) & 0xff);
	  
	  color_counter++;
      token = strtok(NULL, ",");
    }
	--color_counter;
	
	
    int width, height, channels;
    unsigned char *img_source = stbi_load(source, &width, &height, &channels, 0);
	
    if(img_source == NULL) {
        printf("Error in loading the image\n");
        exit(1);
	}
	
	printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);
	size_t img_source_size = width * height * channels;
	
	
	//Now Speicherplatz is reserved for the destiny picture 
	size_t img_destiny_size = width * height * channels;
    unsigned char *img_destiny = malloc(img_destiny_size);
    if(img_destiny == NULL) {
        printf("Unable to allocate memory for the destiny image.\n");
        exit(1);
    }
	
	
	//Now we create the pointers to use for the image
	int *brightness_list; 
	create_pointer_to_memory(&brightness_list,width*height);
	
	//Now the Speicherplatz for all the brightness pixels get reserved
	make_brightness_array(brightness_list, img_source, width, height, channels);
	
	
	//Now the array gets sorted
	qsort(brightness_list, width*height, sizeof(int), cmpfunc);
	
	//This prepares the memory of the class
	int *new_borders;
	create_pointer_to_memory(&new_borders, ++color_counter);
	make_class_border_array(brightness_list, width*height-1, new_borders, color_counter);
	
	
	//Now the pQuartilFilter get's into action and assigns every pixel a certain color based on their brightness
	p_quartil_filter(img_source, width, height, channels, img_destiny, new_borders, palette, color_counter);
	
	
	//Now we save the endresult	
	stbi_write_jpg(destiny, width, height, channels, img_destiny, 100);

	//Now all the memory will be free, goodbye
	free(brightness_list);
	free(new_borders);
	
	stbi_image_free(img_destiny);
	stbi_image_free(img_source);
	
	printf("Saved\n");
	return 0;
 }
 
