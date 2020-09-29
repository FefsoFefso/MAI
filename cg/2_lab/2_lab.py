from matplotlib import pyplot as plt 
from mpl_toolkits.mplot3d.art3d import Poly3DCollection, Line3DCollection 
import numpy as np 

figure = plt.figure(figsize=(8, 8))
figure.canvas.set_window_title('2 laba')

axis = figure.add_subplot(111, projection='3d')

v = np.array([[0, 0, 1], 
    [1.0, 0.0, -1], 
    [0.9135454576426009, 0.40673664307580015, -1], 
    [0.6691306063588582, 0.7431448254773941, -1], 
    [0.30901699437494745, 0.9510565162951535, -1], 
    [-0.10452846326765333, 0.9945218953682734, -1], 
    [-0.4999999999999998, 0.8660254037844387, -1], 
    [-0.8090169943749473, 0.5877852522924732, -1], 
    [-0.9781476007338057, 0.20791169081775931, -1], 
    [-0.9781476007338057, -0.20791169081775907, -1], 
    [-0.8090169943749476, -0.587785252292473, -1], 
    [-0.5000000000000004, -0.8660254037844384, -1], 
    [-0.10452846326765423, -0.9945218953682733, -1], 
    [0.30901699437494723, -0.9510565162951536, -1], 
    [0.6691306063588585, -0.743144825477394, -1], 
    [0.913545457642601, -0.40673664307580015, -1]])

axis.scatter3D(v[:, 0], v[:, 1], v[:, 2])

verts = [[v[0],v[1], v[2]], 
        [v[0], v[2], v[3]], 
        [v[0], v[3], v[4]], 
        [v[0], v[4], v[5]], 
        [v[0], v[5], v[6]], 
        [v[0], v[6], v[7]],
        [v[0], v[7], v[8]],
        [v[0], v[8], v[9]],
        [v[0], v[9], v[10]],
        [v[0], v[10], v[11]],
        [v[0], v[11], v[12]],
        [v[0], v[12], v[13]],
        [v[0], v[13], v[14]],
        [v[0], v[14], v[15]],
        [v[0], v[15], v[1]],
        [v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8], v[9], v[10], v[11], v[12], v[13], v[14], v[15]]]

axis.add_collection3d(Poly3DCollection(verts, edgecolors='black', linewidths=1.5, alpha=1, facecolors='white')) 

plt.axis('off')
plt.show() 
