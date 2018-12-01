from PIL import Image, ImageDraw, ImageFont
import os

train_dir = "Banque Image/training"
fonts_dir = "Banque Image/fonts"

alphabet= "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
#fonts = ["PlayfairDisplay-Regular", "Roboto-Regular", "Slabo27px-Regular"]

#for i in range(len(fonts)):
print("[+] Generating training set in '{}'".format(train_dir))
i = 0
for font in os.listdir(fonts_dir):
    if font[0] != '.':
        #print(font)
        for c in alphabet:

            img = Image.new("RGB", (28,28), (255,255,255))
            draw = ImageDraw.Draw(img)
            loaded_font = ImageFont.truetype(os.path.join(fonts_dir, font), size=20)
            draw.text((4,0), c, font=loaded_font, fill=(0,0,0))
            #img.show()

            save_dir = os.path.join(train_dir, c)
            if not os.path.exists(save_dir):
                os.makedirs(save_dir)
            img.save(os.path.join(save_dir, '{:02}.bmp'.format(i)))
        i+=1


print("[+] Used {} different fonts".format(i))
