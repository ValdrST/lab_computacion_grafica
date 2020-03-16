from math import cos, sin, radians
from pylab import *
import matplotlib.pyplot as plt
import matplotlib.image as mpimg

def pentagon_coords():
    centre = (0,0)  
    num_v = 5
    grados = 360 / num_v
    grado_inic = 0.0
    xv = []
    yv = []
    for i in range(num_v):
        x = cos(radians(grado_inic))
        xv.append(x)
        y = sin(radians(grado_inic))
        yv.append(y)
        x2 = cos(radians(grado_inic + 36))
        xv.append(x2)
        y2 = sin(radians(grado_inic + 36))
        yv.append(y2)
        v = (round(x*.5,2), round(y*.5,2), round(x2*.5,2), round(y2*.5,2))
        print("{0}f, {1}f, 0.0f\n{2}f, {3}f, 0.0f".format(v[0], v[1], v[2], v[3]))
        grado_inic += grados
    return xv, yv
    

# Funcion para obtener las coordenadas u y v por click de mouse
def onclick(event):
    print(event)
    print("{0}f, {1}f,".format(round(event.xdata/512,4), round(((512-event.ydata)/512),4)))

if __name__ == '__main__': 
    fig = plt.figure()
    cid = fig.canvas.mpl_connect('button_press_event', onclick)
    image = mpimg.imread('lab_computacion_grafica/practica_6/practica_6_alt/Textures/dado_10.tga')
    plt.imshow(image)
    plt.show()
