from PIL import Image, ImageDraw, ImageFont
import os

train_dir = "training"

alphabet= "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
fonts = ["PlayfairDisplay-Regular", "Roboto-Regular", "Slabo27px-Regular"]

for i in range(len(fonts)):
    for c in alphabet:

        img = Image.new("RGB", (28,28), (255,255,255))
        draw = ImageDraw.Draw(img)
        font = ImageFont.truetype(os.path.join("fonts", fonts[i]+".ttf"), size=30)
        draw.text((4,-6), c, font=font, fill=(0,0,0))
        #img.show()

        save_dir = os.path.join(train_dir, c)
        if not os.path.exists(save_dir):
            os.makedirs(save_dir)
        img.save(os.path.join(save_dir, '{:02}.bmp'.format(i)))