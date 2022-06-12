from PIL import Image  
import os

desliced =  Image.new("RGB", (1000, 1000), "white")

directory = 'pieces'
 

for filename in os.listdir(directory):
    f = os.path.join(directory, filename)

    temp = f.split(".")
    x = (temp[0])[-3:]
    y = (temp[1])[-3:]

    img = Image.open(f)

    Image.Image.paste(desliced, img, (int(x), int(y)))

output = desliced.save("original.jpg")
