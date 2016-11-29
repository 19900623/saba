﻿#ifndef SABA_BASE_FILE_H_
#define SABA_BASE_FILE_H_

#include <cstdio>
#include <vector>
#include <cstdint>
#include <string>
#include <fstream>

namespace saba
{
	class File
	{
	public:
		using Offset = int64_t;

		File();

		File(const File&) = delete;
		File& operator = (const File&) = delete;

		bool Open(const char* filepath);
		bool OpenText(const char* filepath);
		bool Create(const char* filepath);
		bool CreateText(const char* filepath);

		bool Open(const std::string& filepath) { return Open(filepath.c_str()); }
		bool OpenText(const std::string& filepath) { return OpenText(filepath.c_str()); }
		bool Create(const std::string& filepath) { return Create(filepath.c_str()); }
		bool CreateText(const std::string& filepath) { return CreateText(filepath.c_str()); }

		void Close();
		bool IsOpen();
		Offset GetSize() const;
		bool IsBad() const;
		void ClearBadFlag();

		FILE* GetFilePointer() const;

		bool ReadAll(std::vector<char>* buffer);

		enum class SeekDir
		{
			Begin,
			Current,
			End,
		};
		bool Seek(Offset offset, SeekDir origin);
		Offset Tell();

		template <typename T>
		bool Read(T* buffer, size_t count = 1)
		{
			if (buffer == nullptr)
			{
				return false;
			}

			if (!IsOpen())
			{
				return false;
			}
#if _WIN32
			if (fread_s(buffer, sizeof(T) * count, sizeof(T), count, m_fp) != count)
			{
				m_badFlag = true;
				return false;
			}
#else // !_WIN32
			if (fread(buffer, sizeof(T), count, m_fp) != count)
			{
				m_badFlag = true;
				return false;
			}
#endif //!_WIN32
			return true;
		}

		template <typename T>
		bool ReadAll(std::vector<char>* buffer);

		template <typename T>
		bool Write(T* buffer, size_t count = 1)
		{
			if (buffer == nullptr)
			{
				return false;
			}

			if (!IsOpen())
			{
				return false;
			}

			if (fwrite(buffer, sizeof(T), count, m_fp) != count)
			{
				m_badFlag = true;
				return false;
			}
			return true;
		}

	private:
		bool OpenFile(const char* filepath, const char* mode);

	private:
		FILE*	m_fp;
		Offset	m_fileSize;
		bool	m_badFlag;
	};

	class TextFileReader
	{
	public:
		TextFileReader() = default;
		explicit TextFileReader(const char* filepath);
		explicit TextFileReader(const std::string& filepath);

		bool Open(const char* filepath);
		bool Open(const std::string& filepath);
		void Close();
		bool IsOpen();

		std::string ReadLine();
		std::string ReadAll();
		bool IsEof();

	private:
		std::ifstream	m_ifs;
	};
}

#endif // !BASE_FILE_H_