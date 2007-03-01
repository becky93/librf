#include "librf/librf.h"
#include <sstream>
#include <tclap/CmdLine.h>
#include <iostream>
#include <fstream>
#include <set>
#include <vector>

using namespace std;
using namespace librf;
using namespace TCLAP;
int main(int argc, char*argv[]) {
  // Check arguments
  try {
    CmdLine cmd("singleclock", ' ', "0.1");
    SwitchArg headerFlag("","header","CSV file has a var name header",false);
    ValueArg<string> delimArg("","delim","CSV delimiter", false,",","delimiter");
    ValueArg<string> labelArg("l", "label",
                              "Label file", false, "", "labels");
    ValueArg<string>  dataArg("d", "data",
                                   "Training Data", true, "", "testdata");
    ValueArg<string> outputArg("o", "output", "predictions", true, "", "output");
    ValueArg<int> treesArg("t", "trees", "# Trees", false, 10, "int");
    ValueArg<int> kArg("k", "vars", "# vars per tree", false, 10, "int");
    ValueArg<int> cArg("c", "clock", "which clock", true, 19, "int"); 
    cmd.add(delimArg);
    cmd.add(labelArg);
    cmd.add(outputArg);
    cmd.add(dataArg);
    cmd.add(treesArg);
    cmd.add(kArg);
    cmd.add(cArg);
    cmd.parse(argc, argv);

    bool header = headerFlag.getValue();
    string delim = delimArg.getValue();
    string labelfile = labelArg.getValue();
    string datafile = dataArg.getValue();
    string outfile = outputArg.getValue();
    string outmodel = outfile + ".model";
    int K = kArg.getValue();
    int num_trees = treesArg.getValue();
    int clock = cArg.getValue();
    float clock_threshold = clock * 50.0;
    InstanceSet* iset = InstanceSet::load_csv_and_labels(datafile, labelfile, header, delim);
    // alter instance_set
    for (int i = 0; i < iset->num_attributes(); i++) {
      vector<float> backup;
      iset->save_var(i, &backup);
      for (int j = 0; j < backup.size(); ++j) {
        float val = backup[j];
        if (val > clock_threshold) {
          backup[j] = 1;
        } else {
          backup[j] = 0;
        }
      }
      iset->load_var(i, backup);
    }
    RandomForest rf(*iset, num_trees, K);
    cout << "OOB accuracy: " << rf.oob_accuracy() << endl;
    vector<pair<float, float> > rd;
    vector<int> hist;
    rf.reliability_diagram(20, &rd, &hist);
    for (int i = 0; i < rd.size(); ++i) {
      cout << rd[i].first << " " << rd[i].second << " " << hist[i] << endl;
    }
    ofstream out(outfile.c_str());
    for (int i = 0; i < iset->size(); i++) {
      out << rf.oob_predict_prob(i, 1) << endl;
    }
    ofstream modelout(outmodel.c_str());
    rf.write(modelout);
    delete iset;
  }
  catch (TCLAP::ArgException &e)  // catch any exceptions 
  {
    cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
  }
  return 0;
}
