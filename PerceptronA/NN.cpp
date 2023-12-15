#include "NN.h"
	double NN::GetI(ul layer, ul oneuron) {
		double val = 0;
		ul neuron = 0;
		while (neuron < arch.lsize[layer]) {
			val += neuron_vals[layer][neuron] * neuron_weights[layer][oneuron][neuron];
			neuron++;
		}
		val += neuron_weights[layer][oneuron][neuron];
		return val;
	}
	void NN::RandomWeights() {
		ul layer = 0;
		while (layer < arch.lcount - 1) {
			ul weight = 0;
			while (weight < arch.lsize[layer]+1) {
				ul neuron = 0;
				while (neuron < arch.lsize[layer + 1]) {
					neuron_weights[layer][neuron][weight] = (((double)((rand() | (rand() << 16)) % 10000001)) / 10000000)*2-1;
					neuron++;
				}
				weight++;
			}
			layer++;
		}
	}
	void NN::RandomValues() {
		ul layer = 0;
		while (layer < arch.lcount) {
			ul neuron = 0;
			while (neuron < arch.lsize[layer]) {
				neuron_vals[layer][neuron] = (((double)((rand() | (rand() << 16)) % 10000001)) / 10000000) * 2 - 1;
				neuron_in[layer][neuron] = (((double)((rand() | (rand() << 16)) % 10000001)) / 10000000) * 2 - 1;
				neuron++;
			}
			layer++;
		}
	}
	void NN::SetAF(ui m) {
		AF.set(m);
	}
	void NN::Init(NN_Arch _arch) {
		arch = _arch;
		uint32_t alloccur = 0; 
		ul cn = 0;
		ul l = 0;
		allocsize = 0;
		allocsize += arch.lcount * sizeof(double*) * 2;
		allocsize += arch.lcount * sizeof( double**) * 2;
		while (l < arch.lcount) {
			allocsize += sizeof(double) * arch.lsize[l]*2;
			if (l != arch.lcount - 1) {
				allocsize += arch.lsize[l + 1] * sizeof(double*)*2;
				cn = 0;
				while (cn < arch.lsize[l]) {
					allocsize += sizeof(double) * (1+arch.lsize[l]) * 2;
					cn++;
				}
			}
			if (maxlayersize < arch.lsize[l]) {
				maxlayersize = arch.lsize[l];
			}
			l++;
		}
		allocsize += maxlayersize * sizeof(double) * 2;
		allocsize += sizeof(double) * arch.lsize[arch.lcount - 1];
		allocsize += sizeof(double) * maxlayersize * 2;
		memory = malloc(allocsize);
		neuron_vals = (double**)((uint64_t)memory + (uint64_t)alloccur);
		alloccur += arch.lcount * sizeof(double*);
		neuron_errder = (double**)((uint64_t)memory + (uint64_t)alloccur);
		alloccur += arch.lcount * sizeof(double*);
		neuron_weights = (double***)((uint64_t)memory + (uint64_t)alloccur);
		alloccur += arch.lcount * sizeof(double**);
		neuron_in = (double**)((uint64_t)memory + (uint64_t)alloccur);
		alloccur += arch.lcount * sizeof(double*);
		weight_change = (double***)((uint64_t)memory + (uint64_t)alloccur); 
		alloccur += arch.lcount * sizeof(double**);
		l = 0;
		while (l < arch.lcount) {
			neuron_in[l] = (double*)((uint64_t)memory + (uint64_t)alloccur);
			alloccur += sizeof(double) * arch.lsize[l];
			neuron_vals[l] = (double*)((uint64_t)memory + (uint64_t)alloccur);
			alloccur += sizeof(double) * arch.lsize[l];
			neuron_errder[l] = (double*)((uint64_t)memory + (uint64_t)alloccur);
			alloccur += sizeof(double) * arch.lsize[l];
			cn = 0;
			/*if (l != arch.lcount - 1) {
				while (cn < arch.lsize[l]) {
					ul on = 0;
					while (on < arch.lsize[l + 1]) {
						weight_change[l][on][cn] = 0;
						on++;
					}
					cn++;
				}
			}*/
			if (l != arch.lcount - 1) {
				weight_change[l] = (double**)((uint64_t)memory + (uint64_t)alloccur);
				alloccur += sizeof(double*) * arch.lsize[l+1];
				neuron_weights[l] = (double**)((uint64_t)memory + (uint64_t)alloccur);
				alloccur += sizeof(double*) * arch.lsize[l+1];
				cn = 0;
				while (cn < arch.lsize[l+1]) {
					neuron_weights[l][cn] = (double*)((uint64_t)memory + (uint64_t)alloccur);
					alloccur += sizeof(double) * (arch.lsize[l]+1);
					weight_change[l][cn] = (double*)((uint64_t)memory + (uint64_t)alloccur);
					alloccur += sizeof(double) * (arch.lsize[l]+1);
					cn++;
				}
			}
			l++;
		}
		ideals = (double*)((uint64_t)memory + (uint64_t)alloccur);
		alloccur += sizeof(double) * arch.lsize[arch.lcount - 1];
		deltas_buf = (double*)((uint64_t)memory + (uint64_t)alloccur);
		alloccur += sizeof(double) * maxlayersize;
		deltas_buf2 = (double*)((uint64_t)memory + (uint64_t)alloccur);
		alloccur += sizeof(double) * maxlayersize;
		SetAF(arch.AF);
 		RandomWeights();
		//RandomValues();
	}
	double NN::ForwardFeed() {
		ul layer = 1;
		ul neuron;
		double a;
		while (layer < arch.lcount) {
			neuron = 0;
			while (neuron < arch.lsize[layer]) {
				a = GetI(layer - 1, neuron);
				neuron_in[layer][neuron] = a;
				neuron_vals[layer][neuron] = AF.use(a);
				neuron++;
			}
			layer++;
		}
		neuron = 0;
		double val = 0;
		while (neuron < arch.lsize[arch.lcount - 1]) {
			val += pow(neuron_vals[arch.lcount - 1][neuron]-ideals[neuron],2);
			neuron++;
		}
		return val;
	}
	void NN::ZeroChangeFill() {
		ui layer = 0;
		while (layer < arch.lcount-1) {
			ui neuron = 0;
			while (neuron < arch.lsize[layer]) {
				ui weight = 0;
				while (weight < arch.lsize[layer + 1]) {
					weight_change[layer][weight][neuron] = 0;
					weight++;
				}
				neuron++;
			}
			layer++;
		}
	}
	void NN::ApplyWeightChanges(double N) {
		ui layer = 0;
		ui weight;
		while (layer < arch.lcount - 1) {
			ui neuron = 0;
			while (neuron < arch.lsize[layer]) {
				weight = 0;
				while (weight < arch.lsize[layer + 1]) {
					neuron_weights[layer][weight][neuron] += neuron_errder[layer + 1][weight] * neuron_vals[layer][neuron] * N;
					//std::cerr << neuron_errder[layer + 1][weight] * neuron_vals[layer][neuron] * N << std::endl;
					weight++;
				}
				neuron++;
			}
			weight = 0;
			while (weight < arch.lsize[layer + 1]) {
				neuron_weights[layer][weight][neuron] += neuron_errder[layer + 1][weight] * N;
				//std::cerr << neuron_errder[layer + 1][weight] * N << std::endl;
				weight++;
			}
			layer++;
		}
	}
	//======================================================================
	void NN::BackPropogation(double N) {
		ZeroChangeFill();
		ul layer = arch.lcount-1;
		ul neuron = 0;
		ul oneuron = 0;
		while (neuron < arch.lsize[layer]) {
			neuron_errder[layer][neuron] = 2*(ideals[neuron]-neuron_vals[layer][neuron]) * AF.useDer(neuron_vals[layer][neuron]);
			neuron++;
		}
		layer--;
		while (layer > 0) {
			neuron = 0;
			while (neuron < arch.lsize[layer]) {
				//neuron_errder[layer][neuron] = AF.useDer(neuron_in[layer][neuron]);
				neuron_errder[layer][neuron] = 0;
				oneuron = 0;
				while (oneuron < arch.lsize[layer + 1]) {
					neuron_errder[layer][neuron] += neuron_errder[layer+1][oneuron]*neuron_weights[layer][oneuron][neuron];
					//std::cerr << neuron_errder[layer + 1][oneuron] << " " <<  neuron_weights[layer][oneuron][neuron] << std::endl;
					oneuron++;
				}
				neuron_errder[layer][neuron] *= AF.useDer(neuron_vals[layer][neuron]);
				neuron++;
			}
			layer--;
		}
		ApplyWeightChanges(N);
	}
	//======================================================================
	void NN::SetInput(double* input) {
		ul neuron = 0;
		while (neuron < arch.lsize[0]) {
			neuron_vals[0][neuron] = input[neuron];
			neuron++;
		}
	}
	void NN::SetIdeals(double* ideals_) {
		ul ideal = 0;
		while (ideal < arch.lsize[arch.lcount - 1]) {
			ideals[ideal] = ideals_[ideal];
			ideal++;
		}
	}
	void NN::PrintVals(ul layer) {
		ul c = 0;
		while (c < arch.lsize[layer]) {
			std::cout << c << " " << neuron_vals[layer][c] << std::endl;
			c++;	
		}
	}
	void NN::PrintWeights(ul layer) {
		ul neuron = 0;
		while (neuron < arch.lsize[layer]) {
			ul oneuron = 0;
			std::cerr << neuron << ": ";
			while (oneuron < arch.lsize[layer + 1]) {
				std::cerr << oneuron << " - " << neuron_weights[layer][oneuron][neuron] << ", ";
				oneuron++;
			}
			std::cerr << "\n";
			neuron++;
		}

	}
	void NN::PrintAllInfo() {
		std::cerr << "(Neural Network)" << ":\n";
		std::cerr << "(Layers count: " << arch.lcount << " Activation function: " << arch.AF << ")" <<  std::endl;
		std::cerr << "(Layers data:)\n";
		ul layer = 0;
		while (layer < arch.lcount) {
			std::cerr << "( layer: " << layer;
			std::cerr << " Neurons count: " << arch.lsize[layer]; 
			std::cerr << " Neurons values: {";
			ul neuron = 0;
			while (neuron < arch.lsize[layer] - 1) {
				std::cerr << neuron_vals[layer][neuron] << ",";
				neuron++;
			}
			std::cerr << neuron_vals[layer][neuron] << "}";
			if (layer != arch.lcount - 1) {
				std::cerr << " Neuron Weights: {";
				neuron = 0;
				while (neuron < arch.lsize[layer] - 1) {
					std::cerr << "{";
					ul weight = 0;
					while (weight < arch.lsize[layer + 1] - 1) {
						std::cerr << neuron_weights[layer][weight][neuron] << ",";
						weight++;
					}
					std::cerr << neuron_weights[layer][weight][neuron];
					std::cerr << "},";
					neuron++;
				}
				std::cerr << "{";
				ul weight = 0;
				while (weight < arch.lsize[layer + 1] - 1) {
					std::cerr << neuron_weights[layer][weight][neuron] << ",";
					weight++;
				}
				std::cerr << neuron_weights[layer][weight][neuron];
				std::cerr << "}";
				std::cerr << "}";
			}
			std::cerr << " )" << std::endl;
			layer++;
		}
	}
	//FILE ARCH
	/*
	4 UL - Layers Count (N0)
	4 UL - Layer 0 Size
	...
	4 UL - Layer N0 Size

	double*** - Weight Buffer
	*/
	void NN::SaveToFile(wchar_t * path) {
		HANDLE file = CreateFileW(path, GENERIC_READ | GENERIC_WRITE,0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY,NULL);
		SetFilePointer(file,0,NULL,0);
		WriteFile(file,(void *)arch.lcount,4,NULL,NULL);
		ul layer = 0;
		while (layer < arch.lcount) {
			SetFilePointer(file, 4+(layer*4), NULL, 0);
			WriteFile(file, (void*)(arch.lsize[layer]), 4, NULL, NULL);
			layer++;
		}
		ul skip = 4 + (layer * 4);
		SetFilePointer(file, skip, NULL, 0);
		layer = 0;
		while (layer < arch.lcount - 1) {
			ul an = 0;
			while (an < arch.lsize[layer]) {
				ul bn = 0;
				while (bn < arch.lsize[layer + 1]) {
					SetFilePointer(file, skip, NULL, 0);
					WriteFile(file, (void*)(&(neuron_weights[layer][bn][an])), sizeof(double), NULL, NULL);
					skip += sizeof(double);
					bn++;
				}
				an++;
			}
			layer++;
		}
	}
	void NN::OpenFromFile(wchar_t* path) {
		HANDLE file = CreateFileW(path, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY, NULL);
		SetFilePointer(file, 0, NULL, 0);
		ReadFile(file, (void*)arch.lcount, 4, NULL, NULL);
		ul layer = 0;
		while (layer < arch.lcount) {
			SetFilePointer(file, 4 + (layer * 4), NULL, 0);
			ReadFile(file, (void*)(arch.lsize[layer]), 4, NULL, NULL);
			layer++;
		}
		ul skip = 4 + (layer * 4);
		SetFilePointer(file, skip, NULL, 0);
		layer = 0;
		while (layer < arch.lcount - 1) {
			ul an = 0;
			while (an < arch.lsize[layer]) {
				ul bn = 0;
				while (bn < arch.lsize[layer + 1]) {
					SetFilePointer(file, skip, NULL, 0);
					ReadFile(file, (void*)(&(neuron_weights[layer][bn][an])), sizeof(double), NULL, NULL);
					skip += sizeof(double);
					bn++;
				}
				an++;
			}
			layer++;
		}
	}