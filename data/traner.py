from PIL import Image, ImageFont, ImageDraw
from string import ascii_letters
import numpy as np

def range_alpha(start_letter, end_letter):
    return ascii_letters[ascii_letters.index(start_letter): ascii_letters.index(end_letter) + 1]

def char_to_pixels(text, fontsize=14):
    font = ImageFont.truetype('arialbd.ttf', fontsize) 
    image = Image.new('L', (10, 12), 1)  
    draw = ImageDraw.Draw(image)
    draw.text((0, 0), text, font=font) 
    arr = np.asarray(image)
    arr = np.where(arr, 0, 1)
    arr = arr[(arr != 0).any(axis=1)]
    return arr

def MakeBin(arr):
    result = np.where(arr, '1', '0')
    return '\n'.join([' '.join(row) for row in result])

def SetUpLetter(filename, start, end, size, num, matrixsize):
    f = open(filename, "w")
    f.write(f"{ num }\n")
    f.write(f"{ matrixsize }\n")

    for c in range_alpha(start, end):
        arr = char_to_pixels(c, size)
        f.write(c)
        f.write('\n')
        f.write(MakeBin(arr))
        f.write('\n')
    f.close()

def SetUpDigit(filename, start, end, size, num, matrixsize):
    f = open(filename, "w")
    f.write(f"{ num }\n")
    f.write(f"{ matrixsize }\n")

    for c in range(start, end):
        arr = char_to_pixels(str(c), size)
        f.write(str(c))
        f.write('\n')
        f.write(MakeBin(arr))
        f.write('\n')
    f.close()
    

SetUpLetter("letters.txt", 'A', 'E', 13, 4, 100)
SetUpDigit("digits.txt", 0, 2, 13, 2, 100)
