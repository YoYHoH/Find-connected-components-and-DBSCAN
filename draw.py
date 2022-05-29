import matplotlib.pyplot as plt
import numpy as np

#read a file of result of dbscan
def myreaddbscan( filename ):
	f=open(filename)
	a=f.readline()
	b=a.strip()
	b=b.split(" ")
	epsilon= float(b[0])
	M=int(b[1])
	ncluster=int(b[2])
	
	a=f.readline()
	x=[]

	while len(a)!=0:
	    b=a.strip()
	    b=b.split(" ")
	    x.append([(float)(b[0]),(float)(b[1]),(int)(b[2])])
	    a=f.readline()
	x=np.array(x)

	return x,epsilon,M,ncluster

#read a file of restult of scc in 2D plan
def myreadscc( filename ):
	f=open(filename)
	a=f.readline()
	b=a.strip()
	b=b.split(" ")
	epsilon= float(b[0])
	ncluster=int(b[1])
	
	a=f.readline()
	x=[]

	while len(a)!=0:
	    b=a.strip()
	    b=b.split(" ")
	    x.append([(float)(b[0]),(float)(b[1]),(int)(b[2])])
	    a=f.readline()
	x=np.array(x)
	return x,epsilon,ncluster


## change the filename if needed
filename1="result_dbscan4"
x,epsilon1,M,ncluster=myreaddbscan(filename1)
iso=0
for i in x:
	if i[2]==-1:
		iso=iso+1
# plot the result of dbscan
plt.figure()
plt.scatter(x[:,0],x[:,1],c=x[:,2],s=40)
plt.title("epsilon:"+str(epsilon1)+" M:"+str(M)+" number of cluster: "+str(ncluster)+" number isolated point: "+str(iso))



## we can use the sklearn package to test
#from sklearn.cluster import DBSCAN
#y_pred = DBSCAN(eps = epsilon1, min_samples = M+1,algorithm='brute').fit_predict(x[:,:2])
#plt.figure()
#plt.scatter(x[:,0],x[:,1],c=y_pred,s=40)



#plot the result of connected components, if needed

#filename2="result_scc3";
#x2,epsilon2,ncomponents=myreadscc(filename2)
#iso2=0
#for i in x2:
#	if i[2]==-1:
#		iso2=iso2+1

#plt.figure()
#plt.scatter(x2[:,0],x2[:,1],c=x2[:,2])
#plt.title("epsilon:"+str(epsilon2)+" number of connected components: "+str(ncomponents)+" number isolated point: "+str(iso2))

plt.show()








