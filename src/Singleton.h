/*=====================================
#
# Copyright 2016
#
# Author:luoyi 844262725@qq.com
#
# Filename: singleton.h
#
# Last modified:2016-5-7
#
# Description:
#
#=====================================*/

#ifndef SWORD_SINGLETON_H__
#define SWORD_SINGLETON_H__


#include <cassert>

namespace SWORD {

	template <typename T> class Singleton {

	protected:

		static T* msSingleton;

	public:

		Singleton(const Singleton<T> &) = delete;

		Singleton& operator=(const Singleton<T> &) = delete;

		Singleton(void)
		{
			assert(!msSingleton);

			msSingleton = static_cast<T*>(this);
		}
		~Singleton(void)
		{
			assert(msSingleton);  msSingleton = 0;
		}
		static T& instance(void)
		{
			assert(msSingleton);  return (*msSingleton);
		}
	};

}

#endif // SWORD_SINGLETON_H__
