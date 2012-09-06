#ifndef __ALLOCATOR_HPP__
#define __ALLOCATOR_HPP__

#include <iostream>

namespace id
{
	template <typename T>
	class allocator;

	template <>
	class allocator<void>
	{
	public:
		typedef void*		pointer;
		typedef const void*	const_pointer;
		typedef void		value_type;

		template <typename U>
		struct rebind
		{
			typedef allocator<U> other;
		};
	};

	template <class T>
	class allocator
	{
	public:
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;
		typedef T*		pointer;
		typedef const T*	const_pointer;
		typedef T&		reference;
		typedef const T&	const_reference;
		typedef T		value_type;
		template <typename U>
		struct rebind
		{
			typedef allocator<U> other;
		};

		allocator() throw();
		allocator(const allocator&) throw();
		template <typename U>
		allocator(const allocator<U>&) throw();
		~allocator() throw();

		pointer		address(reference x) const
		{
			return (&x);
		}
		const_pointer	address(const_reference x) const
		{
			return (&x);
		}

		pointer		allocate(size_type n, allocator<void>::const_pointer hint = 0)
		{
			pointer	ret;

			std::cout << "allocatee" << std::endl;
			(void)hint;
			ret = (pointer) (::operator new (n * sizeof(T)));
			return (ret);
		}
		void		deallocate(pointer p, size_type n)
		{
			(void)n;
			::operator delete ((void*)p);
		}

		size_type	max_size() const throw()
		{
			return 0;
		}

		void		construct(pointer p, const T& val)
		{
			new ((void*)p) T(val);
		}
		void		destroy(pointer p)
		{
			((T*)p)->~T();
		}
	};
};

#endif
