#ifndef DANA_DATAHANDLE_H
#define DANA_DATAHANDLE_H

class Foo
{
public:
    void Clear() {}
};

class DataHandleBase
{
public:
    DataHandleBase() = default;
    DataHandleBase(std::string key) : key_(key) {}
    DataHandleBase(std::string key, std::string description) : key_(key),
                                                               description_(description)
    {}

    virtual ~DataHandleBase() = default;

    std::string Key() const {return key_;};

private:
    std::string key_{};
    std::string description_{};
};

template<class data_t>
class DataHandle : public DataHandleBase
{
public:
    DataHandle(std::string key) : DataHandleBase(key) {}
    DataHandle(std::string key, data_t init_data) : DataHandleBase(key)
    {
        data_ = new data_t(init_data);
    }
    DataHandle(std::string key, data_t init_data, std::string description) : DataHandleBase(key, description)
    {
        data_ = new data_t(init_data);
    }

    virtual ~DataHandle() {delete data_; data_ = nullptr;}

    data_t* Get() {return data_;}

private:
    data_t* data_{nullptr};
};

#endif // DANA_DATAHANDLE_H
