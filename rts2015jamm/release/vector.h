#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <iostream>
#include <stdexcept>
#include "allocator.hpp"

namespace ID
{
	template <typename T, typename Allocator>
	class	vector;

	template <typename T, typename Allocator = std::allocator<T> >
	class	vect_iter
	{
	public:
		typedef T					value_type;
		typedef std::size_t				size_type;
		typedef std::ptrdiff_t				difference_type;
		typedef typename Allocator::reference		reference;
		typedef typename Allocator::const_reference	const_reference;
		typedef typename Allocator::pointer		pointer;
		typedef typename Allocator::const_pointer	const_pointer;

	private:
		const vector<T, Allocator>*	vect;
		size_type			index;

	public:
		vect_iter() : vect(0), index(0) { }
		vect_iter(const vector<T, Allocator>& s, size_type idx) : vect(&s), index(idx) { }
		void	operator=(const vect_iter<T, Allocator>& i)
		{
			this->vect = i.vect;
			this->index = i.index;
		}
		vect_iter	operator++(int)
		{
			return vect_iter(*this->vect, index++);
		}
		vect_iter	operator++()
		{
			index++;
			return *this;
		}
		vect_iter	operator--(int)
		{
			return vect_iter(*this->vect, index--);
		}
		vect_iter	operator--()
		{
			index--;
			return *this;
		}
		pointer	operator->() const
		{
			return this;
		}
		reference	operator*()
		{
			return this->vect->tbl[this->index];
		}
		const_reference	operator*() const
		{
			return this->vect->tbl[this->index];
		}
		reference	operator[](const difference_type& n)
		{
			return this->vect->tbl[this->index + n];
		}
		vect_iter&	operator+=(const difference_type& n)
		{
			this->index = this->index + n;
			return *this;
		}
		vect_iter&	operator-=(const difference_type& n)
		{
			this->index = this->index - n;
			return *this;
		}
		vect_iter	operator+(const difference_type& n)
		{
			return vect_iter(*this->vect, this->index + n);
		}
		vect_iter	operator-(const difference_type& n)
		{
			this->index = this->index - n;
			return *this;
		}
		bool	operator==(const vect_iter& right)
		{
			return (this->vect == right.vect && this->index == right.index);
		}
		bool	operator!=(const vect_iter<T, Allocator>& right)
		{
			return !(this->vect == right.vect && this->index == right.index);
		}
		bool	operator<(const vect_iter<T, Allocator>& right)
		{
			return (this->vect < right.vect);
/*			size_type	i;

			i = 0;
			while (i < this->vect->size() && i < right.vect->size())
			{
				if (this->vect->tbl[i] > right.vect->tbl[i])
					return false;
				if (this->vect->tbl[i] < right.vect->tbl[i])
					return true;
				i = i + 1;
			}
			if (this->vect->size() < right.vect->size())
				return true;
			return false;*/
		}
		bool	operator>(const vect_iter<T, Allocator>& right)
		{
			return (this->vect > right.vect);
/*			size_type	i;

			i = 0;
			while (i < this->vect->size() && i < right.vect->size())
			{
				if (this->vect->tbl[i] > right.vect->tbl[i])
					return true;
				if (this->vect->tbl[i] < right.vect->tbl[i])
					return false;
				i = i + 1;
			}
			if (this->vect->size() > right.vect->size())
				return true;
			return false;*/
		}
		bool	operator<=(const vect_iter<T, Allocator>& right)
		{
			if (*this == right || *this < right)
				return true;
			return false;
		}
		bool	operator>=(const vect_iter<T, Allocator>& right)
		{
			if (*this == right || *this > right)
				return true;
			return false;
		}
	};

	template <typename Parent, typename U, typename Allocator = std::allocator<U> >
	class	vect_riter : public Parent
	{
	public:
		typedef U					value_type;
		typedef std::size_t				size_type;
		typedef std::ptrdiff_t				difference_type;
		typedef typename Allocator::reference		reference;
		typedef typename Allocator::const_reference	const_reference;
		typedef typename Allocator::pointer		pointer;
		typedef typename Allocator::const_pointer	const_pointer;

	private:
		const vector<U, Allocator>*	vect;
		size_type			index;

	public:
		vect_riter() : vect(0), index(0) { }
		vect_riter(const vector<U, Allocator>& s, size_type idx) : vect(&s), index(idx) { }
		void	operator=(const vect_riter<Parent, U, Allocator>& i)
		{
			this->vect = i.vect;
			this->index = i.index;
		}
		vect_riter	operator++(int)
		{
			return vect_riter(*this->vect, index--);
		}
		vect_riter	operator++()
		{
			index--;
			return *this;
		}
		vect_riter	operator--(int)
		{
			return vect_riter(*this->vect, index--);
		}
		vect_riter	operator--()
		{
			index++;
			return *this;
		}
		reference	operator*()
		{
			return this->vect->tbl[this->index];
		}
		const_reference	operator*() const
		{
			return this->vect->tbl[this->index];
		}
		reference	operator[](const difference_type& n)
		{
			return this->vect->tbl[this->index - n];
		}
		vect_riter&	operator+=(const difference_type& n)
		{
			this->index = this->index - n;
			return *this;
		}
		vect_riter&	operator-=(const difference_type& n)
		{
			this->index = this->index + n;
			return *this;
		}
		vect_riter	operator+(const difference_type& n)
		{
			return vect_riter(*this->vect, this->index - n);
		}
		vect_riter	operator-(const difference_type& n)
		{
			this->index = this->index + n;
			return *this;
		}
		bool	operator==(const vect_riter& right)
		{
			return (this->vect == right.vect);
		}
		bool	operator!=(const vect_riter& right)
		{
			return !(this->vect == right.vect);
		}
		bool	operator<(const vect_riter& right)
		{
			return (this->vect < right.vect);
/*			size_type	i;

			i = 0;
			while (i < this->vect->size() && i < right.vect->size())
			{
				if (this->vect->tbl[i] > right.vect->tbl[i])
					return false;
				if (this->vect->tbl[i] < right.vect->tbl[i])
					return true;
				i = i + 1;
			}
			if (this->vect->size() < right.vect->size())
				return true;
			return false;*/
		}
		bool	operator>(const vect_riter& right)
		{
			return (this->vect > right.vect);
/*			size_type	i;

			i = this->index;
			//while (i < this->vect->size() && i < right.vect->size())
			while (i != 0)
			{
				if (this->vect->tbl[i] > right.vect->tbl[i])
					return true;
				if (this->vect->tbl[i] < right.vect->tbl[i])
					return false;
				i = i - 1;
			}
			if (this->vect->size() > right.vect->size())
				return true;
			return false;*/
		}
		bool	operator<=(const vect_riter& right)
		{
			if (*this == right || *this < right)
				return true;
			return false;
		}
		bool	operator>=(const vect_riter& right)
		{
			if (*this == right || *this > right)
				return true;
			return false;
		}
	};

	template <typename T, typename Allocator = std::allocator<T> >
	class vector
	{
	public:
		typedef T					value_type;
		typedef Allocator				allocator_type;
		typedef std::size_t				size_type;
		typedef std::ptrdiff_t				difference_type;
		typedef typename Allocator::reference		reference;
		typedef typename Allocator::const_reference	const_reference;
		typedef typename Allocator::pointer		pointer;
		typedef typename Allocator::const_pointer	const_pointer;

		typedef vect_iter<T, Allocator>				iterator;
		typedef const vect_iter<T, Allocator>			const_iterator;
		typedef vect_riter<iterator, T, Allocator>		reverse_iterator;
		typedef const vect_riter<const_iterator, T, Allocator>	const_reverse_iterator;

	private:
		allocator_type					vector_allocator;
		T*						tbl;
		size_type					vector_capacity;
		size_type					vector_size;

		friend class	vect_iter<T, Allocator>;
		friend class	vect_riter<iterator, T, Allocator>;

	public:

		explicit	vector(const Allocator& alloc = Allocator())
		{
			this->vector_allocator = alloc;
			this->tbl = 0;
			this->vector_capacity = 0;
			this->vector_size = 0;
		}
		explicit	vector(size_type count)
		{
			if (count != 0)
				this->tbl = this->vector_allocator.allocate(count, T());
			else
				this->tbl = 0;
			this->vector_capacity = count;
			this->vector_size = count;
		}
		vector(size_type count, const T& value, const Allocator& alloc = Allocator())
		{
			this->vector_allocator = alloc;
			if (count != 0)
				this->tbl = this->vector_allocator.allocate(count);
			else
				this->tbl = 0;
			this->vector_capacity = count;
			this->vector_size = count;
			while (count != 0)
			{
				this->vector_allocator.construct(this->tbl + count - 1, value);
				count = count - 1;
			}
		}

//	template <typename InputIterator>
//	vector(InputIterator first, InputIterator last, const Allocator& alloc = Allocator());

		vector(const vector& other)
		{
			size_type	i;

			this->vector_capacity = other.vector_capacity;
			this->vector_size = other.vector_size;
			this->tbl = this->vector_allocator.allocate(this->vector_capacity);
			i = 0;
			while (i < this->vector_size)
			{
				this->tbl[i] = other.tbl[i];
				i = i + 1;
			}
		}

		~vector()
		{
			while (this->vector_size != 0)
			{
				this->vector_allocator.destroy(this->tbl + this->vector_size - 1);
				this->vector_size = this->vector_size - 1;
			}
			this->vector_allocator.deallocate(this->tbl, this->vector_capacity);
		}

		vector<T,Allocator>&	operator=(const vector<T,Allocator>& right)
		{
			size_type	i;

			i = 0;
			while (i < this->vector_size)
			{
				this->vector_allocator.destroy(this->tbl + i);
				i = i + 1;
			}
			this->vector_allocator.deallocate(this->tbl, this->vector_capacity);
			this->reserve(right.vector_capacity);
			this->vector_allocator = right.vector_allocator;
			this->vector_capacity = right.vector_capacity;
			this->vector_size = right.vector_size;
			i = 0;
			while (i < this->vector_size)
			{
				this->vector_allocator.construct(this->tbl + i, right.tbl[i]);
				i = i + 1;
			}
			return *this;
		}
//	vector&			operator=(initializer_list<T>);
		void	assign(size_type count, const T& val)
		{
			size_type	i;

			i = 0;
			if (count > this->vector_capacity)
			{
				while (i < this->vector_size)
				{
					this->vector_allocator.destroy(this->tbl + i);
					i = i + 1;
				}
				this->vector_allocator.deallocate(this->tbl, this->vector_capacity);
				this->tbl = this->vector_allocator.allocate(count, this->tbl);
				i = 0;
			}
			else
			{
				while (i < this->vector_size)
				{
					this->tbl[i] = val;
					i = i + 1;
				}
			}
			while (i < count)
			{
				this->vector_allocator.construct(this->tbl + i, val);
				i = i + 1;
			}
			this->vector_capacity = count;
			this->vector_size = count;
		}
//	template <typename InputIterator>
//	void assign(InputIterator first, InputIterator last);
		friend bool	operator==(const vector<T,Allocator>& x, const vector<T, Allocator>& y)
		{
			size_type	i;

			i = 0;
			while (i < x.size() && i < y.size())
			{
				if (x.tbl[i] != y.tbl[i])
					return false;
				i = i + 1;
			}
			if (x.size() != y.size())
				return false;
			return true;
		}
		friend bool	operator!=(const vector<T,Allocator>& x, const vector<T, Allocator>& y)
		{
			return !(x == y);
		}
		friend bool	operator<(const vector<T, Allocator>& x, const vector<T, Allocator>& y)
		{
			size_type	i;

			i = 0;
			while (i < x.size() && i < y.size())
			{
				if (x.tbl[i] > y.tbl[i])
					return false;
				if (x.tbl[i] < y.tbl[i])
					return true;
				i = i + 1;
			}
			if (x.size() < y.size())
				return true;
			return false;
		}
		friend bool	operator>(const vector<T, Allocator>& x, const vector<T, Allocator>& y)
		{
			size_type	i;

			i = 0;
			while (i < x.size() && i < y.size())
			{
				if (x.tbl[i] < y.tbl[i])
					return false;
				if (x.tbl[i] > y.tbl[i])
					return true;
				i = i + 1;
			}
			if (x.size() > y.size())
				return true;
			return false;
		}
		friend bool	operator<=(const vector<T, Allocator>& x, const vector<T, Allocator>& y)
		{
			if (x == y || x < y)
				return true;
			return false;
		}
		friend bool	operator>=(const vector<T, Allocator>& x, const vector<T, Allocator>& y)
		{
			if (x == y || x > y)
				return true;
			return false;
		}
		allocator_type		get_allocator() const
		{
			return this->vector_allocator;
		}

		reference	at(size_type pos)
		{
			if (pos >= this->vector_size)
				throw std::out_of_range("vector::at");
			return this->tbl[pos];
		}
		const_reference at(size_type pos) const
		{
			if (pos >= this->vector_size)
				throw std::out_of_range("vector::at");
			return this->tbl[pos];
		}
		reference       operator[](size_type pos)
		{
			return this->tbl[pos];
		}
		const_reference	operator[](size_type pos) const
		{
			return this->tbl[pos];
		}
		reference	front()
		{
			return this->tbl[0];
		}
		const_reference	front() const
		{
			return this->tbl[0];
		}
		reference	back()
		{
			return this->tbl[this->vector_size - 1];
		}
		const_reference	back() const
		{
			return this->tbl[this->vector_size - 1];
		}
		T*		data()
		{
			return this->tbl;
		}
		const T*	data() const
		{
			return this->tbl;
		}
		iterator		begin()
		{
			return iterator(*this, 0);
		}
		const_iterator		begin() const
		{
			return const_iterator(*this, 0);
		}
		iterator		end()
		{
			return iterator(*this, this->vector_size);
		}
		const_iterator		end() const
		{
			return const_iterator(*this, this->vector_size);
		}
		reverse_iterator	rbegin()
		{
			return reverse_iterator(*this, this->vector_size - 1);
		}
		const_reverse_iterator	rbegin() const
		{
			return reverse_iterator(*this, this->vector_size - 1);
		}
		reverse_iterator	rend()
		{
			return reverse_iterator(*this, -1);
		}
		const_reverse_iterator	rend() const
		{
			return reverse_iterator(*this, -1);
		}

		bool		empty() const
		{
			if (this->vector_size == 0)
				return true;
			return false;
		}
		size_type	size() const
		{
			return this->vector_size;
		}
		size_type	max_size() const
		{
			return this->vector_allocator.max_size();
		}
		void		reserve(size_type size)
		{
			T*		tmp;
			size_type	i;

			if (this->vector_size < size)
			{
				tmp = this->tbl;
				this->tbl = this->vector_allocator.allocate(size);
				i = 0;
				while (i < this->vector_size)
				{
					this->vector_allocator.construct(this->tbl + i, tmp[i]);
					this->vector_allocator.destroy(tmp + i);
					i = i + 1;
				}
				this->vector_allocator.deallocate(tmp, this->vector_capacity);
				this->vector_capacity = size;
			}
		}
		size_type	capacity() const
		{
			return this->vector_capacity;
		}

		void		clear()
		{
			::operator delete (this->tbl);
			this->tbl = NULL;
			this->vector_size = 0;
			this->vector_capacity = 0;
		}
//	iterator	insert(iterator pos, const value_type& value)
//	void		insert(iterator pos, size_type count, const value_type& value);
//	template <typename InputIterator>
//	void		insert(iterator pos, InputIterator first, InputIterator last);
//	iterator	erase(const_iterator pos);
//	iterator	erase(const_iterator first, const_iterator last);
		void		push_back(const T& val)
		{
			if (this->vector_size >= this->vector_capacity)
			{
				if (this->vector_capacity == 0)
					this->reserve(1);
				this->reserve(this->vector_capacity + this->vector_capacity);
			}
			this->vector_allocator.construct(this->tbl + this->vector_size, val);
			this->vector_size = this->vector_size + 1;
		}

		void		pop_back()
		{
			this->vector_allocator.destroy(this->tbl + this->vector_size - 1);
			this->vector_size = this->vector_size - 1;
		}

		void		resize(size_type count, T value = T())
		{
			size_type	i;

			i = this->vector_size;
			if (this->vector_size < count)
			{
				this->reserve(count);
				while (i < count)
				{
					this->vector_allocator.construct(this->tbl + i, value);
					i = i + 1;
				}
			}
			else if (this->vector_size > count)
			{
				while (i > count)
				{
					i = i - 1;
					this->vector_allocator.destroy(this->tbl + i);
				}
			}
			this->vector_size = count;
		}
		void		swap(vector& other)
		{
			vector	tmp;

			tmp.vector_allocator = this->vector_allocator;
			tmp.vector_size = this->vector_size;
			tmp.vector_capacity = this->vector_capacity;
			tmp.tbl = this->tbl;

			this->vector_allocator = other.vector_allocator;
			this->vector_size = other.vector_size;
			this->vector_capacity = other.vector_capacity;
			this->tbl = other.tbl;

			other.vector_allocator = tmp.vector_allocator;
			other.vector_size = tmp.vector_size;
			other.vector_capacity = tmp.vector_capacity;
			other.tbl = tmp.tbl;
		}
	};
};

#endif
