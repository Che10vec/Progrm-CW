#pragma once
#include<vector>
#include<complex>
#include<cmath>
struct wav {
	uint8_t ChunkID[4] = {0,0,0,0};//"RIFF"
	uint8_t ChunkSize[4] = { 0,0,0,0 };
	uint8_t Format[4] = { 0,0,0,0 };//"WAVE"
	uint8_t Subchunk1ID[4] = { 0,0,0,0 };//"fmt "
	uint8_t Subchunk1Size[4] = { 0,0,0,0 };
	uint8_t AudioFormat[2] = { 0,0};//1
	uint8_t NumChannels[2] = { 0,0 };//1
	uint8_t SampleRate[4] = { 0,0,0,0 }; //44100
	uint8_t ByteRate[4] = { 0,0,0,0 };//44100 * BitsPerSample/8
	uint8_t BlockAlign[2] = { 0,0 };//BitsPerSample/8
	uint8_t BitsPerSample[2] = { 0,0 };//16 for tests
	uint8_t Subchunk2ID[4] = { 0,0,0,0 };//"data"
	uint8_t Subchunk2Size[4] = { 0,0,0,0 };
	std::vector<uint8_t> Data;
};
struct ngff {
	uint8_t ChunkID[4] = { 0,0,0,0 };//"NGFF"
	uint8_t ChunkSize[4] = { 0,0,0,0 };//4 + 8 + Subchunk1ID + 8
	uint8_t Format[4]= {0,0,0,0};//"NGFF"
	uint8_t Subchunk1ID[4] = { 0,0,0,0 };//"fmt "
	uint8_t Subchunk1Size[4] = { 0,0,0,0 };
	uint8_t LowerFreqDomain[4] = { 0,0,0,0 };//0
	uint8_t UpperFreqDomain[4] = { 0,0,0,0 };//22050
	uint8_t EncodingType[1] = { 0 };//1 is complex double
	uint8_t Subchunk2ID[4] = { 0,0,0,0 };//"data"
	uint8_t Subchunk2Size[4] = { 0,0,0,0 };
	std::vector<double> Data;
};
class Audio
{
public:
	static const int SampleRate = 44100;
	uint16_t bitresolution = 16;
	std::string name;
	uint32_t size = 0;

	void convert_from_wav(wav wav);
	wav convert_to_wav();
	void convert_from_ngff(ngff ngff);
	ngff convert_to_ngff();
	void concatenation(Audio* a);
	void addition(Audio* a);
	void subtraction(Audio* a);
	void multiplication(Audio* a);
	void division(Audio* a);
	void set(double first, double last, double value);
	void remove_obj();
	static Audio* create_obj(std::string name, wav wav);
	static Audio* create_obj(std::string name, ngff ngff);
	void rename_obj(std::string s);
	Audio* copy_obj();
private:
	void zero_remover_t();
	void zero_remover_f();
	size_t powof2(size_t s);
	void FFT();
	void ReFFT();
	std::vector<std::complex<double>> time_dom;
	std::vector<std::complex<double>> freq_dom;
	void fft(std::vector<std::complex<double>>& vec);
	void refft(std::vector<std::complex<double>>& vec);
};
