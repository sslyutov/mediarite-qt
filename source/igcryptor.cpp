
/*!\brief Cryptor configurationn
* \file igcryptor
* \date October 01, 2022
* \author Sergey Slyutov
*/

#include "igcrypto.h"

#include "igcryptor.h"

#include <list>

#include <map>

#include <string>

#include<iostream>

#include <fstream>

#include <filesystem>


/*!\brief commonhint outputs standard recommendations to the console when insufficient parameters are specified*/
void commonhint()
{
	std::cout << "for help call igcryptor.exe -help" << std::endl;
}

CCryptoConfig::CCryptoConfig(int argc, char* argv[]) :
	m_inbuffer(NULL),
	m_inbufsize(0),
	m_outbuffer(NULL),
	m_outbufsize(0)
{

	for (int i = 0; i < argc; i++) {

		std::string strarg(argv[i]);

		if ((strarg.compare("enc") == 0) || (strarg.compare("dec") == 0)  ){
			m_operation = argv[i];
			continue;
		}

		if(strarg.compare("-in") == 0){
			if (++i < argc) {
				m_infile = argv[i];
				continue;
			}else{
				std::cout << "! - input file is missing. see -in [filename] - !" << std::endl;
				commonhint();
				return;

			}
		}

		if(strarg.compare("-out") == 0){
			if (++i < argc) {
				m_outfile = argv[i];
				continue;
			}else{
				std::cout << "! - output file is missing. see -out [filename] - !" << std::endl;
				commonhint();
				return;
			}
		}

		if(strarg.compare("-pass") == 0){
			if (++i < argc) {
				memcpy(m_password, "12345678901234567890123456789012", bit32);
				memcpy(m_password, argv[i], strlen(argv[i]));
				continue;
			}else{
				std::cout << "!- password is missing. see parameter [password]- !" << std::endl;
				commonhint();
				return;
			}
		}

		if (strarg.compare("-help") == 0){
			outputhelp();
			return;
		}
	}
};

CCryptoConfig::~CCryptoConfig()
{
};

/*!\brief validate all required parameters. Call this method before running crypto engine
* the successfull validation or the first invalid condition are reported to std::out
* \return true is all parameter of valid and ready to be passed to crypto engine.
* false if any parameter does not satisfy condition required for running crypto engine
*/
bool  CCryptoConfig::validate(void)
{
	std::ifstream infile;
	infile.open(m_infile,std::ios_base::in);
	if (!infile.is_open()) {
		std::cout << "!- cannot open input file: " << m_infile << " - !" << std::endl;
		outputhelp();
		return false;
	}
	infile.close();

	std::ofstream outfile;
	outfile.open(m_outfile, std::ios_base::out);
	if (!outfile.is_open()) {
		std::cout << "!- cannot open output file: " << m_outfile << " - !" << std::endl;
		outputhelp();
		return false;
	}
	outfile.close();

	return true;
};


void CCryptoConfig::read(void)
{
	if (m_inbuffer)
		delete m_inbuffer;

	std::ifstream infile;
	
	infile.open(m_infile, std::ios_base::binary);
	
	if (infile.is_open()) {

		std::filesystem::path p(m_infile);
	
		m_inbufsize = (uint32_t)std::filesystem::file_size(p);
		
		m_inbuffer = new char [m_inbufsize];

		//infile.tellg();
//		while (infile.good()) {
		infile.read(m_inbuffer, m_inbufsize);
		//}
	}else{
		std::cout << "! - coudl not open file " << m_infile << " - !" << std::endl;
		outputhelp();
	}

	infile.close();

	std::cout << "file " << m_infile << " has been successfully read." << " bytes count is " << m_inbufsize << std::endl;

};


void CCryptoConfig::run(void)
{

	if (m_outbuffer)
		delete m_outbuffer;
	
	if (m_operation.compare("enc") == 0)
	{
		ns_igcrypto::encrypt(m_inbuffer, m_inbufsize, &m_outbuffer, &m_outbufsize, m_password);
		std::cout << m_outbufsize << " bytes has been encrypted" << std::endl;
		return;
	}
	if (m_operation.compare("dec") == 0)
	{
		ns_igcrypto::decrypt(m_inbuffer, m_inbufsize, &m_outbuffer, &m_outbufsize, m_password);
		std::cout << m_outbufsize << " bytes has been deccrypted" << std::endl;
		return;
	}
	std::cout << "! - missing parameter [operation]. enc or dec has not been specified - !" << std::endl;
	outputhelp();
};


void CCryptoConfig::write(void)
{
	std::ofstream outfile;

	outfile.open(m_outfile, std::ios_base::binary);

	if (outfile.is_open()) {

		outfile.write(m_outbuffer, m_outbufsize);

	}
	else {
		outputhelp();
	}

	outfile.close();

	std::cout << "file '" << m_infile << "' has been successfully decripted into file '" << m_outfile << "'" << std::endl;
};


/*!\brief output brief manual to the utiltiy */
void outputhelp()
{
	std::cout << "" << std::endl;
	std::cout << "igcryptor utility provides TEA encryption functions                 " << std::endl;
	std::cout << "format: igcryptor [operation] -in [filename] -out [filename] -pass [password] " << std::endl;
	std::cout << " [operation] - enc to ecrypt                                                  " << std::endl;
	std::cout << "               dec to decrypto                                                " << std::endl;
	std::cout << " -in [filename] - specifies input file                                        " << std::endl;
	std::cout << " -out[filename] - specifies output file                                       " << std::endl;
	std::cout << " -pass[password] - password to crypting                                       " << std::endl;
	std::cout << "all parameters are required                                                   " << std::endl;
	std::cout << "password upto 32 bytes leangth                                                " << std::endl;
	std::cout << "                                                                              " << std::endl;
	std::cout << "examples:                                                                     " << std::endl;
	std::cout << "igcryptor enc - in original.txt -out encrypted.enc.txt -pass yourpassword     " << std::endl;
	std::cout << "igcryptor dec - in encrypted.txt -out decrypted.dec.txt -pass yourpassword    " << std::endl;
	std::cout << "" << std::endl;
};