#include <cstring>
#include <cstdio>

struct string
{
public:
	int Length;
	const char* Buffer;

	string();
	string(const char* s);
	string(int length, const char* s);
	char* cstr() const;
	string Substring(int start, int length);
	char operator[] (const int index);
};

string::string()
	: Length(0), Buffer(0)
{
}

string::string(const char* s)
	: Length(std::strlen(s)), Buffer(s)
{
}

string::string(int length, const char* s)
	: Length(length), Buffer(s)
{
}

char* string::cstr() const
{
	auto buffer = new char[Length + 1];
	std::memcpy(buffer, Buffer, Length);
	buffer[Length] = 0;
	return buffer;
}

string string::Substring(int start, int length)
{
	return string(length, Buffer + start);
}

char string::operator[] (const int index)
{
	return Buffer[index];
}

namespace System
{
	namespace Console
	{
		class Console
		{
		public:
			// TODO the const here is a hack until we have proper mutability
			void Write(string value) const;
			void WriteLine(string value) const;
		};

		void Console::Write(string value) const
		{
			std::printf("%.*s", value.Length, value.Buffer);
		}

		void Console::WriteLine(string value) const
		{
			std::printf("%.*s\n", value.Length, value.Buffer);
		}

		class Arguments
		{
		private:
			string* args;
		public:
			typedef const string* const_iterator;
			const int Length;

			Arguments(int argc, const char * argv[]);
			const_iterator begin() const { return &args[0]; }
			const_iterator end() const { return &args[Length]; }
		};

		Arguments::Arguments(int argc, const char * argv[])
			: Length(argc)
		{
			args = new string[argc];
			for (int i = 0; i < argc; i++)
				args[i] = string(argv[i]);
		}
	}

	namespace Collections
	{
		template<typename T>
		class Array
		{
		private:
			T* data;

		public:
			const int Count;

			Array(int capacity);
			T operator [](int i) const { return data[i]; }
			T& operator [](int i) { return data[i]; }
		};

		template<typename T>
		Array<T>::Array(int capacity)
			: Count(capacity), data(new T[capacity])
		{
		}
	}

	namespace IO
	{
		class FileReader
		{
		private:
			std::FILE* file;

			FileReader(string fileName);

		public:
			static FileReader* New_Open(string fileName);
			string ReadToEndSync();
		};

		FileReader::FileReader(string fileName)
		{
			auto fname = fileName.cstr();
			fopen_s(&file, fname, "r"); // TODO check error
			delete[] fname;
		}

		FileReader* FileReader::New_Open(string fileName)
		{
			return new FileReader(fileName);
		}

		string FileReader::ReadToEndSync()
		{
			std::fseek(file, 0, SEEK_END);
			auto length = std::ftell(file);
			std::fseek(file, 0, SEEK_SET);
			auto buffer = new char[length];
			length = fread(buffer, 1, length, file);
			return string(length, buffer);
		}
	}

	namespace Text
	{
		class StringBuilder
		{
		};
	}
}