import sys, os, struct;
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
		if su == lbl[i]: correct+=1;
	
	return correct/num;

if __name__ == '__main__':
	cyc = 1;
	if len(sys.argv)>=2:
		try: cyc = int(sys.argv[1]);
		except: pass;

	train(tflow.learn);
	tflow.think(50);
	acc = test(tflow.getnum);
	print("Accuracy : {}%".format(acc*100));
