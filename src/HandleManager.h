
/*=====================================
#
# Copyright 2016
#
# Author:luoyi 844262725@qq.com
#
# Filename: HandleManager.h
#
# Last modified:2016-5-7
#
# Description:
#
#=====================================*/
#ifndef SWORD_HANDLEMANAGER_H__
#define SWORD_HANDLEMANAGER_H__


#include <cassert>
#include <stdint.h>
#include <vector>
#include "Platform.h"

SWORD_BEGIN

template<class TAG> class Handle {
public:
	Handle()
		:handle_(0) {
	}

	typedef TAG HandleTag;

	void init(uint32_t index) {
		assert(isNull());
		assert(index&MAX_INDEX == index);

		if (++s_auto_magic_ > MAX_MAGIC) {
			s_auto_magic_ = 1;
		}

		handle_data_.magic = s_auto_magic_;
		handle_data_.index = index;
	}

	bool operator !=(const Handle<TAG>& rhs) { return handle_ != rhs.get_handle(); }
	bool operator ==(const Handle<TAG>& rhs) { return handle_ == rhs.get_handle(); }

	bool isNull()const { return handle_ == 0; }
	uint32_t get_handle()const { return handle_; }
	uint32_t get_index()const { return handle_data_.index; }
	uint32_t get_magic()const { return handle_data_.magic; }

private:
	enum HandleBit {
		MAX_BITS_INDEX = 16,
		MAX_BITS_MAGIC = 16,
		INVALID_MAGIC = 0,
		MAX_INDEX = (1 << MAX_BITS_INDEX) - 1,
		MAX_MAGIC = (1 << MAX_BITS_MAGIC) - 1
	};

	struct HandleData {
		uint32_t index : MAX_BITS_INDEX;
		uint32_t magic : MAX_BITS_MAGIC;
	};

	union {
		uint32_t handle_;
		HandleData handle_data_;
	};
	static s_auto_magic_;
};

template<class DATA, class HandleTag=DATA> class HandleManager {
private:
	typedef std::vector<DATA> DataVec;
	typedef std::vector<uint32_t> MagicVec;
	typedef std::vector<uint32_t> FreeVec;
	typedef Handle<HandleTag> HandleType;

	DataVec user_data_;
	MagicVec magic_number_;
	FreeVec free_slots_;
	DATA default_data_;

public:
	DATA* acquire(HandleType& handle) {
		uint32_t index = 0;
		if (free_slots_.empty()) {
			index = magic_number_.size();
			user_data_.push_back(default_data_);
			magic_number_.push_back(handle.get_magic());
		}
		else {
			index = free_slots_.back();
			free_slots_.pop_back();
			magic_number_[index] = handle.get_magic();
		}
		handle.init(index);
		return &user_data_[index];
	}

	void release(HandleType& handle) {
		uint32_t index = handle.get_index();
		assert(index < user_data_.size());
		assert(magic_number_[index] == handle.get_magic());

		magic_number_[index] = 0;
		free_slots_.push_back(index);
	}

	DATA* dereference(const HandleType& handle) {
		
		if (handleValid(handle))
			return &user_data_[handle.get_index()];
		assert(0);
		return nullptr;
	}

	const DATA* dereference(const HandleType& handle)const {
		typedef HandleManager<DATA, HandleTag> ThisType;
		return const_cast<ThisType*>(this)->dereference(handle);
	}

	uint32_t get_used_handle_count()const { return magic_number_.size() - free_slots_.size; }
	
	bool hasUsedHandles()const { return get_used_handle_count() != 0; }

	bool handleValid(HandleType& handle) {
		return!(handle.isNull()
				|| handle.get_index() >= user_data_.size()
				|| magic_number_[handle.get_index()] != handle.get_handle());
	}
};

SWORD_END

#endif

