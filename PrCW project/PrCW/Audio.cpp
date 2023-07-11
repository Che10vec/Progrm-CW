#include "Audio.h"
/// <summary>
/// private func of Audio class; made to be recursive to follow fft algorithm 
/// </summary>
void Audio::fft(std::vector<std::complex<double>>& vec) {
	if (vec.size() != 1) {
		std::vector<std::complex<double>> vec1(vec.size() / 2);
		std::vector<std::complex<double>> vec2(vec.size() / 2);
		for (size_t l = 0; 2 * l < vec.size(); l++) {
			vec1[l] = vec[2 * l];
			vec2[l] = vec[2 * l + 1];
		}
		fft(vec1);
		fft(vec2);
		double rou = 2 * acos(-1)/*Pi*/ / vec.size();
		std::complex<double> w(1);
		std::complex<double> wn(cos(rou), sin(rou));
		for (size_t l = 0; 2 * l < vec.size(); l++) {
			vec[l] = vec1[l] + w * vec2[l];
			vec[l + vec.size() / 2] = vec1[l] - w * vec2[l];
			w *= wn;
		}
	}
}
/// <summary>
/// private func of Audio class; made to be recursive to follow fft algorithm (reverse)
/// </summary>
void Audio::refft(std::vector<std::complex<double>>& vec) {
	if (vec.size() != 1) {
		std::vector<std::complex<double>> vec1(vec.size() / 2), vec2(vec.size() / 2);
		for (size_t l = 0; 2 * l < vec.size(); l++) {
			vec1[l] = vec[2 * l];
			vec2[l] = vec[2 * l + 1];
		}
		refft(vec1);
		refft(vec2);
		double rou = -2 * acos(-1)/*Pi*/ / vec.size();
		std::complex<double> w(1);
		std::complex<double>wn(cos(rou), sin(rou));
		for (size_t l = 0; 2 * l < vec.size(); l++) {
			vec[l] = vec1[l] + w * vec2[l];
			vec[l + vec.size() / 2] = vec1[l] - w * vec2[l];
			vec[l] /= 2;
			vec[l + vec.size() / 2] /= 2;
			w *= wn;
		}
	}
}
/// <summary>
/// Initialize fft algorithm
/// </summary>
void Audio::FFT() {
	std::vector<std::complex<double>> time_dom_temp = this->time_dom;
	std::vector<std::complex<double>> freq_dom_temp;
	size_t pow2 = powof2(time_dom_temp.size());
	time_dom_temp.resize(pow2, 0);
	this->time_dom.resize(pow2, 0);
	this->fft(time_dom_temp);
	this->freq_dom = time_dom_temp;
	this->size = (uint32_t)this->time_dom.size();
};
/// <summary>
/// Initialize fft algorithm (reverse)
/// </summary>
void Audio::ReFFT() {
	std::vector<std::complex<double>> freq_dom_temp = this->freq_dom;
	size_t pow2 = powof2(freq_dom_temp.size());
	freq_dom_temp.resize(pow2, 0);
	this->freq_dom.resize(pow2, 0);
	this->refft(freq_dom_temp);
	this->time_dom = freq_dom_temp;;
	this->size = (uint32_t)this->time_dom.size();
};
/// <summary>
/// give closest greates power of 2
/// </summary>
/// <param name="s">for size</param>
/// <returns>power of 2</returns>
size_t Audio::powof2(size_t s) {
	size_t l = 1;
	while (l < s) {
		l <<= 1;
	}
	if (l == 0) {
		throw _HAS_EXCEPTIONS;
	}
	else {
		return l;
	}
}
/// <summary>
/// 
/// </summary>
void Audio::zero_remover_t() {
	for (size_t l = this->time_dom.size() - 1; l > 0; l--) {
		if (this->time_dom[l].real() == 0) {
			this->time_dom.erase(this->time_dom.begin() + l);
		}
		else {
			break;
		}
	}
	this->size = (uint32_t)this->time_dom.size();
};
/// <summary>
/// 
/// </summary>
void Audio::zero_remover_f() {
	std::complex<double> zero{ 0,0 };
	for (size_t l = this->freq_dom.size() - 1; l >= 0 && this->freq_dom[l] == zero; l--) {
		this->freq_dom.erase(this->freq_dom.begin() + l);
	}
	this->size = (uint32_t)this->freq_dom.size();
};
/// <summary>
/// 
/// </summary>
/// <param name="wav">data from file</param>
void Audio::convert_from_wav(wav wav) {
	this->bitresolution = (wav.BitsPerSample[1] << 8) + wav.BitsPerSample[0];
	int samplesize = (this->bitresolution / 8);
	double tempd;
	uint64_t tempui = 0;
	for (size_t l1 = 0; l1 < wav.Data.size()/ samplesize; l1++) {
		for (size_t l2 = 0; l2 < samplesize;l2++) {
			tempui += (uint64_t)wav.Data[l1* samplesize + l2] << (l2 * 8);
		}
		tempd = double(tempui) / (pow(2, this->bitresolution - 1) -1)-1;
		tempui = 0;
		this->time_dom.push_back(tempd);
	};
};
/// <summary>
/// use current data at time_dom and turn it to wav 
/// </summary>
/// <returns>data for file</returns>
wav Audio::convert_to_wav() {
	this->zero_remover_t();
	uint32_t samplerate = this->SampleRate;
	uint32_t byterate = samplerate * this->bitresolution / 8;
	uint32_t subchunk1size = 16;
	uint32_t subchunk2size = this->size * this->bitresolution / 8;
	uint32_t chunksize = 4 + 8 + subchunk1size + 8 + subchunk2size;
	uint16_t align = this->bitresolution / 8;
	wav w;
	w.ChunkID[0] = 'R';
	w.ChunkID[1] = 'I';
	w.ChunkID[2] = 'F';
	w.ChunkID[3] = 'F';
	w.ChunkSize[0] = (uint8_t)(chunksize >> (8 * 0));
	w.ChunkSize[1] = (uint8_t)(chunksize >> (8 * 1));
	w.ChunkSize[2] = (uint8_t)(chunksize >> (8 * 2));
	w.ChunkSize[3] = (uint8_t)(chunksize >> (8 * 3));
	w.Format[0] = 'W';
	w.Format[1] = 'A';
	w.Format[2] = 'V';
	w.Format[3] = 'E';
	w.Subchunk1ID[0] = 'f';
	w.Subchunk1ID[1] = 'm';
	w.Subchunk1ID[2] = 't';
	w.Subchunk1ID[3] = ' ';
	w.Subchunk1Size[0] = (uint8_t)(subchunk1size >> (8 * 0));
	w.Subchunk1Size[1] = (uint8_t)(subchunk1size >> (8 * 1));
	w.Subchunk1Size[2] = (uint8_t)(subchunk1size >> (8 * 2));
	w.Subchunk1Size[3] = (uint8_t)(subchunk1size >> (8 * 3));
	w.AudioFormat[0] = 1;
	w.NumChannels[0] = 1;
	w.SampleRate[0] = (uint8_t)(samplerate >> (8 * 0));
	w.SampleRate[1] = (uint8_t)(samplerate >> (8 * 1));
	w.SampleRate[2] = (uint8_t)(samplerate >> (8 * 2));
	w.SampleRate[3] = (uint8_t)(samplerate >> (8 * 3));
	w.ByteRate[0] = (uint8_t)(byterate >> (8 * 0));
	w.ByteRate[1] = (uint8_t)(byterate >> (8 * 1));
	w.ByteRate[2] = (uint8_t)(byterate >> (8 * 2));
	w.ByteRate[3] = (uint8_t)(byterate >> (8 * 3));
	w.BlockAlign[0] = (uint8_t)(align >> (8 * 0));
	w.BlockAlign[1] = (uint8_t)(align >> (8 * 1));
	w.BitsPerSample[0] = (uint8_t)(this->bitresolution >> (8 * 0));
	w.BitsPerSample[1] = (uint8_t)(this->bitresolution >> (8 * 1));
	w.Subchunk2ID[0] = 'd';
	w.Subchunk2ID[1] = 'a';
	w.Subchunk2ID[2] = 't';
	w.Subchunk2ID[3] = 'a';
	w.Subchunk2Size[0] = (uint8_t)(subchunk2size >> (8 * 0));
	w.Subchunk2Size[1] = (uint8_t)(subchunk2size >> (8 * 1));
	w.Subchunk2Size[2] = (uint8_t)(subchunk2size >> (8 * 2));
	w.Subchunk2Size[3] = (uint8_t)(subchunk2size >> (8 * 3));
	double tempd;
	uint64_t tempui;
	for (std::complex<double> sample : this->time_dom) {
		tempd = sample.real() + 1;
		tempui = (uint64_t)(tempd * (pow(2, this->bitresolution - 1) - 1));
		for (int l = 0; l < this->bitresolution / 8;l++) {
			w.Data.push_back((uint8_t)(tempui>>(8*l)));
		};
	}
	return w;
};
/// <summary>
/// 
/// </summary>
/// <param name="ngff">data from file</param>
void Audio::convert_from_ngff(ngff ngff) {
	this->freq_dom.resize(ngff.Data.size()/2);
	for (size_t l = 0; l<this->freq_dom.size(); l++) {
		this->freq_dom[l] = std::complex<double>(ngff.Data[l * 2], ngff.Data[l * 2 + 1]);
	};
};
/// <summary>
/// use current data at freq_dom to turn it to ngff
/// </summary>
/// <returns>data for file</returns>
ngff Audio::convert_to_ngff() {
	this->zero_remover_f();
	ngff n;
	uint32_t subchunk1size = 9;
	uint32_t subchunk2size = this->size * 2 * sizeof(double);
	uint32_t chunksize = 4 + 8 + subchunk1size + 8 + subchunk2size;
	uint32_t maxfreq = this->SampleRate / 2;
	
	n.ChunkID[0] = 'N';
	n.ChunkID[1] = 'G';
	n.ChunkID[2] = 'F';
	n.ChunkID[3] = 'F';
	n.ChunkSize[0] = (uint8_t)(chunksize >> (8 * 0));
	n.ChunkSize[1] = (uint8_t)(chunksize >> (8 * 1));
	n.ChunkSize[2] = (uint8_t)(chunksize >> (8 * 2));
	n.ChunkSize[3] = (uint8_t)(chunksize >> (8 * 3));
	n.Format[0] = 'N';
	n.Format[1] = 'G';
	n.Format[2] = 'F';
	n.Format[3] = 'F';
	n.Subchunk1ID[0] = 'f';
	n.Subchunk1ID[1] = 'm';
	n.Subchunk1ID[2] = 't';
	n.Subchunk1ID[3] = ' ';
	n.Subchunk1Size[0] = (uint8_t)(subchunk1size >> (8 * 0));
	n.Subchunk1Size[1] = (uint8_t)(subchunk1size >> (8 * 1));
	n.Subchunk1Size[2] = (uint8_t)(subchunk1size >> (8 * 2));
	n.Subchunk1Size[3] = (uint8_t)(subchunk1size >> (8 * 3));
	n.LowerFreqDomain[0] = 0;
	n.LowerFreqDomain[1] = 0;
	n.LowerFreqDomain[2] = 0;
	n.LowerFreqDomain[3] = 0;
	n.UpperFreqDomain[0] = (uint8_t)(maxfreq >> (8 * 0));
	n.UpperFreqDomain[1] = (uint8_t)(maxfreq >> (8 * 1));
	n.UpperFreqDomain[2] = (uint8_t)(maxfreq >> (8 * 2));
	n.UpperFreqDomain[3] = (uint8_t)(maxfreq >> (8 * 3));
	n.EncodingType[0] = 1;
	n.Subchunk2ID[0] = 'd';
	n.Subchunk2ID[1] = 'a';
	n.Subchunk2ID[2] = 't';
	n.Subchunk2ID[3] = 'a';
	n.Subchunk2Size[0] = (uint8_t)(subchunk2size >> (8 * 0));
	n.Subchunk2Size[1] = (uint8_t)(subchunk2size >> (8 * 1));
	n.Subchunk2Size[2] = (uint8_t)(subchunk2size >> (8 * 2));
	n.Subchunk2Size[3] = (uint8_t)(subchunk2size >> (8 * 3));
	for (std::complex<double> freq : this->freq_dom) {
		n.Data.push_back(freq.real());
		n.Data.push_back(freq.imag());
	};
	return n;
};
/// <summary>
/// 
/// </summary>
/// <param name="a"></param>
void Audio::concatenation(Audio* a) {
	this->zero_remover_t();
	a->zero_remover_t();
	this->time_dom.insert(this->time_dom.end(), a->time_dom.begin(), a->time_dom.end());
	this->FFT();
};
/// <summary>
/// add 2 objects, altering the first
/// </summary>
/// <param name="a">second object</param>
void Audio::addition(Audio* a) {
	if (this->time_dom.size() < a->time_dom.size()) {
		this->time_dom.resize(a->time_dom.size());
	}
	for (size_t l = 0; l < this->time_dom.size() && l < a->time_dom.size(); l++) {
		this->time_dom[l] += a->time_dom[l];
	};
	this->FFT();
};
/// <summary>
/// subtraction of 2 objects, altering the first
/// </summary>
/// <param name="a">second object</param>
void Audio::subtraction(Audio* a) {
	if (this->time_dom.size() < a->time_dom.size()) {
		this->time_dom.resize(a->time_dom.size());
	}
	for (size_t l = 0; l < this->time_dom.size() && l < a->time_dom.size(); l++) {
		this->time_dom[l] -= a->time_dom[l];
	};
	this->FFT();
};
/// <summary>
/// multiply 2 objects, altering the first
/// </summary>
/// <param name="a">second object</param>
void Audio::multiplication(Audio* a) {
	if (this->freq_dom.size() < a->freq_dom.size()) {
		this->freq_dom.resize(a->freq_dom.size());
	}
	for (size_t l = 0; l < this->freq_dom.size() && l < a->freq_dom.size(); l++) {
		this->freq_dom[l] *= a->freq_dom[l];
	};
	this->ReFFT();
};
/// <summary>
/// division of 2 objects, altering the first
/// </summary>
/// <param name="a">second object</param>
void Audio::division(Audio* a) {
	if (this->freq_dom.size() < a->freq_dom.size()) {
		this->freq_dom.resize(a->freq_dom.size());
	}
	for (size_t l = 0; l < this->freq_dom.size() && l < a->freq_dom.size(); l++) {
		this->freq_dom[l] /= a->freq_dom[l];
	};
	this->ReFFT();
};
/// <summary>
/// set the frequancy within the bound
/// </summary>
/// <param name="first">lower bound</param>
/// <param name="last">upper bound</param>
/// <param name="value"> to set</param>
void Audio::set(double first, double last, double value) {
	double delta_freq = double(this->SampleRate / 2) / this->freq_dom.size();
	size_t pos_first = size_t(first / delta_freq);
	size_t pos_last = size_t(last / delta_freq);
	for (size_t l = pos_first; l < pos_last; l++) {
		this->freq_dom[l] = value;
	};
	this->ReFFT();
};
/// <summary>
/// removes object
/// </summary>
void Audio::remove_obj() {
	delete this;
};
/// <summary>
/// creates object
/// </summary>
/// <param name="name"></param>
/// <param name="wav">struct representing file format</param>
/// <returns></returns>
Audio* Audio::create_obj(std::string name, wav wav) {
	Audio* a = new Audio;
	a->name = name;
	a->convert_from_wav(wav);
	a->FFT();
	return a;
};
/// <summary>
/// creates object
/// </summary>
/// <param name="name"></param>
/// <param name="idk">struct representing file format</param>
/// <returns></returns>
Audio* Audio::create_obj(std::string name, ngff ngff) {
	Audio* a = new Audio;
	a->name = name;
	a->convert_from_ngff(ngff);
	a->ReFFT();
	return a;
};
/// <summary>
/// change name of the object
/// </summary>
/// <param name="s">new name</param>
void Audio::rename_obj(std::string s) {
	this->name = s;
};
/// <summary>
/// copying object
/// </summary>
/// <returns>new object</returns>
Audio* Audio::copy_obj() {
	Audio* a = new Audio;
	a->bitresolution = this->bitresolution;
	a->name = this->name;
	a->size = this->size;
	a->time_dom = this->time_dom;
	a->freq_dom = this->freq_dom;
	return a;
};
