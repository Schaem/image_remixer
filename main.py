import sys
import os
import filter_application as fa

def main():
    """
    Main function to handle command-line arguments and execute appropriate functions.
    """
    if sys.argv[1] == "--help":
        print("Usage for single processing meaning one picture, one palette file:")
        print("python main.py input.jpg output.jpg palette.txt")
        print("")
        print("Usage for color-batch processing meaning using one picture, multiple palettes:")
        print("python main.py --color-batch input.jpg output_prefix palettes_folder")
        print("")
        print("Usage for image-batch processing meaning multiple images, one palette:")
        print("python main.py --image-batch images_folder output_folder palette.txt")
        print("")
        print("Usage for combine-batch processing meaning multiple images, multiple palettes:")
        print("python main.py --combine-batch images_folder output_folder palettes_folder")
        sys.exit(1)

    if len(sys.argv) == 4:
        # Single palette processing
        source = sys.argv[1]
        destiny = sys.argv[2]
        palette_file = sys.argv[3]

        if not source.endswith('.jpg') or not destiny.endswith('.jpg') or not palette_file.endswith('.txt'):
            print("Check file extensions (.jpg for images, .txt for palette)")
            sys.exit(1)

        palette = fa.extract_scss_rgb_colors(palette_file)
        if len(palette) > 20:
            print("Too many colors in palette (max 20)")
            sys.exit(1)

        fa.apply_palette_by_brightness(source, palette, destiny)

    elif len(sys.argv) == 5 and sys.argv[1] == '--color-batch':
        # Batch palette processing
        source = sys.argv[2]
        output_prefix = sys.argv[3]
        palettes_folder = sys.argv[4]
        if not source.endswith('.jpg') or not os.path.isdir(palettes_folder):
            print("Check file extensions (.jpg for images) and if palettes_folder is a valid directory")
            sys.exit(1)
        fa.batch_apply_palette_by_brightness(source, palettes_folder, output_prefix)

    elif len(sys.argv) == 5 and sys.argv[1] == '--image-batch':
        # Batch image processing
        images_folder = sys.argv[2]
        output_folder = sys.argv[3]
        palette_file = sys.argv[4]
        if not os.path.isdir(images_folder) or not os.path.isdir(output_folder) or not palette_file.endswith('.txt'):
            print("Check file extensions (.txt for text) and if folders are valid directories")
            sys.exit(1)
        fa.batch_transform_images_with_palette(images_folder, palette_file, output_folder)

    elif len(sys.argv) == 5 and sys.argv[1] == '--combine-batch':
       # Batch combine processing
        images_folder = sys.argv[2]
        output_folder = sys.argv[3]
        palette_folder = sys.argv[4]
        if not os.path.isdir(images_folder) or not os.path.isdir(output_folder) or not os.path.isdir(palette_folder):
           print("Check file extensions (.txt for text) and if folders are valid directories")
           sys.exit(1)
        fa.batch_transform_images_with_palettes(images_folder, palette_folder, output_folder)


    else:
        print("Invalid arguments. Use --help for usage information.")
        sys.exit(1)
    print("finished!")

if __name__ == "__main__":
    main()
