#Python 3.X
from PIL import Image

#constantes
img = Image.open("photo.jpg").convert('RGB')
global lrg, lng, pix, white, black
lrg = img.size[0]; lng = img.size[1]
pix=img.load()
white = (255,255,255); black = (0,0,0)

def grayscale(img):
    for i in range (0,lrg):
        for j in range (0,lng):
            (red,green,blue) = pix[i,j]
            gray = (red + green + blue) // 3
            pix[i,j] = (gray, gray, gray)


def binarize(img):
    for i in range (0,lrg):
        for j in range (0,lng):
            (gray, gray1, gray2) = pix[i,j]
            if(gray > 128):
                pix[i,j]= white
            else:
                pix[i,j]=black


def rmblackdot(img):
    for i in range (0,lrg):
        for j in range (0,lng):
            amout_color = neighbor(i, j)
            if(amout_color == True):
                pix[i,j]= white
            if(amout_color == False):
                pix[i,j]=black


def neighbor(i, j): #ajoute les voisin du pixel se trouvant en i j
    tab = []
    tab.append(pix[(i+1)%lrg,j]);tab.append(pix[(i-1)%lrg,j]);
    tab.append(pix[i,(j+1)%lng]);tab.append(pix[i,(j-1)%lng]);
    tab.append(pix[(i+1)%lrg,(j+1)%lng]);tab.append(pix[(i-1)%lrg,(j-1)%lng]);
    tab.append(pix[(i+1)%lrg,(j-1)%lng]);tab.append(pix[(i-1)%lrg,(j+1)%lng]);

    return iswhite(tab)

def iswhite(tab): #si 5 ou plus des pixel sont blanc, on renvoie Vraie
    long = len(tab) #on renvoie faux pour plus de 5 pixel noir
    cptw = 0; cptb = 0
    for i in range (long):
        if(tab[i] == white):
            cptw += 1
        if(tab[i] == black):
            cptb += 1

    if(cptw > 4):
        return True
    if(cptb > 4):
        return False
    else: #si les valeur sont equitables, on ne changera rien
        return -1





grayscale(img)
binarize(img)
img.save("photobinarized.jpg")

rmblackdot(img)
img.save("photomodif.jpg")

print ("Done")

