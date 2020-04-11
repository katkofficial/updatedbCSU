#pragma once

#include <vector>
#include <string>

#include "Constants.h"

namespace data
{
	enum fields : size_t
	{
		fullName,
		empty,
		OO,
		zero,
		empty2,
		contractNumber,
		debt,
		empty3,
		empty4,
		empty5,
		column102,
		contractHolder,
		contractNumber2,
		contractConclusionDate,
		column100,
		faculty,
		status
	};

	//Container size must be tableColumns size
	template<typename CharT, typename container = std::vector<std::basic_string<CharT>>>
	class UsersData
	{
	private:
		container data;

	public:
		UsersData(const container& values);

		UsersData(container&& values) noexcept;

		const std::basic_string<CharT>& get(fields field) const noexcept;

		const container& get() const noexcept;

		std::basic_string<CharT>& get(fields field);

		container& get();

		~UsersData() = default;
	};

	template<typename CharT,typename container>
	UsersData<CharT, container>::UsersData(const container& values) : data(values)
	{
		
	}

	template<typename CharT,typename container>
	UsersData<CharT, container>::UsersData(container&& values) noexcept : data(std::move(values))
	{
		
	}

	template<typename CharT, typename container>
	typename const std::basic_string<CharT>& UsersData<CharT, container>::get(fields field) const noexcept
	{
		return data[field];
	}

	template<typename CharT, typename container>
	const container& UsersData<CharT, container>::get() const noexcept
	{
		return data;
	}

	template<typename CharT, typename container>
	typename std::basic_string<CharT>& UsersData<CharT, container>::get(fields field)
	{
		return data[field];
	}

	template<typename CharT, typename container>
	container& UsersData<CharT, container>::get()
	{
		return data;
	}
}