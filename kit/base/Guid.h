#ifndef _KIT_GUID_H_
#define _KIT_GUID_H_

#include <string>

namespace kit
{
    struct GuidStruct
    {
        unsigned int Data1;
        unsigned short Data2;
        unsigned short Data3;
        unsigned char Data4[8];
    };

    class Base16
    {
    public:
        static std::string encode(const std::string & data, bool up_cast=true);
        static std::string decode(const std::string & hex);
    };

    class Guid
    {
    public:
		bool is_empty() const
		{
			return (*this) == Guid::Null;
		}

        Guid()
        {
            clear();
        }

        Guid(const std::string & guid_str)
        {
            from_string(guid_str);
        }

        Guid(unsigned char * bytes)
        {
            from_bytes(bytes);
        }

        void from_string(const std::string & guid_str);

        std::string to_string() const;

        void from_bytes(unsigned char * bytes);

        void to_bytes(unsigned char * bytes) const;

        void clear()
        {
            memset((char*)&guid_, 0, sizeof(guid_));
        }

		friend bool operator==(
			const Guid & l, 
			const Guid & r)
		{
			return memcmp(&l.guid_, &r.guid_, sizeof(l.guid_)) == 0;
		}

		friend bool operator!=(
			const Guid & l, 
			const Guid & r)
		{
			return !(l == r);
		}

		friend bool operator<(
			const Guid & l, 
			const Guid & r)
		{
			return memcmp(&l.guid_, &r.guid_, sizeof(l.guid_)) < 0;
		}

        size_t get_int() const
        {
            return guid_.Data1;
        }

		static const Guid Null;

    private:
        GuidStruct guid_;
    };
}

#endif