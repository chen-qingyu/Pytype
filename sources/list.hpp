//! @file list.hpp
//! @author Chen QingYu <chen_qingyu@qq.com>
//! @brief List template class.
//! @date 2023.01.05

#ifndef LIST_HPP
#define LIST_HPP

#include "detail.hpp"

namespace pyincpp
{

/// List is collection of homogeneous objects.
template <typename T>
class List
{
private:
    // Vector.
    std::vector<T> vector_;

public:
    /*
     * Constructor
     */

    /// Create an empty list.
    List() = default;

    /// Create a list with the contents of the initializer list `init`.
    List(const std::initializer_list<T>& init)
        : vector_(init)
    {
    }

    /// Create a list with the contents of the range [`first`, `last`).
    template <std::input_iterator InputIt>
    List(const InputIt& first, const InputIt& last)
        : vector_(first, last)
    {
    }

    /// Create a list from std::vector.
    List(const std::vector<T>& vector)
        : vector_(vector)
    {
    }

    /*
     * Comparison
     */

    /// Compare the list with another list.
    auto operator<=>(const List& that) const = default;

    /*
     * Iterator
     */

    /// Return an iterator to the first element of the list.
    auto begin() const
    {
        return vector_.begin();
    }

    /// Return an iterator to the element following the last element of the list.
    auto end() const
    {
        return vector_.end();
    }

    /// Return a reverse iterator to the first element of the reversed list.
    auto rbegin() const
    {
        return vector_.rbegin();
    }

    /// Return a reverse iterator to the element following the last element of the reversed list.
    auto rend() const
    {
        return vector_.rend();
    }

    /*
     * Access
     */

    /// Return the reference to the element at the specified position in the list.
    /// Index can be negative, like Python's list: list[-1] gets the last element.
    T& operator[](int index)
    {
        detail::check_bounds(index, -size(), size());

        return vector_[index >= 0 ? index : index + size()];
    }

    /// Return the const reference to element at the specified position in the list.
    /// Index can be negative, like Python's list: list[-1] gets the last element.
    const T& operator[](int index) const
    {
        return const_cast<List&>(*this)[index];
    }

    /*
     * Examination
     */

    /// Return the number of elements in the list.
    int size() const
    {
        return vector_.size();
    }

    /// Return `true` if the list contains no elements.
    bool is_empty() const
    {
        return vector_.empty();
    }

    /// Return the iterator of the specified element in the list, or end() if the list does not contain the element.
    auto find(const T& element) const
    {
        return std::find(begin(), end(), element);
    }

    /// Return the index of the first occurrence of the specified `element`, or -1 if the list does not contain the element in the specified range [`start`, `stop`].
    int index(const T& element, int start = 0, int stop = INT_MAX) const
    {
        stop = stop > size() ? size() : stop;
        auto it = std::find(begin() + start, begin() + stop, element);
        return it == begin() + stop ? -1 : it - begin();
    }

    /// Return `true` if the list contains the specified `element` in the specified range [`start`, `stop`].
    bool contains(const T& element, int start = 0, int stop = INT_MAX) const
    {
        return index(element, start, stop) != -1;
    }

    /// Count the total number of occurrences of the specified `element` in the list.
    int count(const T& element) const
    {
        return std::count(begin(), end(), element);
    }

    /*
     * Manipulation
     */

    /// Insert the specified `element` at the specified `index` in the list.
    /// Index can be negative.
    void insert(int index, const T& element)
    {
        detail::check_full(size(), INT_MAX);
        detail::check_bounds(index, -size(), size() + 1);

        index = index >= 0 ? index : index + size();
        vector_.insert(begin() + index, element);
    }

    /// Remove and return the `element` at the specified `index` in the list.
    /// Index can be negative.
    T remove(int index)
    {
        detail::check_empty(size());
        detail::check_bounds(index, -size(), size());

        index = index >= 0 ? index : index + size();
        T element = std::move(vector_[index]);
        vector_.erase(begin() + index);

        return element;
    }

    /// Append the specified `element` to the end of the list.
    List& operator+=(const T& element)
    {
        detail::check_full(size(), INT_MAX);

        vector_.push_back(element);

        return *this;
    }

    /// Extend the specified `list` to the end of the list.
    List& operator+=(const List& list)
    {
        detail::check_full(size() / 2 + list.size() / 2, INT_MAX / 2);

        vector_.insert(end(), list.begin(), list.end());

        return *this;
    }

    /// Remove the first occurrence of the specified element from the list.
    List& operator-=(const T& element)
    {
        if (auto it = std::find(begin(), end(), element); it != end())
        {
            vector_.erase(it);
        }

        return *this;
    }

    /// Add the list to itself a certain number of `times`.
    List& operator*=(int times)
    {
        return *this = std::move(*this * times);
    }

    /// Remove all the specified `element`s from the list.
    List& operator/=(const T& element)
    {
        auto it = std::remove(vector_.begin(), vector_.end(), element);
        vector_.erase(it, vector_.end());
        return *this;
    }

    /// Rotate the list to right `n` elements.
    List& operator>>=(int n)
    {
        if (size() <= 1 || n == 0)
        {
            return *this;
        }

        if (n < 0)
        {
            return *this <<= -n;
        }

        return *this <<= size() - n;
    }

    /// Rotate the list to left `n` elements.
    List& operator<<=(int n)
    {
        if (size() <= 1 || n == 0)
        {
            return *this;
        }

        n %= size();

        if (n < 0)
        {
            n += size();
        }

        std::rotate(vector_.begin(), vector_.begin() + n, vector_.end());

        return *this;
    }

    /// Reverse the list in place.
    List& reverse()
    {
        std::reverse(vector_.begin(), vector_.end());

        return *this;
    }

    /// Eliminate duplicate elements of the list.
    /// Will not change the original relative order of elements.
    List& uniquify()
    {
        std::vector<T> buffer;
        for (auto&& e : vector_)
        {
            if (std::find(buffer.begin(), buffer.end(), e) == buffer.end())
            {
                buffer.push_back(e);
            }
        }
        vector_ = std::move(buffer);

        return *this;
    }

    /// Sort the list according to the order induced by the specified comparator.
    /// The sort is stable: the method will not reorder equal elements.
    List& sort(bool (*comparator)(const T& e1, const T& e2) = [](const T& e1, const T& e2)
               { return e1 < e2; })
    {
        std::stable_sort(vector_.begin(), vector_.end(), comparator);

        return *this;
    }

    /// Erase the contents of the range [`start`, `stop`) of the list.
    List& erase(int start, int stop)
    {
        detail::check_bounds(start, 0, size() + 1);
        detail::check_bounds(stop, 0, size() + 1);

        vector_.erase(vector_.begin() + start, vector_.begin() + stop);

        return *this;
    }

    /// Perform the given `action` for each element of the list.
    template <typename F>
    List& map(const F& action)
    {
        std::for_each(vector_.begin(), vector_.end(), action);

        return *this;
    }

    /// Filter the elements in the list so that the elements that meet the `predicate` are retained.
    template <typename F>
    List& filter(const F& predicate)
    {
        auto it = std::copy_if(vector_.begin(), vector_.end(), vector_.begin(), predicate);
        vector_.erase(it, vector_.end());

        return *this;
    }

    /// Extend the list by appending elements of the range [`first`, `last`).
    template <std::input_iterator InputIt>
    void extend(const InputIt& first, const InputIt& last)
    {
        vector_.insert(vector_.end(), first, last);
    }

    /// Remove all of the elements from the list.
    void clear()
    {
        vector_.clear();
    }

    /*
     * Production
     */

    /// Return slice of the list from `start` (included) to `stop` (excluded) with certain `step` (default 1).
    /// Index and step length can be negative.
    List slice(int start, int stop, int step = 1) const
    {
        if (step == 0)
        {
            throw std::runtime_error("Error: Require step != 0 for slice(start, stop, step).");
        }

        detail::check_bounds(start, -size(), size());
        detail::check_bounds(stop, -size() - 1, size() + 1);

        // convert
        start = start < 0 ? start + size() : start;
        stop = stop < 0 ? stop + size() : stop;

        // copy
        std::vector<T> buffer;
        for (int i = start; (step > 0) ? (i < stop) : (i > stop); i += step)
        {
            buffer.push_back(vector_[i]);
        }

        return buffer;
    }

    /// Generate a new list and append the specified `element` to the end of the list.
    List operator+(const T& element) const
    {
        return List(*this) += element;
    }

    /// Generate a new list and extend the specified `list` to the end of the list.
    List operator+(const List& list) const
    {
        return List(*this) += list;
    }

    /// Generate a new list and remove the first occurrence of the specified `element` from the list.
    List operator-(const T& element) const
    {
        return List(*this) -= element;
    }

    /// Generate a new list and add the list to itself a certain number of `times`.
    List operator*(int times) const
    {
        if (times < 0)
        {
            throw std::runtime_error("Error: Require times >= 0 for repeat.");
        }

        detail::check_full(size() * times, INT_MAX);

        std::vector<T> buffer(size() * times);
        for (int part = 0; part < times; part++)
        {
            std::copy(begin(), end(), buffer.begin() + size() * part);
        }

        return buffer;
    }

    /// Generate a new list and remove all the specified `elements` from the list.
    List operator/(const T& element) const
    {
        return List(*this) /= element;
    }

    /*
     * Print
     */

    /// Output the list to the specified output stream.
    friend std::ostream& operator<<(std::ostream& os, const List& list)
    {
        return detail::print(os, list.begin(), list.end(), '[', ']');
    }
};

} // namespace pyincpp

#endif // LIST_HPP
