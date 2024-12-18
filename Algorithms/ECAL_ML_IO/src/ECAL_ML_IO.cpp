//
// Inspired by Yulei Zhang's ECAL_Writer
// Created by Qibin Liu on 2/12/3.
//
// ECAL Machine learning I/O usinng hdf5 format
// Input: read in hdf5 and creates the collection
// Output: derive to hdf5 format

#ifdef BUILD_HDF5

#include "Algo/ECAL_ML_IO.h"

#include <utility>
#include "Core/AnaData.h"

ECAL_ML_IO::ECAL_ML_IO(string name, shared_ptr<EventStoreAndWriter> evtwrt) : AnaProcessor(std::move(name),
                                                                                           std::move(evtwrt))
{
    // Add description for this AnaProcessor
    Description = "ECAL ML I/O for Calorimeter(ECAL)";

    // RegisterIntParameter("ApplytoHCAL", "Smearing on HCAL", &fs_apply_to_hcal, true);
    RegisterStringParameter("Hdf5FileImport", "Hdf5File to read in", &fpath_in, "import.h5");
    RegisterStringParameter("Hdf5FileExport", "Hdf5File to write out", &fpath_out, "export.h5");
    RegisterIntParameter("RW", "0(Read) in or 1(Write) out", &mode_rw, 1);

    RegisterStringParameter("ConditionDS", "The dataset name of condition which will be read from/write to hdf5", &ecal_cn, "condition");
    RegisterStringParameter("ConditionBranch", "The branch name of condition which will be saved to root", &ecal_cb_in, "cond_E");

    RegisterStringParameter("EnergyDS", "The dataset name of Energy which will be write/read of hdf5", &ecal_en, "energy");

    RegisterStringParameter("CollectionExport", "Calorimeter (ECAL) Collection to Use for export", &ecal_col_out, "ECAL_FS0");
    RegisterStringParameter("CollectionImport", "Calorimeter (ECAL) Collection to Import to", &ecal_col_in, "mECAL");

    RegisterDoubleParameter("ConditionValueExport", "The condition value which will be saved to the hdf5 in the conditionDS", &ecal_cv_out, 8000000.);
    RegisterDoubleParameter("ThresoldImport", "The thresold value used for import to hit", &ecal_th_in, 0.);
    RegisterDoubleParameter("ThresoldExport", "The thresold value used for export to hdf5", &ecal_th_out, 0.);
    // later the truth value or individual partcle energy could be used
    RegisterIntParameter("chunk", "how many event in the buffer", &nbuffer, 1000);

    RegisterIntParameter("CompressionLevel", "Compression level for hdf5 (0-9)", &compression_level, 1);
    // RegisterDoubleParameter("Calibration_Factor", "Calibration Factor", &scale_factor,
    //                         1.); //this is for post-calibration
}

void ECAL_ML_IO::Begin()
{
    /*
     *
     *  DEFINE Processor explicitly with NAME in ControlManager.cpp L.50
     *  DEFINE Processor explicitly with NAME in ControlManager.cpp L.50
     *  DEFINE Processor explicitly with NAME in ControlManager.cpp L.50
     *
     */

    if (EvtWrt && mode_rw == 0) // import
    {
        EvtWrt->RegisterDoubleVariable(ecal_col_in + "_" + ecal_cb_in, &mECAL_cond_E, ecal_col_in + "_" + ecal_cb_in + "/D");
    }

    // init the hdf5 file

    dims_E[0] = 0;
    dims_E[1] = dAnaData->getNECalCellX();
    dims_E[2] = dAnaData->getNECalCellY();
    dims_E[3] = dAnaData->getNECalCellZ();

    chunk_E[0] = nbuffer;
    chunk_E[1] = dims_E[1];
    chunk_E[2] = dims_E[2];
    chunk_E[3] = dims_E[3];

    dims_cond[0] = 0;
    dims_cond[1] = 1;
    chunk_cond[0] = nbuffer;
    chunk_cond[1] = 1;


    hsize_t i, j, k, l;
    // buffer init
    // must use single chunk memory
    wdata = new double ***[chunk_E[0]];
    for (i = 0; i < chunk_E[0]; ++i)
    {
        wdata[i] = new double **[chunk_E[1]];
        for (j = 0; j < chunk_E[1]; ++j)
        {
            wdata[i][j] = new double *[chunk_E[2]];
        }
    }
    wdata[0][0][0] = new double[chunk_E[0] * chunk_E[1] * chunk_E[2] * chunk_E[3]];
    for (i = 0; i < chunk_E[0]; ++i)
    {
        for (j = 0; j < chunk_E[1]; ++j)
        {
            for (k = 0; k < chunk_E[2]; ++k)
            {
                wdata[i][j][k] = wdata[0][0][0] + k * chunk_E[3] + j * chunk_E[3] * chunk_E[2] + i * chunk_E[3] * chunk_E[2] * chunk_E[1];
            }
        }
    }
    for (i = 0; i < chunk_E[0]; ++i)
    {
        for (j = 0; j < chunk_E[1]; ++j)
        {
            for (k = 0; k < chunk_E[2]; ++k)
            {
                for (l = 0; l < chunk_E[3]; ++l)
                {
                    wdata[i][j][k][l] = 0;
                }
            }
        }
    }

    // cond
    cdata = new double *[chunk_cond[0]];
    cdata[0] = new double[chunk_cond[0] * chunk_cond[1]];
    for (i = 0; i < chunk_cond[0]; ++i)
    {
        cdata[i] = cdata[0] + i * chunk_cond[1];
    }
    for (i = 0; i < chunk_cond[0]; ++i)
    {
        for (j = 0; j < chunk_cond[1]; ++j)
        {
            cdata[i][j] = 0;
        }
    }

    cout << "HDF5 buffer ready." << endl;

    // // check gzip filter -- skip
    // avail = H5Zfilter_avail(H5Z_FILTER_DEFLATE);
    // if (!avail)
    // {
    //     printf("gzip filter not available.\n");
    //     return;
    // }
    // status = H5Zget_filter_info(H5Z_FILTER_DEFLATE, &filter_info);
    // if (!(filter_info & H5Z_FILTER_CONFIG_ENCODE_ENABLED) ||
    //     !(filter_info & H5Z_FILTER_CONFIG_DECODE_ENABLED))
    // {
    //     printf("gzip filter not available for encoding and decoding.\n");
    //     return;
    // }

    if (mode_rw == 0)
    { // R mode;
        // file = H5Fopen(fpath_in.data(), H5F_ACC_RDONLY, H5P_DEFAULT);
        h5f = new H5File(fpath_in, H5F_ACC_RDONLY);
        h5d_E = new DataSet(h5f->openDataSet(ecal_en));

        DataSpace filespace_E = h5d_E->getSpace();
        int in_rank_E = filespace_E.getSimpleExtentNdims();
        if (in_rank_E != 4)
        {
            cerr << "Erro hdf5 format, rank=" << in_rank_E << " : " << ecal_en << endl;
            return;
        }
        hsize_t in_dims_E[4]; // dataset dimensions
        in_rank_E = filespace_E.getSimpleExtentDims(in_dims_E);
        cout << "dataset rank = " << in_rank_E << ", dimensions "
             << (unsigned long)(in_dims_E[0]) << " x "
             << (unsigned long)(in_dims_E[1]) << " x "
             << (unsigned long)(in_dims_E[2]) << " x "
             << (unsigned long)(in_dims_E[3]) << endl;
        if (in_dims_E[1] != dims_E[1] || in_dims_E[2] != dims_E[2] || in_dims_E[3] != dims_E[3])
        {
            cerr << "Erro hdf5 format, dims" << endl;
            return;
        }
        max_read = in_dims_E[0];

        DSetCreatPropList cparms_E = h5d_E->getCreatePlist();
        hsize_t in_chunk_E[4];
        int in_rank_chunk_E;
        if (H5D_CHUNKED == cparms_E.getLayout())
        {
            in_rank_chunk_E = cparms_E.getChunk(4, in_chunk_E);
            cout << "chunk rank " << in_rank_chunk_E << "dimensions "
                 << (unsigned long)(in_chunk_E[0]) << " x "
                 << (unsigned long)(in_chunk_E[1]) << " x "
                 << (unsigned long)(in_chunk_E[2]) << " x "
                 << (unsigned long)(in_chunk_E[3]) << endl;
            // check chunk
            if (in_chunk_E[0] != chunk_E[0] ||
                in_chunk_E[1] != chunk_E[1] ||
                in_chunk_E[2] != chunk_E[2] ||
                in_chunk_E[3] != chunk_E[3])
            {
                cout << "Warn chunk is differernt. Might be slow: " << ecal_en << endl;
            }
        }
        // for cond
        h5d_cond = new DataSet(h5f->openDataSet(ecal_cn));
        DataSpace filespace_cond = h5d_cond->getSpace();
        int in_rank_cond = filespace_cond.getSimpleExtentNdims();
        if (in_rank_cond != 2)
        {
            cerr << "Erro hdf5 format, rank=" << in_rank_cond << " : " << ecal_cn << endl;
            return;
        }
        hsize_t in_dims_cond[2]; // dataset dimensions
        in_rank_cond = filespace_cond.getSimpleExtentDims(in_dims_cond);
        cout << "dataset rank = " << in_rank_cond << ", dimensions "
             << (unsigned long)(in_dims_cond[0]) << " x "
             << (unsigned long)(in_dims_cond[1]) << endl;
        if (in_dims_cond[1] != dims_cond[1])
        {
            cerr << "Erro hdf5 format, dims" << endl;
            return;
        }
        if (max_read != in_dims_cond[0])
        {
            cerr << "Err not aligned condition and energy!!" << endl;
            return;
        }

        DSetCreatPropList cparms_cond = h5d_cond->getCreatePlist();
        hsize_t in_chunk_cond[2];
        int in_rank_chunk_cond;
        if (H5D_CHUNKED == cparms_cond.getLayout())
        {
            in_rank_chunk_cond = cparms_cond.getChunk(2, in_chunk_cond);
            cout << "chunk rank " << in_rank_chunk_cond << "dimensions "
                 << (unsigned long)(in_chunk_cond[0]) << " x "
                 << (unsigned long)(in_chunk_cond[1]) << endl;
            // check chunk
            if (in_chunk_cond[0] != chunk_cond[0] ||
                in_chunk_cond[1] != chunk_cond[1])
            {
                cout << "Warn chunk is differernt. Might be slow: " << ecal_cn << endl;
            }
        }

        //init load:
        load();

        cout << "HDF5 ready to read..." << endl;
    }
    else if (mode_rw == 1)
    { // W mode

        h5f = new H5File(fpath_out, H5F_ACC_TRUNC);
        // file = H5Fcreate(fpath_out.data(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
        hsize_t maxdims_E[] = {H5S_UNLIMITED, dims_E[1], dims_E[2], dims_E[3]};
        DataSpace mspace_E(4, dims_E, maxdims_E);

        DSetCreatPropList cparms_E;
        cparms_E.setChunk(4, chunk_E);
        double fill_val = 0.;
        cparms_E.setFillValue(PredType::NATIVE_DOUBLE, &fill_val);
        cparms_E.setDeflate(compression_level);

        h5d_E = new DataSet(h5f->createDataSet(
            ecal_en, PredType::NATIVE_DOUBLE,
            mspace_E, cparms_E));

        // cond
        hsize_t maxdims_cond[] = {H5S_UNLIMITED, dims_cond[1]};
        DataSpace mspace_cond(2, dims_cond, maxdims_cond);

        DSetCreatPropList cparms_cond;
        cparms_cond.setChunk(2, chunk_cond);
        cparms_cond.setFillValue(PredType::NATIVE_DOUBLE, &fill_val);
        cparms_cond.setDeflate(compression_level);

        h5d_cond = new DataSet(h5f->createDataSet(
            ecal_cn, PredType::NATIVE_DOUBLE,
            mspace_cond, cparms_cond));
        cout << "HDF5 ready to write..." << endl;
    }
    else
    {
        cerr << mode_rw << " unknown, what do you want to do?" << endl;
        return;
    }

    // reset counter
        index = 0;
}

void ECAL_ML_IO::flush()
{
    if (index <= 0)
        return;
    // if(index!=nbuffer) index=nbuffer;
    hsize_t i, j, k, l;
    hsize_t _index(index);
    hsize_t ext_dims_E[] = {_index, chunk_E[1], chunk_E[2], chunk_E[3]};
    hsize_t new_dims_E[] = {dims_E[0] + index, dims_E[1], dims_E[2], dims_E[3]};

    // extend
    h5d_E->extend(new_dims_E);

    // set region
    DataSpace fspace_E = h5d_E->getSpace();
    hsize_t offset_E[] = {dims_E[0], 0, 0, 0};
    fspace_E.selectHyperslab(H5S_SELECT_SET, ext_dims_E, offset_E);

    // build mem space
    DataSpace mspace_E(4, ext_dims_E);

    // write data
    h5d_E->write(***wdata, PredType::NATIVE_DOUBLE, mspace_E, fspace_E);

    // same for condition
    hsize_t ext_dims_cond[] = {_index, chunk_cond[1]};
    hsize_t new_dims_cond[] = {dims_cond[0] + index, dims_cond[1]};

    // extend
    h5d_cond->extend(new_dims_cond);

    // set region
    DataSpace fspace_cond = h5d_cond->getSpace();
    hsize_t offset_cond[] = {dims_cond[0], 0};
    fspace_cond.selectHyperslab(H5S_SELECT_SET, ext_dims_cond, offset_cond);

    // build mem space
    DataSpace mspace_cond(2, ext_dims_cond);

    // write condition data
    h5d_cond->write(*cdata, PredType::NATIVE_DOUBLE, mspace_cond, fspace_cond);

    // clean buffer
    for (i = 0; i < chunk_E[0]; ++i)
        for (j = 0; j < chunk_E[1]; ++j)
            for (k = 0; k < chunk_E[2]; ++k)
                for (l = 0; l < chunk_E[3]; ++l)
                    wdata[i][j][k][l] = 0;

    for (i = 0; i < chunk_cond[0]; ++i)
        for (j = 0; j < chunk_cond[1]; ++j)
            cdata[i][j] = 0;

    dims_E[0] += index;
    dims_cond[0] += index;
    index = 0;
    // cout << "Flush hdf5 successfully -->" << dims_E[0] << endl;
}

void ECAL_ML_IO::load()
{
    // cout << "dims_E[0] " << dims_E[0] << endl;
    // cout << "max_read " << max_read << endl;
    if (to_read < 0 || max_read <= dims_E[0])
    {
        // cout << "Read hdf5 at end" << endl;
        to_read = -1;
        return;
    }
    else if (max_read - dims_E[0] >= (hsize_t)nbuffer)
    {
        // cout << "Read hdf5 one chunk" << endl;
        to_read = nbuffer;
    }
    else
    {
        // cout << "Read hdf5 partial chunk" << endl;
        to_read = max_read - dims_E[0];
    }

    // E
    hsize_t _to_read(to_read);
    hsize_t in_chunk_E[] = {_to_read, chunk_E[1], chunk_E[2], chunk_E[3]};
    DataSpace mspace_E(4, in_chunk_E);
    hsize_t offset_E[] = {dims_E[0], 0, 0, 0};
    hsize_t count_E[4] = {_to_read, chunk_E[1], chunk_E[2], chunk_E[3]};
    DataSpace fspace_E = h5d_E->getSpace();
    fspace_E.selectHyperslab(H5S_SELECT_SET, count_E, offset_E);
    h5d_E->read(***wdata, PredType::NATIVE_DOUBLE, mspace_E, fspace_E);
    dims_E[0] += to_read;

    // cond
    hsize_t in_chunk_cond[] = {_to_read, chunk_cond[1]};
    DataSpace mspace_cond(2, in_chunk_cond);
    hsize_t offset_cond[] = {dims_cond[0], 0};
    hsize_t count_cond[] = {_to_read, chunk_cond[1]};
    DataSpace fspace_cond = h5d_cond->getSpace();
    fspace_cond.selectHyperslab(H5S_SELECT_SET, count_cond, offset_cond);
    h5d_cond->read(*cdata, PredType::NATIVE_DOUBLE, mspace_cond, fspace_cond);
    dims_cond[0] += to_read;

    index = 0;
    return;
}

void ECAL_ML_IO::ProcessEvt(AnaEvent *evt)
{
    hsize_t j, k, l;
    // W mode
    if (mode_rw == 1)
    {
        if (index == nbuffer)
        {
            flush();
        }
        const auto &HitCollection = evt->getCalorimeterHitCollection();
        if (HitCollection.count(ecal_col_out) != 0)
        {
            // read in hits and set corresponding location
            const auto &hits = HitCollection.at(ecal_col_out);
            double ***wdata_evt = wdata[index];
            for (auto *hit : *hits)
            {
                int Xid = hit->getCellIdX() - 1; // from 1??
                int Yid = hit->getCellIdY() - 1;
                int Zid = hit->getCellIdZ() - 1;
                if (wdata_evt[Xid][Yid][Zid] > 0)
                    cout << "Warning: hit overlap in same event!!(will be added together): " << Xid << "," << Yid << "," << Zid << endl;
                if (hit->getE() > ecal_th_out)
                    wdata_evt[Xid][Yid][Zid] += hit->getE();
            }
            // now all the condition set to the pre-set value, later could use truth
            cdata[index][0] = ecal_cv_out;
            index++;
        }
        else
        {
            // if not exists, print out error
            cerr << ecal_col_out << " not found" << endl;
        }
    }
    else if (mode_rw == 0) // R mode
    {
        mECAL_cond_E = -1;
        if (to_read < 0)
        {
            cout << "HDF5 already at end" << endl;
            return; // already ended
        }
        if (index == nbuffer)
        {
            // cout << "Loading one chunk of hdf5..." << endl;
            load();
        }
        if (index >= to_read)
        {
            cout << "HDF5 end mark " << index << " " << to_read << endl;
            to_read = -1;
            return;
        }
        // read data
        auto mECALCollection = evt->RegisterCalorimeterHitCollection(ecal_col_in);
        double ***wdata_evt = wdata[index];
        for (j = 0; j < chunk_E[1]; j++)
            for (k = 0; k < chunk_E[2]; k++)
                for (l = 0; l < chunk_E[3]; l++)
                {
                    double E = wdata_evt[j][k][l];
                    if (E < ecal_th_in)
                        continue;
                    auto hit = new CalorimeterHit();
                    hit->setE(E);
                    hit->setCellIdX(j + 1);
                    hit->setCellIdY(k + 1);
                    hit->setCellIdZ(l + 1);
                    mECALCollection->push_back(hit);
                }
        // fill condition read in
        mECAL_cond_E = cdata[index][0];
        // cout<<mECAL_cond_E<<endl;
        index++;
    }
    else
    {
        cerr << mode_rw << " unknown, what do you want to do?" << endl;
    }
}

void ECAL_ML_IO::CheckEvt(AnaEvent * /*evt*/)
{
    // cout<<"Check!"<<endl;
}

void ECAL_ML_IO::End()
{
    hsize_t i, j;
    if (mode_rw == 1 || mode_rw == 0)
    {
        if (mode_rw == 1)
            flush();
    }
    else
    {
        cerr << mode_rw << " unknown, what do you want to do?" << endl;
    }
    delete h5d_E;
    delete h5d_cond;
    delete h5f;
    cout << "Hdf5 closed." << endl;
    delete wdata[0][0][0];
    for (i = 0; i < chunk_E[0]; ++i)
    {
        for (j = 0; j < chunk_E[1]; ++j)
        {
            delete wdata[i][j];
        }
        delete wdata[i];
    }
    delete wdata;

    delete cdata[0];
    delete cdata;
    cout << "BUffer cleaned." << endl;
}

#endif // BUILD_HDF5
