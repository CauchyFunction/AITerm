import numpy as np;

## constants
OptRate = 0.01; # 250, 150, 50, 10
SZ1 = 40;
SZ2 = 20;
SZ3 = 10;
SZ4 = 10;

## datas
img_sz = 784;
x_data = [];
y_data = [];

## variables
W = [0,0,0,0];
B = [0,0,0,0];
W[0] = [[0]*SZ1] * img_sz;
W[1] = [[0]*SZ2] * SZ1;
W[2] = [[0]*SZ3] * SZ2;
W[3] = [[0]*SZ4] * SZ3;
B[0] = [[0]*SZ1];
B[1] = [[0]*SZ2];
B[2] = [[0]*SZ3];
B[3] = [[0]*SZ4];

## functions (basic)
def sigmoid(x): return 1/(1+np.exp(-x));
def add(a,b): return a+b;
def deri_sigmoid2(x): return x*(1-x);
def deri_sigmoid(x):
	t = np.exp(x);
	return t/(t+1)/(t+1);

def makemat(x):
	mat = [[0]*10];
	mat[0][x]=1;
	return mat;

def makenum(x):
	mxi = 0;
	for i in range(10):
		if x[0][i] > x[0][mxi]: mxi=i;
	return mxi;

## functions (user)
def learn(img, num):
	global img_sz, x_data, y_data;
	img_sz = len(img);
	x_data.append(img);
	y_data.append(num);

Y = [0, 0, 0, 0];
def calc(vec):
	global Y;
	vsig = np.vectorize(sigmoid);
	Y[0] = vsig(np.add(np.matmul(vec, W[0]), B[0]));
	for i in range(1,4): Y[i] = vsig(np.add(np.matmul(Y[i-1], W[i]), B[i]));

def feedback_spe(ix, vec, dE):
	global W, B;
	(r,c) = (len(W[ix]), len(W[ix][0])); # i (vec)->j (dE)
	for j in range(0,c):
		for i in range(0,r):
			W[ix][i][j] += OptRate * dE[j] * vec[i];
		B[ix][0][j] += OptRate * dE[j];
	pass;

def feedback(vec, ans):
	calc(vec);
	A = Y;
	for i in range(0,len(A[3][0])):
		A[3][0][i] = -(ans[0][i]-Y[3][0][i]) * deri_sigmoid2(Y[3][0][i]);
	for j in range(2,-1,-1):
		for i in range(0,len(A[j][0])):
			A[j][0][i] = 0;
			for k in range(0,len(A[j+1][0])):
				A[j][0][i] += A[j+1][0][k] * W[j+1][i][k];
			A[j][0][i] *= deri_sigmoid2(Y[j][0][i]);

	feedback_spe(0, vec[0], A[0][0]);
	for i in range(1,4): feedback_spe(i, Y[i-1][0], A[i][0]);
	pass;

def think(cycle=1):
	global W;
	for k in range(4):
		for i in range(0,len(W[k])):
			for j in range(0,len(W[k][i])): W[k][i][j] = np.random.uniform(-1.0, 1.0);

	for i in range(cycle):
		i2 = i%len(y_data);
		feedback(x_data[i2:i2+1], makemat(y_data[i2]));
		if (i+1)%30 == 0: print(i+1);

#	print(W[3]);
	pass;

def getnum(img):
	calc([img]);
	return makenum(Y[3]);
