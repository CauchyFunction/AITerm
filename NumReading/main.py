import sys, os, struct, time;
from array import array;
import tflow, naive;

def train(func):
	fname_img = os.path.join("./training/", "train-images.idx3-ubyte");
	fname_lbl = os.path.join("./training/", "train-labels.idx1-ubyte");

	flbl = open(fname_lbl, 'rb');
	(magic, num) = struct.unpack(">II", flbl.read(8));
	lbl = array("b", flbl.read());
	flbl.close();

	fimg = open(fname_img, 'rb');
	(magic, num, rows, cols) = struct.unpack(">IIII", fimg.read(16));
	img = array("B", fimg.read());
	fimg.close();

	for i in range(num):
		func(img[i*rows*cols : (i+1)*rows*cols], lbl[i]);

def test(func):
	fname_img = os.path.join("./testing/", "t10k-images.idx3-ubyte");
	fname_lbl = os.path.join("./testing/", "t10k-labels.idx1-ubyte");

	flbl = open(fname_lbl, 'rb');
	(magic, num) = struct.unpack(">II", flbl.read(8));
	lbl = array("b", flbl.read());
	flbl.close();

	fimg = open(fname_img, 'rb');
	(magic, num, rows, cols) = struct.unpack(">IIII", fimg.read(16));
	img = array("B", fimg.read());
	fimg.close();

	correct = 0;
	for i in range(num):
		su = func(img[i*rows*cols : (i+1)*rows*cols]);
		if (i+1)%1000 == 0: print("{}/{}".format(correct,i+1));
		if su == lbl[i]: correct+=1;
	
	return correct/num;

if __name__ == '__main__':
	cyc = 50;
	if len(sys.argv)>=2:
		try: cyc = int(sys.argv[1]);
		except: pass;

	# tflow
	st = time.time();
	train(tflow.learn);
	tflow.think(cyc);
	print("Training Complete ({} s)".format(time.time() - st));
	st = time.time();
	acc = test(tflow.getnum);
	print("Time : {} s, Accuracy : {}%".format(time.time() - st, acc*100));

	# naive
	st = time.time();
	train(naive.learn);
	naive.think(cyc);
	print("Training Complete ({} s)".format(time.time() - st));
	st = time.time();
	acc = test(naive.getnum);
	print("Accuracy : {}%".format(acc*100));
	print("Time : {} s, Accuracy : {}%".format(time.time() - st, acc*100));
