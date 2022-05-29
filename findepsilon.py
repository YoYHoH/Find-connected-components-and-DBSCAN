from sklearn.neighbors import NearestNeighbors
import plotly.express as px
import numpy as np
neighbors = 4

def myread( filename ):
	f=open(filename)
	a=f.readline()

	a=f.readline()
	x=[]
	while len(a)!=0:
	    b=a.strip()
	    b=b.split(" ")
	    x.append([(float)(b[0]),(float)(b[1])])
	    a=f.readline()
	x=np.array(x)
	return x

##change the filename. it is a file with only number of points and data of 2D points
filename="HA"
X_embedded=myread(filename)
# X_embedded is your data
nbrs = NearestNeighbors(n_neighbors=neighbors ).fit(X_embedded)
distances, indices = nbrs.kneighbors(X_embedded)
distance_desc = sorted(distances[:,2], reverse=True)
fig=px.line(x=list(range(1,len(distance_desc )+1)),y= distance_desc )
fig.show()







                 
