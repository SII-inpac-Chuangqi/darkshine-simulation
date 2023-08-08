//
// Inspired by Yulei Zhang's ECAL_Writer
// Created by Qibin Liu on 2/12/3.
//
// ECAL Machine learning I/O usinng hdf5 format
// Input: read in hdf5 and creates the collection
// Output: derive to hdf5 format

#ifndef DANA_ECAL_ML_IO_H
#define DANA_ECAL_ML_IO_H

// #include "hdf5.h"

// #ifndef H5_NO_NAMESPACE
//     #ifndef H5_NO_STD
//         using std::cout;
//         using std::endl;
//     #endif  // H5_NO_STD
// #endif

#include "H5Cpp.h"

#ifndef H5_NO_NAMESPACE
    using namespace H5;
#endif

#include <utility>

#include "Core/AnaProcessor.h"

using namespace std;

class ECAL_ML_IO : public AnaProcessor {
public:
    // No need to change anything here
    // Must initialized with Name
    explicit ECAL_ML_IO(string name, shared_ptr<EventStoreAndWriter> evtwrt);
    ~ECAL_ML_IO() override = default;

    void Begin() override;

    void InitEvt() override
    {
        ;
    }

    void ProcessEvt(AnaEvent* evt) override;

    void CheckEvt(AnaEvent* evt) override;

    void flush();
    void load();

    void End() override;

    // Define some functions here if necessary

private:

    // config
    string fpath_in;
    string fpath_out;
    string ecal_cn;
    string ecal_cb_in;
    string ecal_en;
    string ecal_col_out;
    string ecal_col_in;

    int mode_rw;
    int nbuffer;

    double ecal_cv_out;
    double ecal_th_in;
    double ecal_th_out;

    // event data
    double mECAL_cond_E;

    // C++ handler hdf5 
    H5File* h5f;
    DataSet* h5d_E;
    DataSet* h5d_cond;

    // data buffer
    double ****wdata; // -- I know the C dynamic ndarray is preimitive but let me know if you know how to do it better.
    double **cdata;

    //hdf5 related
    hid_t file, space_E, space_cond, dset_E, dset_cond, dcpl_E, dcpl_cond;
    herr_t status;
    htri_t avail;
    H5Z_filter_t filter_type;
    unsigned int filter_info;

    hsize_t dims_E[4],
            dims_cond[2],
            // extdims_E[4],
            // extdims_cond[2],
            chunk_E[4],
            chunk_cond[2],
            // start_E[4]={0},
            // end_E[4]={0},
            // start_cond[2]={0},
            // end_cond[2]={0},
            max_read=0;
    int    to_read=0,index=0; //remaining and index of event inside one chunk
};


#endif //DSIMU_EXAMPLEPROCESSOR_H
