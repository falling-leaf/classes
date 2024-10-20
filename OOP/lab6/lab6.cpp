#include "lab6.h"

//note: for the range test of at(), if the index is out of range, the program will exit.
//note: because of using free() in destruction ctor and capacity inflate function, so if the program is 
//used in VScode, it will pause at the free() function.

//default construct vector:size = 0, capacity = 5
template <class T>
Vector<T>::Vector()
{
    m_nSize = 0;
    m_nCapacity = 5;
    m_pElements = new T(m_nCapacity);
}

//size set construct vector
template <class T>
Vector<T>::Vector(int size)
{
    m_nSize = 0;
    m_nCapacity = size;
    m_pElements = new T(m_nCapacity);
}

//copy the vector to the new vector
template <class T>
Vector<T>::Vector(const Vector& r)
{
    m_nSize = r.m_nSize;
    m_nCapacity = r.m_nCapacity;
    m_pElements = new T(m_nCapacity);
    for (int i = 0; i < m_nSize; i++)
        m_pElements[i] = r.m_pElements[i];
}

//destruct the vector
template <class T>
Vector<T>::~Vector()
{
    cout << "desctructing vector..." << endl;
    delete(m_pElements);
    m_nSize = m_nCapacity = 0;
}

template <class T>
T& Vector<T>::operator[](int index)
{
    return m_pElements[index];
}

//get vector a[index], judge the range of index
template <class T>
T& Vector<T>::at(int index)
{
    if (index < 0 || index >= m_nSize)
    {
        cout << "the index is out of range. the program will exit." << endl;
        exit(1);
    }
    return m_pElements[index];
}

// return the size of the container
template <class T>
int Vector<T>::size() const
{
    return m_nSize;
}          

// adds an element to the end 
template <class T>
void Vector<T>::push_back(const T& x)
{
    m_nSize++;
    if (m_nSize >= m_nCapacity)
    {
        int newsize = m_nSize * 1.5;
        int i = 0;
        T* temp = new T(newsize);
        m_nCapacity = newsize;
        for (i = 0; i < m_nSize - 1; i++)
            temp[i] = m_pElements[i];
        temp[i] = x;
        delete(m_pElements);
        m_pElements = temp;
    }
    else m_pElements[m_nSize - 1] = x;
}

//clear the content(but won't change the capacity)
template <class T>
void Vector<T>::clear()
{
    int i = 0;
    for (i = 0; i < m_nSize; i++)
        m_pElements[i] = 0;
    m_nSize = 0;
}

//check if the vector is empty(that doesn't mean the capacity is 0)
template <class T>
bool Vector<T>::empty() const
{
    if (m_nSize == 0)
        return true;
    else return false;
}

//expand the capacity to double
template <class T>
void Vector<T>::inflate()
{
    T* temp;
    temp = new T(2 * m_nCapacity);
    int i = 0;
    for (i = 0; i < m_nSize; i++)
        temp[i] = m_pElements[i];
    delete(m_pElements);
    m_nCapacity = 2 * m_nCapacity;
    m_pElements = temp;
}

int main()
{
    Vector<int> a;//test default vector
    Vector<char> b(1);//set the vector b with capacity 1
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);
    b.push_back('a');
    b.push_back('b');
    b.push_back('c');
    b.push_back('d');
    cout << b.size() << endl;//return the size of b
    cout << a[0] << " " << b[0] << endl;//test the operator[]
    b[1] = 'e';//test the operator[] as a left value
    cout << b.at(1) << endl;//test at()
    b.at(2) = 'f';//test at() as a left value
    cout << b[2] << endl;
    Vector<char> c(b);//test copy function
    cout << c[2] << endl;
    b.clear();//test clear function
    cout << b.size() << endl;//the size of b is now 0.
    if (b.empty())//test empty funcion
        cout << "now b is empty." << endl;

    
}