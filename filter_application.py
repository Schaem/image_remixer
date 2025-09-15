import sys
import re
import os
import numpy as np
from PIL import Image


def extract_scss_rgb_colors(filename):
    """
    Extract RGB color values from a SCSS file between the markers
    /* SCSS RGB */ and /* SCSS Gradient */ if you use the text from coolors.co
    shouldn't be more than 20 colors per palette.

    Returns a list of RGB tuples. 
    """
    rgb_values_palette = []
    in_section = False
    with open(filename, 'r') as f:
        for line in f:
            if '/* SCSS RGB */' in line:
                in_section = True
                continue
            if '/* SCSS Gradient */' in line and in_section:
                break
            if in_section:
                line = line.strip()
                match = re.search(r'rgba\((\d+),\s*(\d+),\s*(\d+)', line)
                if match:
                    rgb = tuple(map(int, match.groups()))
                    rgb_values_palette.append(rgb)
    amount_colors = len(rgb_values_palette)

    if amount_colors > 20:
        print(f"Too many colors in {filename} (max 20)")
        sys.exit(1)
    return rgb_values_palette

def apply_palette_by_brightness(image_path, palette, output_path):
    """
    image_path: path to the input image, it should end with .jpg
    palette: list of RGB tuples, e.g. [(255,0,0), (0,255,0), (0,0,255)]
    output_path: path to save the output image, it should end with .jpg

    returns nothing, saves the image to output_path
    """
    img = Image.open(image_path).convert('RGB')
    img_array = np.array(img)
    height, width, channels = img_array.shape

    # Calculate brightness for each pixel
    brightness = np.mean(img_array, axis=2).flatten()
    sorted_brightness = np.sort(brightness)
    amount_colors = len(palette)

    # Calculate class borders
    class_borders = [0]
    for i in range(amount_colors):
        idx = min(int((i + 1) * len(sorted_brightness) / amount_colors) - 1, len(sorted_brightness) - 1)
        class_borders.append(sorted_brightness[idx])

    # Map each pixel to a palette color based on brightness class
    new_img_array = np.zeros_like(img_array)
    flat_img = img_array.reshape(-1, 3)
    for i in range(amount_colors):
        # we have here a boolean array called "mask" where TRUE means the pixel belongs to the current brightness class
        mask = (brightness >= class_borders[i]) & (brightness <= class_borders[i+1])
        # we apply the palette color to all pixels that belong to the current brightness class
        new_img_array.reshape(-1, 3)[mask] = palette[i]

    # Save the new image
    new_img = Image.fromarray(new_img_array.astype('uint8'))
    new_img.save(output_path, quality=100)
    print(f"Saved transformed image to {output_path}")

def batch_apply_palette_by_brightness(image_path, palettes_folder, output_prefix):
    """
    here we batch transform one image with all palettes in a folder

    image_path: path to the input image
    palettes_folder: path to the folder containing palette files
    output_prefix: prefix for output files, e.g. 'output_' will produce output_0.jpg, output_1.jpg, etc.

    returns nothing, saves the images to output_path
    """
    palette_files = [f for f in os.listdir(palettes_folder) if f.endswith('.txt')]
    for idx, palette_file in enumerate(palette_files):
        palette_path = os.path.join(palettes_folder, palette_file)
        rgb_value_palette = extract_scss_rgb_colors(palette_path)
        output_path = f"{output_prefix}{idx}.jpg"
        apply_palette_by_brightness(image_path, rgb_value_palette, output_path)
        print(f"Processed palette {idx} -> {output_path}")

def batch_transform_images_with_palette(images_folder, palette_file, output_folder):
    """
    here we batch transform multiple image with one palette in a folder

    image_folder: folder for the input images
    palette_file: path to palette file
    output_folder: folder to save the output images

    returns nothing, saves the images to output_folder
    """
    palette = extract_scss_rgb_colors(palette_file)
    if len(palette) > 20:
        print("Too many colors in palette (max 20)")
        return

    # List all .jpg files in the images folder
    image_files = [f for f in os.listdir(images_folder) if f.lower().endswith('.jpg')]
    os.makedirs(output_folder, exist_ok=True)

    for image_file in image_files:
        input_path = os.path.join(images_folder, image_file)
        output_path = os.path.join(output_folder, f"transformed_{image_file}")
        apply_palette_by_brightness(input_path, palette, output_path)
        print(f"Processed {image_file} -> {output_path}")

def batch_transform_images_with_palettes(images_folder, palettes_folder, output_folder):
    """
    here we batch transform multiple images with multiple palettes in folders

    image_folder: folder for the input images
    palettes_folder: path to the folder containing palette files
    output_folder: folder to save the output images

    returns nothing, saves the images to output_folder
    """
    palette_files = [f for f in os.listdir(palettes_folder) if f.endswith('.txt')]
    # List all image files
    image_files = [f for f in os.listdir(images_folder) if f.lower().endswith('.jpg')]
    os.makedirs(output_folder, exist_ok=True)

    for palette_idx, palette_file in enumerate(palette_files):
        palette_path = os.path.join(palettes_folder, palette_file)
        palette = extract_scss_rgb_colors(palette_path)
        if len(palette) > 20:
            print(f"Too many colors in {palette_file} (max 20), skipping.")
            continue
        for image_file in image_files:
            input_path = os.path.join(images_folder, image_file)
            output_name = f"transformed_{palette_idx}_{image_file}"
            output_path = os.path.join(output_folder, output_name)
            apply_palette_by_brightness(input_path, palette, output_path)
            print(f"Processed {image_file} with {palette_file} -> {output_name}")