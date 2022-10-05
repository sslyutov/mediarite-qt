/*!\brief Cryptor configurationn
* \file igcryptor
* \date October 01, 2022
* \author Sergey Slyutov
*/
#ifndef _IGCRYPTOR_H

#define _IGCRYPTOR_H

#include <string>

class CCryptoConfig
{
public:

	CCryptoConfig(int argc, char* argv[]);

	~CCryptoConfig();

	bool validate(void);

	void read(void);

	void run(void);

	void write(void);

private:

	std::string m_operation;

	std::string m_infile;

	std::string m_outfile;

	enum passdepth{ 
		bit32 = 32
	};

	
	char m_password[bit32];

	char* m_inbuffer;
	uint32_t m_inbufsize;

	char* m_outbuffer;
	uint32_t m_outbufsize;
};

void outputhelp();

#endif // IGCRYPTOR_H