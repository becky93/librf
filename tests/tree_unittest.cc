

#include "tree.h"
#include "instance_set.h"
#include "weights.h"
#include <UnitTest++.h>
#include <iostream>
#include <fstream>
using namespace std;

struct TreeFixture {
	// Do some Setup
	TreeFixture() {
    // Load example libSVM file
		is_ = new InstanceSet("data/tree.svm",3);
	}
	// Do some Teardown
	~TreeFixture() {
		delete is_;
	}
		InstanceSet *is_;
};

TEST_FIXTURE(TreeFixture, TrainCheck)
{
	// Make sure the file loaded properly
	 CHECK_EQUAL(is_->size(), 8);

   weight_list *weights2 = new weight_list(8,8);
   for (int i = 0; i < 8; ++i) {
     weights2->add(i);
   }
   Tree tree(*is_, weights2, 12, 14);
   tree.grow();
   tree.print();
   cout << "running prediction on training set" <<endl;
   cout << tree.training_accuracy() << endl;
}
/*  Instance i1("-1 0:1 1:1 2:-1", 3);
  Instance i2("+1 0:1 1:-1 2:-1", 3);
  cout << "------predictions--------" << endl;
  cout << int(tree->predict(i1)) << endl;
  cout << int(tree->predict(i2)) << endl;
  cout << "running prediction on training set" <<endl;
}
/*/
struct TreeFixtureBig {
	// Do some Setup
	TreeFixtureBig() {
    // Load example libSVM file
    cout << "Loading large instance set" <<endl;
		is_ = new InstanceSet("data/heart.svm", 14);
    cout << "Loading Done";
	}
	// Do some Teardown
	~TreeFixtureBig() {
		delete is_;
	}
		InstanceSet *is_;
};
TEST_FIXTURE(TreeFixtureBig, BigCheck) {
 CHECK_EQUAL(is_->size(), 270);
  weight_list *weights2 = new weight_list(is_->size(), is_->size());
  for (int i = 0; i < is_->size() / 2; ++i) {
    weights2->add(i);
  }
  cout << "Training..." <<endl;
  Tree *tree = new Tree(*is_,
                         weights2, 12, 16);
  tree->grow();
  cout << "Training accuracy: " << tree->training_accuracy() << endl;
  tree->print();
  cout << "running prediction on training set" <<endl;
  cout << tree->training_accuracy() << endl;
  map<int, float> importance;
  unsigned int seed = 1;
  tree->variable_importance(&importance, &seed);
  for (map<int, float>::iterator i = importance.begin();
       i != importance.end();++i) {
      cout << i->first << " : " << i->second <<endl;
  }
}
/*
struct TrainPredictFixture {
  TrainPredictFixture() {
    cout << "loading photo training data" << endl;
    train_ = new InstanceSet("data/io_train.svm", 145);
    cout << "loading photo test data" << endl;
    test_ = new InstanceSet("data/io_test.svm", 145);
  }
  ~TrainPredictFixture() {
    delete train_;
    delete test_;
  }
  InstanceSet* train_;
  InstanceSet* test_;
};

TEST_FIXTURE(TrainPredictFixture, TrainPredictCheck) {
  CHECK_EQUAL(train_->size(), 3611);
  CHECK_EQUAL(test_->size(), 1204);
  weight_list *weights2 = new weight_list(3611, 3611);
  for (int i = 0; i < 3611; ++i) {
    weights2->add(i);
  }
  Tree* tree = new Tree(*train_, weights2, 0, 12);
  ofstream out("test.model");
  tree->write(out);
  Tree* tree2 = new Tree;
  ifstream in("test.model");
  tree2->read(in);
  cout << "Orig. testing accuracy" << tree->testing_accuracy(*test_) <<endl;
  cout << "Load. testing accuracy" << tree2->testing_accuracy(*test_) <<endl;
  //cout << "training accuracy" << tree->training_accuracy() <<endl;
  //cout << "testing accuracy" << tree->testing_accuracy(*test_) <<endl;
}
*/
/*
int main()
{
    return UnitTest::RunAllTests();
}*/

