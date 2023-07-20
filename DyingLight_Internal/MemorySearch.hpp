#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <optional>

// Github: http://github.com/TKazer

namespace MemorySearch
{
	/*
		��ʽ -> "E0 F0 B0 ?? 23 55 ?? 77"
		ͨ��� -> "??"
		std:c++17
	*/
	std::optional<DWORD64> Search(std::string Pattern, DWORD64 StartAddress, DWORD64 SearchMemorySize = 0xffffff)
	{
		if (Pattern.empty())
			return std::nullopt;
		// �����ı�ȥ��
		Pattern.erase(std::remove(Pattern.begin(), Pattern.end(), ' '), Pattern.end());

		if (Pattern.size() % 2 != 0)
			return std::nullopt;

		std::vector<std::string> PatternTextVec;
		std::vector<std::optional<UINT8>> PatternVec;

		// �����ֽ��ı�
		for (int i = 0; i < Pattern.size(); i += 2)
			PatternTextVec.push_back(Pattern.substr(i, 2));

		// �����ֽ��б�
		std::for_each(PatternTextVec.begin(), PatternTextVec.end(), [&](std::string ByteHex)
			{
				PatternVec.push_back(ByteHex == "??" ? std::nullopt : std::optional(std::stoi(ByteHex, 0, 16)));
			});

		UINT8* pStartAddress = reinterpret_cast<UINT8*>(StartAddress);
		UINT8* pEndAddress = reinterpret_cast<UINT8*>(StartAddress + SearchMemorySize);

		// �ڴ�����
		auto iter = std::search(pStartAddress, pEndAddress, PatternVec.cbegin(), PatternVec.cend(),
			[](UINT8 Byte, std::optional<UINT8> PatternByte) -> bool
			{
				return PatternByte.has_value() ? Byte == PatternByte.value() : true;
			});

		if (iter == pEndAddress)
			return std::nullopt;

		return reinterpret_cast<DWORD64>(iter);
	}
}
