import numpy as np
import time

def sigmoid(x,deriv=False):
    if(deriv==True):
        return x*(1-x)

    return 1/(1+np.exp(-x))
    
X = np.array([[0,0],
            [0,1],
            [1,0],
            [1,1]])
                
y = np.array([[0],
            [1],
            [1],
            [0]])

#np.random.seed(1)

# les poids (weights) de notre reseau de neuronnes (2 layer)
# ils sont initialises aleatoirement et seront corriges par la suite
syn0 = 2*np.random.random((2,4)) - 1
syn1 = 2*np.random.random((4,1)) - 1

# phase d'entrainement (correction des poids)
for j in range(1000):

    # xor(X) = sigmoid(sigmoid(X * l1) * l2) on parcous le RDN par l'avant (forward)
    l0 = X
    l1 = sigmoid(np.dot(l0,syn0))
    l2 = sigmoid(np.dot(l1,syn1))

    # difference entre le resultat attendu et le resultat final
    l2_error = y - l2
    
    if (j% 100) == 0:
        print ("Cost:" + str(np.mean(np.abs(l2_error))))
        # le cout (cost) mesure la performance de notre RDN
        # il doit etre le plus petit possible
        
    # on corrige les poids du reseau en partant de la fin (back propagation)
    l2_delta = l2_error*sigmoid(l2,deriv=True)
    l1_error = l2_delta.dot(syn1.T)
    l1_delta = l1_error * sigmoid(l1,deriv=True)

    syn1 += l1.T.dot(l2_delta)
    syn0 += l0.T.dot(l1_delta)


print ("Output After Training:")
print ("xor(0, 0) =", sigmoid(np.dot(sigmoid(np.dot([[0, 0]],syn0)),syn1))[0][0])
print ("xor(1, 0) =", sigmoid(np.dot(sigmoid(np.dot([[1, 0]],syn0)),syn1))[0][0])
print ("xor(0, 1) =", sigmoid(np.dot(sigmoid(np.dot([[0, 1]],syn0)),syn1))[0][0])
print ("xor(1, 1) =", sigmoid(np.dot(sigmoid(np.dot([[1, 1]],syn0)),syn1))[0][0])
