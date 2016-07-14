#pragma once

#include <algorithm>
#include <mutex>
#include <vector>
#include <initializer_list>
#include "core/Exception.h"

namespace Core
{

template<class T>
class FixedMatrix
{
public:
    FixedMatrix(size_t rows, size_t cols);
    FixedMatrix(size_t rows, size_t cols, const T * data, size_t dataSize);
    FixedMatrix(size_t rows, size_t cols, const T ** data, size_t dataRows, size_t dataCols);
    FixedMatrix(size_t rows, size_t cols, T ** data, size_t dataRows, size_t dataCols);
    FixedMatrix(const FixedMatrix<T> & other);
    FixedMatrix(FixedMatrix<T> && other);

    FixedMatrix(typename std::vector< std::initializer_list<T> > data);
    virtual ~FixedMatrix();
    size_t Rows() const { return rows; }
    size_t Cols() const { return cols; }

    T const * const * Data() const { return data; }
    operator T const * const * () const { return data; }
    T ** Data() { return data; }
    operator T ** () { return data; }

    void Clear();

    T Get(size_t offsetRows, size_t offsetCols) const;
    size_t Get(size_t offsetRows, size_t offsetCols, T * data, size_t dataSize) const;
    void Get(size_t offsetRows, size_t offsetCols, FixedMatrix<T> & data, size_t numRows, size_t numCols) const;
    FixedMatrix<T> Get(size_t offsetRows, size_t offsetCols, size_t numRows, size_t numCols) const;
    void Set(size_t offsetRows, size_t offsetCols, T data);
    void Set(size_t offsetRows, size_t offsetCols, const T * data, size_t dataSize);
    void Set(size_t offsetRows, size_t offsetCols, const T ** data, size_t dataRows, size_t dataCols);
    void Set(size_t offsetRows, size_t offsetCols, T ** data, size_t dataRows, size_t dataCols);
    void Set(size_t offsetRows, size_t offsetCols, const FixedMatrix<T> & data);

    FixedMatrix<T> & operator = (const FixedMatrix<T> & other);
    FixedMatrix<T> & operator = (FixedMatrix<T> && other);
    bool operator == (const FixedMatrix<T> & other) const;
    bool operator != (const FixedMatrix<T> & other) const;
    T& operator() (size_t offsetRows, size_t offsetCols);
    const T& operator() (size_t offsetRows, size_t offsetCols) const;
    void PrintTo(std::ostream & stream) const;

protected:
    FixedMatrix();
    void AllocateSize(size_t newRows, size_t newCols);
    void Free();

    size_t rows;
    size_t cols;
    T ** data;

    typedef std::recursive_mutex Mutex;
    typedef std::lock_guard<Mutex> Lock;
    Mutex mutex;
};

template<class T>
FixedMatrix<T>::FixedMatrix(size_t rows, size_t cols) :
    rows(0),
    cols(0),
    data(nullptr)
{
    AllocateSize(rows, cols);
    Clear();
}

template<class T>
FixedMatrix<T>::FixedMatrix(size_t rows, size_t cols, const T * data, size_t dataSize) :
    rows(0),
    cols(0),
    data(nullptr)
{
    if ((rows < 0) || (cols < 0))
        return;
    AllocateSize(rows, cols);
    Clear();
    try
    {
        Set(0, 0, data, dataSize);
    }
    catch (...)
    {
        Free();
        throw;
    }
}

template<class T>
FixedMatrix<T>::FixedMatrix(size_t rows, size_t cols, const T ** data, size_t dataRows, size_t dataCols) :
    rows(0),
    cols(0),
    data(nullptr)
{
    if ((rows < 0) || (cols < 0))
        return;
    AllocateSize(rows, cols);
    Clear();
    try
    {
        Set(0, 0, data, dataRows, dataCols);
    }
    catch (...)
    {
        Free();
        throw;
    }
}

template<class T>
FixedMatrix<T>::FixedMatrix(size_t rows, size_t cols, T ** data, size_t dataRows, size_t dataCols) :
    rows(0),
    cols(0),
    data(nullptr)
{
    if ((rows < 0) || (cols < 0))
        return;
    AllocateSize(rows, cols);
    Clear();
    Set(0, 0, data, dataRows, dataCols);
}

template<class T>
FixedMatrix<T>::FixedMatrix(const FixedMatrix<T> & other) :
    rows(0),
    cols(0),
    data(nullptr)
{
    AllocateSize(other.Rows(), other.Cols());
    Clear();
    Set(0, 0, other);
}

template<class T>
FixedMatrix<T>::FixedMatrix(FixedMatrix<T> && other) :
    rows(other.rows),
    cols(other.cols),
    data(other.data)
{
    other.rows = 0;
    other.cols = 0;
    other.data = nullptr;
}

template<class T>
FixedMatrix<T>::FixedMatrix(typename std::vector< std::initializer_list<T> > data) :
    rows(0),
    cols(0),
    data(nullptr)
{
    size_t maxWidth = 0;
    for (auto row : data)
    {
        maxWidth = max(maxWidth, row.size());
    }
    AllocateSize(data.size(), maxWidth);
    Clear();
    size_t offsetRows = 0;
    for (auto row : data)
    {
        size_t offsetCols = 0;
        for (auto value : row)
        {
            Set(offsetRows, offsetCols++, value);
        }
        offsetRows++;
    }
}

template<class T>
FixedMatrix<T>::~FixedMatrix()
{
    Free();
}

template<class T>
void FixedMatrix<T>::Clear()
{
    if (data == nullptr)
        return;
    for (size_t x = 0; x < rows; x++)
    {
        if (data[x] != nullptr)
            memset(data[x], 0, cols * sizeof(T));
    }
}

template<class T>
T FixedMatrix<T>::Get(size_t offsetRows, size_t offsetCols) const
{
    if ((offsetRows >= 0) && (offsetRows < rows) &&
        (offsetCols >= 0) && (offsetCols < cols))
    {
        return data[offsetRows][offsetCols];
    }
    throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offsetRows/offsetCols", "Invalid index");
}

template<class T>
size_t FixedMatrix<T>::Get(size_t offsetRows, size_t offsetCols, T * data, size_t dataSize) const
{
    if ((offsetRows < 0) || (offsetCols < 0) || (dataSize < 0))
        throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offsetRows/offsetCols", "Invalid index");
    size_t srcIndex = 0;
    size_t x = offsetRows;
    size_t y = offsetCols;
    size_t count = std::min(dataSize, (rows - offsetRows) * (cols - offsetCols));
    while (count > 0)
    {
        size_t valuesToCopy = std::min(cols - y, count);
        if (valuesToCopy > 0)
        {
            memcpy(data + srcIndex, this->data[x] + y, valuesToCopy * sizeof(T));
        }
        srcIndex += valuesToCopy;
        count -= valuesToCopy;
        y = 0;
        x++;
    }
    return dataSize;
}

template<class T>
void FixedMatrix<T>::Get(size_t offsetRows, size_t offsetCols, FixedMatrix<T> & data, size_t numRows, size_t numCols) const
{
    if ((offsetRows < 0) || (numRows < 0) || (offsetCols < 0) || (numCols < 0))
        throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offsetRows/offsetCols", "Invalid index");
    size_t dataRows = std::min(numRows, rows - offsetRows);
    size_t dataCols = std::min(numCols, cols - offsetCols);
    data.AllocateSize(dataRows, dataCols);
    for (size_t x = 0; x < dataRows; x++)
    {
        for (size_t y = 0; y < dataCols; y++)
        {
            memcpy(data.data[x], this->data[x + offsetRows] + offsetCols, dataCols * sizeof(T));
        }
    }
}

template<class T>
FixedMatrix<T> FixedMatrix<T>::Get(size_t offsetRows, size_t offsetCols, size_t numRows, size_t numCols) const
{
    if ((offsetRows < 0) || (numRows < 0) || (offsetCols < 0) || (numCols < 0))
        throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offsetRows/offsetCols", "Invalid index");
    size_t dataRows = std::min(numRows, rows - offsetRows);
    size_t dataCols = std::min(numCols, cols - offsetCols);
    FixedMatrix<T> result(dataRows, dataCols);
    for (size_t x = 0; x < dataRows; x++)
    {
        for (size_t y = 0; y < dataCols; y++)
        {
            memcpy(result.data[x], this->data[x + offsetRows] + offsetCols, dataCols * sizeof(T));
        }
    }
    return result;
}

template<class T>
void FixedMatrix<T>::Set(size_t offsetRows, size_t offsetCols, T value)
{
    if ((offsetRows < 0) || (offsetCols < 0) || (offsetRows >= rows) || (offsetCols >= cols))
        throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offsetRows/offsetCols", "Invalid index / offset");
    data[offsetRows][offsetCols] = value;
}

template<class T>
void FixedMatrix<T>::Set(size_t offsetRows, size_t offsetCols, const T * data, size_t dataSize)
{
    if ((offsetRows < 0) || (offsetCols < 0) || (dataSize < 0) || (rows * cols < (dataSize + offsetRows * cols + offsetCols)))
        throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offsetRows/offsetCols", "Invalid index / offset");
    size_t srcIndex = 0;
    size_t x = offsetRows;
    size_t y = offsetCols;
    size_t count = dataSize;
    while (count > 0)
    {
        size_t valuesToCopy = std::min(cols - y, count);
        if (valuesToCopy > 0)
        {
            memcpy(this->data[x] + y, data + srcIndex, valuesToCopy * sizeof(T));
        }
        srcIndex += valuesToCopy;
        count -= valuesToCopy;
        y = 0;
        x++;
    }
}

template<class T>
void FixedMatrix<T>::Set(size_t offsetRows, size_t offsetCols, const T ** data, size_t dataRows, size_t dataCols)
{
    if ((offsetRows < 0) || (offsetCols < 0) || (dataRows < 0) || (dataCols < 0) ||
        (rows < (dataRows + offsetRows)) || (cols < (dataCols + offsetCols)))
        throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offsetRows/offsetCols", "Invalid index / offset");

    for (size_t x = 0; x < dataRows; x++)
    {
        for (size_t y = 0; y < dataCols; y++)
        {
            memcpy(this->data[x + offsetRows] + offsetCols, data[x], dataCols * sizeof(T));
        }
    }
}

template<class T>
void FixedMatrix<T>::Set(size_t offsetRows, size_t offsetCols, T ** data, size_t dataRows, size_t dataCols)
{
    Set(offsetRows, offsetCols, const_cast<const T **>(data), dataRows, dataCols);
}

template<class T>
void FixedMatrix<T>::Set(size_t offsetRows, size_t offsetCols, const FixedMatrix<T> & data)
{
    if (((offsetRows < 0) || (Rows() < (data.Rows() + offsetRows))) ||
        ((offsetCols < 0) || (Cols() < (data.Cols() + offsetCols))))
        throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offsetRows/offsetCols", "Invalid index / offset");

    for (size_t x = 0; x < data.rows; x++)
    {
        memcpy(this->data[x + offsetRows] + offsetCols, data.data[x], data.cols * sizeof(T));
    }
}

template<class T>
FixedMatrix<T> & FixedMatrix<T>::operator = (const FixedMatrix<T> & other)
{
    if (this != &other)
    {
        Free();
        AllocateSize(other.Rows(), other.Cols());
        Set(0, 0, other);
    }

    return *this;
}

template<class T>
FixedMatrix<T> & FixedMatrix<T>::operator = (FixedMatrix<T> && other)
{
    if (this != &other)
    {
        rows = other.rows;
        cols = other.cols;
        data = other.data;
        other.rows = 0;
        other.cols = 0;
        other.data = nullptr;
    }

    return *this;
}

template<class T>
bool FixedMatrix<T>::operator == (const FixedMatrix<T> & other) const
{
    if (&other == this)
        return true;
    if ((other.rows != rows) || (other.cols != cols))
        return false;
    for (size_t x = 0; x < rows; x++)
    {
        for (size_t y = 0; y < cols; y++)
        {
            if (other.data[x][y] != data[x][y])
                return false;
        }
    }
    return true;
}

template<class T>
bool FixedMatrix<T>::operator != (const FixedMatrix<T> & other) const
{
    return ! this->operator ==(other);
}

template<class T>
T& FixedMatrix<T>::operator() (size_t offsetRows, size_t offsetCols)
{
    if ((offsetRows >= 0) && (offsetRows < rows) && (offsetCols >= 0) && (offsetCols < cols))
    {
        return data[offsetRows][offsetCols];
    }
    throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offsetRows/offsetCols", "Invalid index");
}

template<class T>
const T& FixedMatrix<T>::operator() (size_t offsetRows, size_t offsetCols) const
{
    if ((offsetRows >= 0) && (offsetRows < rows) && (offsetCols >= 0) && (offsetCols < cols))
    {
        return data[offsetRows][offsetCols];
    }
    throw Core::ArgumentOutOfRangeException(__func__, __FILE__, __LINE__, "offsetRows/offsetCols", "Invalid index");
}

template<class T>
void PrintTo(FixedMatrix<T> value, std::ostream & stream)
{
    value.PrintTo(stream);
}

template<class T>
void FixedMatrix<T>::PrintTo(std::ostream & stream) const
{
    stream << OSAL::TypeName(*this) << " Item size: " << sizeof(T) << " Size X: " << rows << ", Y" << cols;
}

template<class T>
void FixedMatrix<T>::AllocateSize(size_t newRows, size_t newCols)
{
    Lock lock(mutex);
    if ((newRows < 0) || (newCols < 0))
        return;
    Free();
    data = (T **)malloc(newRows * sizeof(T *));
    for (size_t x = 0; x < newRows; x++)
    {
        data[x] = (T* )malloc(newCols * sizeof(T));
    }
    rows = newRows;
    cols = newCols;
}

template<class T>
void FixedMatrix<T>::Free()
{
    Lock lock(mutex);
    if (data != nullptr)
    {
        for (size_t x = 0; x < rows; x++)
        {
            if (data[x] != nullptr)
            {
                free(data[x]);
            }
        }
        free(data);
    }
    data = nullptr;
    cols = 0;
    rows = 0;
}

} // namespace Core
