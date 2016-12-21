import tensorflow as tf;
import numpy as np;

## constants
SZ1 = 250;
SZ2 = 150;
SZ3 = 50;
SZ4 = 10;

## datas
img_sz = [];
x_data = [];
y_data = [];

## variables
W1_r = [];
W2_r = [];
W3_r = [];
W4_r = [];
b1_r = [];
b2_r = [];
b3_r = [];
b4_r = [];

def sigmoid(x): return 1/(1+np.exp(-x));
def add(a,b): return a+b;
def makemat(x):
	mat = [[0]*10];
	mat[0][x]=1;
	return mat;

def makenum(x):
	mxi = 0;
	for i in range(10):
		if x[0][i] > x[0][mxi]: mxi=i;
	return mxi;

def learn(img, num):
	global img_sz, x_data, y_data;
	img_sz = len(img);
	x_data.append(img);
	y_data.append(num);

def think(cycle=1):
	global W1_r, W2_r, W3_r, W4_r, b1_r, b2_r, b3_r, b4_r;
	print(img_sz);
	print(y_data[0]);
	X = tf.placeholder(tf.float32, shape=[1,img_sz]);
	Y = tf.placeholder(tf.float32);

	# Try to find values for W and b
	W1 = tf.Variable(tf.random_uniform([img_sz,SZ1], -1.0, 1.0));
	W2 = tf.Variable(tf.random_uniform([SZ1,SZ2], -1.0, 1.0));
	W3 = tf.Variable(tf.random_uniform([SZ2,SZ3], -1.0, 1.0));
	W4 = tf.Variable(tf.random_uniform([SZ3,SZ4], -1.0, 1.0));
	b1 = tf.Variable(tf.zeros([1]));
	b2 = tf.Variable(tf.zeros([1]));
	b3 = tf.Variable(tf.zeros([1]));
	b4 = tf.Variable(tf.zeros([1]));

	# Hypothesis
	XM1 = tf.sigmoid(tf.matmul(X, W1) + b1);
	XM2 = tf.sigmoid(tf.matmul(XM1, W2) + b2);
	XM3 = tf.sigmoid(tf.matmul(XM2, W3) + b3);
	y = tf.sigmoid(tf.matmul(XM3, W4) + b4);
	cost = tf.reduce_mean(tf.square(y - Y));

	a = tf.Variable(0.1);
	optimizer = tf.train.GradientDescentOptimizer(a);
	train = optimizer.minimize(cost);

	# Simulate
	init = tf.global_variables_initializer();
	sess = tf.Session();
	sess.run(init);

	for i in range(cycle*img_sz):
		i2 = i%img_sz;
		sess.run(train, feed_dict={ X:x_data[i2:i2+1], Y:makemat(y_data[i2]) });
	
	(W1_r, W2_r, W3_r, W4_r) = (sess.run(W1), sess.run(W2), sess.run(W3), sess.run(W4));
	(b1_r, b2_r, b3_r, b4_r) = (sess.run(b1), sess.run(b2), sess.run(b3), sess.run(b4));
#	print(W1_r, W2_r, b1_r, b2_r);
	pass;

def getnum(img):
	vsig = np.vectorize(sigmoid);
	vadd = np.vectorize(add);
	XM1 = vsig(vadd(np.matmul([img], W1_r), b1_r));
	XM2 = vsig(vadd(np.matmul(XM1, W2_r), b2_r));
	XM3 = vsig(vadd(np.matmul(XM2, W3_r), b3_r));
	y = vsig(vadd(np.matmul(XM3, W4_r), b4_r));
	return makenum(y);
