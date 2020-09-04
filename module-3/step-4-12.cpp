#include <vector>
#include <list>
#include <numeric>
#include <iostream>

template<class T>
class VectorList {
private:
    using VectT = std::vector<T>;
    using ListT = std::list<VectT>;

public:
    using value_type = T;

    VectorList() = default;
    VectorList(VectorList const&) = default;
    VectorList(VectorList&&) = default;

    VectorList& operator=(VectorList&&) = default;
    VectorList& operator=(VectorList const&) = default;

    template<class It>
    void append(It p, It q);

    bool empty() const { return size() == 0; }

    size_t size() const {
        return std::accumulate(begin(), end(), 0, [](size_t res, const T&) {
            return ++res;
        });
    }

    class const_iterator 
        : public std::iterator<std::bidirectional_iterator_tag, const T> {

        typename ListT::const_iterator list_iter_;
        typename VectT::const_iterator vect_iter_;

    public:
        const_iterator() = default;

        explicit const_iterator(typename ListT::const_iterator list_iter, 
                                typename VectT::const_iterator vect_iter)
            : list_iter_(list_iter)
            , vect_iter_(vect_iter)
        {}

        const_iterator(const const_iterator& other)
            : list_iter_(other.list_iter_)
            , vect_iter_(other.vect_iter_)
        {}

        const_iterator& operator++() { 
            if (++vect_iter_ == list_iter_->end())
                vect_iter_ = (++list_iter_)->begin();
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator tmp(*this);
            ++(*this);
            return tmp;
        }

        const_iterator& operator--() {
            if (vect_iter_ == list_iter_->begin())
                vect_iter_ = (--list_iter_)->end();
            --vect_iter_;
            return *this;
        }

        const_iterator operator--(int) {
            const_iterator tmp(*this);
            --(*this);
            return tmp;
        }

        const T* operator->() const { return &(*vect_iter_); }

        const T& operator*() const { return *vect_iter_; }

        bool operator==(const_iterator other) const { 
            return vect_iter_ == other.vect_iter_;
        }

        bool operator!=(const_iterator other) const { 
            return !(*this == other);
        }
    };

    const_iterator begin() const { return const_iterator(data_.begin(), data_.begin()->begin()); }
    const_iterator end()   const { return const_iterator(data_.end(), data_.end()->begin()); }

    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    const_reverse_iterator rend()   const { return const_reverse_iterator(begin()); }

private:
    ListT data_;
};

template<class T>
template<class It>
void VectorList<T>::append(It p, It q) {
    if (p != q)
        data_.push_back(VectT(p, q));
}

int main() {
    VectorList<int> ints;
    std::vector<int> vect1{ 1, 2, 3, 4 };
    std::vector<int> vect2{ 5, 6, 7, 8 };
    ints.append(vect1.begin(), vect1.end());
    ints.append(vect2.begin(), vect2.end());
    for (auto& item : ints) {
        std::cout << item << ' ';
    }
    std::cout << std::endl;
    return 0;
}
