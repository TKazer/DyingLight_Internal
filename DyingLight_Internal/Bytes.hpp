#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>

/****************************************************
* Copyright (C): Liv
* @file		: Bytes.hpp
* @author	: Liv
* @email	: 1319923129@qq.com
* @version	: 1.1
* @date		: 2022/5/10	  12:36
****************************************************/

//类 - 字节管理
class Bytes
{

public:
	std::vector<byte> Data;
	// 默认构造
	Bytes() {}
	// 以字节指针和大小初始化d
	Bytes(const byte* _In, size_t Size);
	// 以字节集合初始化
	Bytes(std::initializer_list<byte> _In);
	// 以整数型初始化字节数组
	Bytes(const int& _In);
	// 以长整数型初始化字节数组
	Bytes(const long long& _In);
	// 以机器码字符串初始化字节数组
	Bytes(const std::string& _In);
	// 重载运算符
	bool operator!=(Bytes _In)
	{
		for (int i = 0; i < _In.Length(); i++)
		{
			if (this->Data[i] != _In.Data[i])
				return true;
		}
		return false;
	}
	bool operator==(Bytes _In)
	{
		for (int i = 0; i < _In.Length(); i++)
		{
			if (this->Data[i] != _In.Data[i])
				return false;
		}
		return true;
	}
	void operator=(Bytes _In)
	{
		Data.clear();
		for (int i = 0; i < _In.Length(); i++)
			this->Data.push_back(_In.Data[i]);
	}
	void operator+=(Bytes _In)
	{
		for (int i = 0; i < _In.Length(); i++)
			this->Data.push_back(_In.Data[i]);
	}
	Bytes& operator+(Bytes _In)
	{
		for (int i = 0; i < _In.Length(); i++)
			this->Data.push_back(_In.Data[i]);
		return *this;
	}

	// 添加字节
	Bytes& Add(const byte _In);
	// 返回字节数组大小
	int Length();
	// 替换字节数据
	bool Replace(int Index, int Length, Bytes Source);
	// 寻找字节数据
	int Find(Bytes Source, int Index = 0);
	// 取中间字节数据
	Bytes Get(int Index, int Length);
	byte* GetData();
	// 输出字节数组
	void Print();
};

Bytes::Bytes(const byte* _In, size_t Size)
{
	for (int i = 0; i < Size; i++)
		Data.push_back(_In[i]);
}

Bytes::Bytes(std::initializer_list<byte> _In)
{
	for (auto i = _In.begin(); i != _In.end(); ++i)
		this->Data.push_back(*i);
}

Bytes::Bytes(const int& _In)
{
	this->Data.resize(4);	
	this->Data[0] = static_cast<byte>(0xff & _In);
	this->Data[1] = static_cast<byte>((0xff00 & _In) >> 8);
	this->Data[2] = static_cast<byte>((0xff0000 & _In) >> 16);
	this->Data[3] = static_cast<byte>((0xff000000 & _In) >> 24);
}

Bytes::Bytes(const long long& _In)
{
	this->Data.resize(8);
	this->Data[0] = static_cast<byte>(0xff & _In);
	this->Data[1] = static_cast<byte>((0xff00 & _In) >> 8);
	this->Data[2] = static_cast<byte>((0xff0000 & _In) >> 16);
	this->Data[3] = static_cast<byte>((0xff000000 & _In) >> 24);
	this->Data[4] = static_cast<byte>((0xff00000000 & _In)>>32);
	this->Data[5] = static_cast<byte>((0xff0000000000 & _In) >> 40);
	this->Data[6] = static_cast<byte>((0xff000000000000 & _In) >> 48);
	this->Data[7] = static_cast<byte>((0xff00000000000000 & _In) >> 56);
}

Bytes::Bytes(const std::string& _In)
{
	if (_In.length() <= 0)
	{
		this->Data.clear();
		return;
	}

	std::string TempData = _In;
	for (int i = 0; i < TempData.length(); i++)
	{
		if (TempData[i] == ' ')
			TempData.replace(i, 1, "");
	}

	if (TempData.length() % 2 != 0)
	{
		this->Data.clear();		
		return;
	}

	for (int i = 0; i < TempData.length(); i += 2)
		this->Add(static_cast<byte>(std::stol(TempData.substr(i, 2), 0, 16)));
}

Bytes& Bytes::Add(const byte _In)
{
	Data.push_back(_In);
	return *this;
}

int Bytes::Length() {
	return this->Data.size();
}

bool Bytes::Replace(int Index, int Length, Bytes Source)
{
	if (Length > Source.Length())
		return false;
	if ((Index + Length) > this->Data.size())
		return false;
	if (Index < 0)
		return false;

	for (int i = Index, j = 0; i < Index + Length; i++, j++)
	{
		this->Data[i] = Source.Data.at(j);
	}
	return true;
}

int Bytes::Find(Bytes Source, int Index)
{
	if (Index < 0)
		return -1;
	if ((Index + Source.Data.size()) > this->Data.size())
		return -1;
	
	for (int i = 0, Counts = 0; i < this->Data.size(); i++)
	{
		for (int j = 0; j < Source.Data.size(); j++)
		{
			if ((i + j) > this->Data.size() - 1)
				return -1;

			if (this->Data.at(i + j) == Source.Data.at(j))
				Counts++;
			else
				Counts = 0;
		}
		if (Counts == Source.Data.size())
			return i;
	}
	return -1;
}

Bytes Bytes::Get(int Index, int Length)
{
	Bytes Result;
	if (Index < 0 || Index > this->Data.size())
		return {};
	for (int i = Index; i < Index + Length && i < this->Data.size(); i++)
		Result.Add(this->Data.at(i));
	return Result;
}

byte* Bytes::GetData()
{
	if (this->Data.size() == 0)
		return nullptr;
	byte* Result = new byte[this->Data.size()];
	for (int i = 0; i < this->Data.size(); i++)
		Result[i] = this->Data.at(i);
	return Result;
}

void Bytes::Print()
{
	std::cout << "[ ";
	for (auto i : this->Data)
		std::cout << static_cast<int>(i) << " ";
	std::cout << "]" << std::endl;
}