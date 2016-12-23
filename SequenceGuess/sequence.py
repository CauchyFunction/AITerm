import numpy as np
from numpy import matmul as mmul

## constant values
TopDown = [100000,10000,1000,100,10,1];

## pre-calculated values
#SEQ = [1,2,3,4,5,6,7,8];
#SEQ = [1,1,2,3,5,8,13,21];
SEQ = [9.0, 9.9, 10.8, 13.8, 19.5, 21.7, 29.8, 38.3, 51.7, 66.6, 65.0, 72.4, 77.8, 87.0, 96.6, 100.3, 115.5, 146.1, 197.0, 243.5, 279.3, 325.7, 350.1, 386.3, 455.6, 556.1, 598.1, 557.5, 374.2, 485.2, 561.6, 533.1, 609.0, 680.5, 764.9, 898.1, 1011.8, 1122.7, 1002.2, 901.9, 1094.5, 1202.5, 1222.8, 1305.6, 1410.4];
#SEQ = [1,4,9,16,25,36,49,64,81,100];
#SEQ = [1/1, 1/2, 1/3, 1/4, 1/5, 1/6, 1/7, 1/8, 1/9];
N = len(SEQ);
SigN = 1.0;

def delta(x1, x2):
	if x1==x2: return 1;
	return 0;

def covar(x1, x2, sigf, l):
	sik2 = delta(x1,x2)*(SigN**2);
	sik1 = np.exp(-0.5 * (((x1-x2)/l)**2))*(sigf**2);
	return sik1+sik2;

def guessVal(sigf, l, gv, x):
	K = [];
	for i in range(gv): K.append([0]*gv);
	Kinv = [];
	K2 = [[0]*gv];
	K2t = [];
	for i in range(gv): K2t.append([0]);
	Kst = [[0]];
	Yt = [];
	for i in range(gv): Yt.append([0]);

	for i in range(gv):
		for j in range(gv):
			K[i][j] = covar(i, j, sigf, l);
		K2[0][i] = covar(x, i, sigf, l);
		K2t[i][0] = K2[0][i];
		Yt[i][0] = SEQ[i];

	Kst = covar(x, x, sigf, l);
	Kinv = np.linalg.inv(np.matrix(K));

	MyY = mmul(mmul(K2, Kinv), Yt);
	return float(MyY[0][0]);

def getAccur(sigf, l):
	sums = 0;
	for i in range(1,N):
		t = i;
#		for t in range(1,i+1):
		gap = guessVal(sigf, l, t, i);
		sums += (gap-SEQ[i])**2;
	return sums;

if __name__ == '__main__':
	sgfM, lM, mi = 1, 1, getAccur(1,1);

	for ls in range(1,51,10):
		for i in range(0,len(TopDown)-1):
			for sgf in range(sgfM+TopDown[i+1],sgfM+TopDown[i],TopDown[i+1]):
				gap = getAccur(sgf, ls);
				if mi > gap: (sgfM, lM, mi) = (sgf, ls, gap);

	print("sigmaF = {}, l = {}".format(sgfM, lM));
	for i in range(N, N+11):
		print("{}: {}".format(i+1970, guessVal(sgfM, lM, N, i)));
