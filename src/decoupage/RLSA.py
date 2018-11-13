def rlsa ( tabBin , seuil ):
    i = 0;
    taille = len(tabBin);
    mem = 0; 
    while i < taille :
        
        if ( tabBin[i] == 0 ):
            #we see how many 0 there is
            while ( tabBin[i] == 0 ):
                mem += 1;
                i +=1;
                if ( i >= taille ): #if the tab finish with 0
                    break
    
            if ( mem <= seuil ):
                #we can modify
                i = i - mem;
                for j in range ( i , i + mem ):
                    tabBin[j] = 1;
                i = j;

        i += 1;
        mem = 0;
    return tabBin;

def printtab (tab):
    for x in range (0 , len(tab)):
        print( " " , tab[x]);



tableau = [0,0,0,1,0,0,0,0,0,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,0];
#
printtab( rlsa( tableau,4))
                    
