class Data {
public:
    int object;
};

class DataArray {
public:
    DataArray();

    ~DataArray();

    int GetObjectRequestNum() const
    {
        return obj_data_array_num;
    }

    const Data* GetObjectRequestData(int i) const
    {
        if (i >= obj_data_array_num) {
            return nullptr;
        }
        return obj_data_array[i];
    }

    int SumUp(const DataArray& source)
    {
        int ret = 0;
        for (int i = 0; i < source.GetObjectRequestNum(); ++i) {
            auto value = source.GetObjectRequestData(i);
            ret = ret + value->object;
        }
        return ret;
    }

private:
    Data** obj_data_array;
    int obj_data_array_num;
};
